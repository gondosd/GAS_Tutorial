// Copyright Daniel Göndös 2026


#include "AbilitySystem/GAS_AbilitySystemComponent.h"

#include "AbilitySystem/Abilities/GAS_GameplayAbility.h"
#include "GameplayTags/GAS_Tags.h"


void UGAS_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	HandleAutoActivatedAbility(AbilitySpec);
}

void UGAS_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	FScopedAbilityListLock ActiveScopeLock(*this); // it prevents us to access an ability when it has been removed
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//GiveAbilityAndActivateOnce(AbilitySpec); <- this would do the same, but once the ability is done, it will be removed
		HandleAutoActivatedAbility(AbilitySpec);
	}
}

void UGAS_AbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.Ability) return;

	for (const FGameplayTag Tag : AbilitySpec.Ability->GetAssetTags())
	{
		if (Tag.MatchesTagExact(GASTags::GASAbilities::ActivateOnGiven))
		{
			TryActivateAbility(AbilitySpec.Handle);
			return;
		}
	}
}
