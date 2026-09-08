// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS_BaseCharacter.generated.h"


UCLASS(Abstract)
class GAS_TUTORIAL_API AGAS_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGAS_BaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
