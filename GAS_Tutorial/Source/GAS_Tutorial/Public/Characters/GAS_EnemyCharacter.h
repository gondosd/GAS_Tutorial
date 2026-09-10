// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS_BaseCharacter.h"
#include "GAS_EnemyCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class GAS_TUTORIAL_API AGAS_EnemyCharacter : public AGAS_BaseCharacter
{
	GENERATED_BODY()

public:
	AGAS_EnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

};
