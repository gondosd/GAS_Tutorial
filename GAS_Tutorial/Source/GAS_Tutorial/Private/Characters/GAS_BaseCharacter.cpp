// Copyright Daniel Göndös 2026


#include "GAS_Tutorial/Public/Characters/GAS_BaseCharacter.h"

#include "AbilitySystemComponent.h"

AGAS_BaseCharacter::AGAS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//Tick and refresh bone transforms whether rendered or not - for bone uptades on a dedicated server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* AGAS_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AGAS_BaseCharacter::GiveStartupAbilities()
{
	if (!GetAbilitySystemComponent()) return;
	
	for (const TSubclassOf<UGameplayAbility>& Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}
