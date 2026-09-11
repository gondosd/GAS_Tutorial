// Copyright Daniel Göndös 2026


#include "Characters/GAS_EnemyCharacter.h"
#include "AbilitySystem/GAS_AbilitySystemComponent.h"
#include "AbilitySystem/GAS_AttributeSet.h"


AGAS_EnemyCharacter::AGAS_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGAS_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGAS_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AGAS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetAbilitySystemComponent()) return;
	
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	
	if (!HasAuthority()) return; //Only on server we give startup abilities

	GiveStartupAbilities();
	InitializeAttributes();
}
