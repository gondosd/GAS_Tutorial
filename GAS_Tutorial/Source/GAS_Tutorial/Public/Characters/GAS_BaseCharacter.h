// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS_BaseCharacter.generated.h"

struct FOnAttributeChangeData;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class GAS_TUTORIAL_API AGAS_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGAS_BaseCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const { return nullptr; }

	FORCEINLINE bool IsAlive() const { return bAlive; }
	FORCEINLINE void SetAlive(bool bAliveStatus) { bAlive = bAliveStatus; }

	UFUNCTION(BlueprintCallable, Category = "GAS|Death")
	virtual void HandleRespawn();
	
protected:
	void GiveStartupAbilities();
	void InitializeAttributes();
	
	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();

public:
	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributeEffect;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
	bool bAlive = true;
};
