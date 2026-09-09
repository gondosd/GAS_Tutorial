// Copyright Daniel Göndös 2026


#include "Characters/GAS_EnemyCharacter.h"
#include "AbilitySystem/GAS_AbilitySystemComponent.h"


AGAS_EnemyCharacter::AGAS_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

UAbilitySystemComponent* AGAS_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGAS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetAbilitySystemComponent()) return;
	
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	
	if (!HasAuthority()) return; //Only on server we give startup abilities

	GiveStartupAbilities();
}
