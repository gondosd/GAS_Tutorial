// Copyright Daniel Göndös 2026


#include "AbilitySystem/Abilities/Enemy/GAS_SearchForTarget.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AbilityTasks/GAS_WaitGameplayEvent.h"
#include "Characters/GAS_EnemyCharacter.h"
#include "GameplayTags/GAS_Tags.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/GAS_BlueprintLibrary.h"

UGAS_SearchForTarget::UGAS_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGAS_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningEnemy = Cast<AGAS_EnemyCharacter>(GetAvatarActorFromActorInfo());
	check(OwningEnemy.IsValid());

	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	check(OwningAIController.IsValid());

	StartSearch();

	WaitGameplayEventTask = UGAS_WaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(), GASTags::Events::Enemy::EndAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UGAS_SearchForTarget::StartSearch()
{
	if (bDrawDebugs)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("USearchForTarget::StartSearch")));

	if (!OwningEnemy.IsValid()) return;


	const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
	SearchDelayTask->Activate();
}

void UGAS_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	if (OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched)
		StartSearch();
}

void UGAS_SearchForTarget::Search()
{
	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	if (!OwningEnemy.IsValid()) return;

	FClosesActorsWithTagResult ClosestActorResult = UGAS_BlueprintLibrary::FindClosestActorWithTag(
		GetAvatarActorFromActorInfo(), SearchOrigin, GASActorTags::Player, OwningEnemy->SearchRange);

	TargetBaseCharacter = Cast<AGAS_BaseCharacter>(ClosestActorResult.Actor);

	if (!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}

	if (TargetBaseCharacter->IsAlive())
	{
		MoveToTargetAndAttack();
	}
	else
	{
		StartSearch();
	}
}

void UGAS_SearchForTarget::MoveToTargetAndAttack()
{
	if (!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetBaseCharacter.IsValid()) return;
	if (!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}

	MoveToLocationOrActorTask = UAITask_MoveTo::AIMoveTo(OwningAIController.Get(), FVector(), TargetBaseCharacter.Get(), OwningEnemy->AcceptanceRadius);
	MoveToLocationOrActorTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToLocationOrActorTask->ConditionalPerformMove();
}

void UGAS_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}
	OwningEnemy->RotateToTarget_BP(TargetBaseCharacter.Get());

	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
	AttackDelayTask->Activate();
}

void UGAS_SearchForTarget::Attack()
{
	const FGameplayTag AttackTag = GASTags::GASAbilities::Enemy::Attack;
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}
