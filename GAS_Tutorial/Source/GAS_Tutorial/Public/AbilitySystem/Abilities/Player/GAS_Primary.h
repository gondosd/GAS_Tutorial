// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GAS_GameplayAbility.h"
#include "GAS_Primary.generated.h"

UCLASS()
class GAS_TUTORIAL_API UGAS_Primary : public UGAS_GameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
	TArray<AActor*> HitBoxOverlapTest();
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
	void SendHitReactEventToActors(const TArray<AActor*>& Actors);
	
private:
	void DrawHitBoxOverlapDebugs(const TArray<FOverlapResult>& OverlapResults, const FVector& HitboxLocation) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Abilities")
	float HitBoxRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Abilities")
	float HitBoxForwardOffset = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Abilities")
	float HitBoxElevationOffset = 20.f;
	
	
	
};
