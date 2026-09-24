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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();
	
	void StopMovementUntilLanded();

protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;

private:
	UFUNCTION()
	void EnableMovementOnLanded(const FHitResult& Hit);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "GAS|AI")
	float AcceptanceRadius{500.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "GAS|AI")
	float MinAttackDelay{.1f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "GAS|AI")
	float MaxAttackDelay{.5f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category= "GAS|AI")
	bool bIsBeingLaunched{false};

private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
