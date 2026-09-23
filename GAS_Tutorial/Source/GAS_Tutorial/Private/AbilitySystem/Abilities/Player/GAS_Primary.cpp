// Copyright Daniel Göndös 2026


#include "AbilitySystem/Abilities/Player/GAS_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/GAS_Tags.h"

void UGAS_Primary::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGAS_Primary::SendHitReactEventToActors(const TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, GASTags::Events::Enemy::HitReact, Payload);
	}
}
