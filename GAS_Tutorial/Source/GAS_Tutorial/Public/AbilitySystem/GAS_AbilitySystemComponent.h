// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS_AbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_TUTORIAL_API UGAS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRep_ActivateAbilities() override;

private:
	void HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec);
};
