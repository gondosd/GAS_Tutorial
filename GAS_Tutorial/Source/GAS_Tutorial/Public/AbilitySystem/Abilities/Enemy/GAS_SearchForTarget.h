// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AbilitySystem/Abilities/GAS_GameplayAbility.h"
#include "GAS_SearchForTarget.generated.h"

class UAITask_MoveTo;
class AGAS_BaseCharacter;
class UAbilityTask_WaitDelay;
class UGAS_WaitGameplayEvent;
class AAIController;
class AGAS_EnemyCharacter;

UCLASS()
class GAS_TUTORIAL_API UGAS_SearchForTarget : public UGAS_GameplayAbility
{
	GENERATED_BODY()

public:
	UGAS_SearchForTarget();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void StartSearch();

public:
	TWeakObjectPtr<AGAS_EnemyCharacter> OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;
	TWeakObjectPtr<AGAS_BaseCharacter> TargetBaseCharacter;
	
private:
	UPROPERTY()
	TObjectPtr<UGAS_WaitGameplayEvent> WaitGameplayEventTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> SearchDelayTask;
	
	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToLocationOrActorTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;
	
	UFUNCTION()
	void EndAttackEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void Search();

	void MoveToTargetAndAttack();
	
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController);
	
	UFUNCTION()
	void Attack();
};
