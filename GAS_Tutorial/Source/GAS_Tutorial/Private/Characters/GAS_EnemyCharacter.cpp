// Copyright Daniel Göndös 2026


#include "Characters/GAS_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GAS_AbilitySystemComponent.h"
#include "AbilitySystem/GAS_AttributeSet.h"
#include "GameplayTags/GAS_Tags.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/AIModule/Classes/AIController.h"


AGAS_EnemyCharacter::AGAS_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSet>("AttributeSet");
}

void AGAS_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

UAbilitySystemComponent* AGAS_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGAS_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AGAS_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	AAIController* AIController = GetController<AAIController>();
	if (AIController)
		AIController->StopMovement();

	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
}

void AGAS_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, GASTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
}

void AGAS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GetAbilitySystemComponent()) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if (!HasAuthority()) return; //Only on server we give startup abilities

	GiveStartupAbilities();
	InitializeAttributes(InitializeAttributeEffect);

	UGAS_AttributeSet* GAS_AttributeSet = Cast<UGAS_AttributeSet>(GetAttributeSet());
	if (!GAS_AttributeSet) return;

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GAS_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void AGAS_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	AAIController* AIController = GetController<AAIController>();

	if (AIController)
		AIController->StopMovement();
}
