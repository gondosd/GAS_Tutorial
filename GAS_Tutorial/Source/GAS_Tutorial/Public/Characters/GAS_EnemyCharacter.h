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
	virtual UAttributeSet* GetAttributeSet() const override;
	
	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();

protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "GAS|AI")
	float AcceptanceRadius{500.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "GAS|AI")
	float MinAttackDelay{.1f};
	
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "GAS|AI")
	float MaxAttackDelay{.5f};

private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
