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

TArray<AActor*> UGAS_Primary::HitBoxOverlapTest()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	//Ensure that the overlap test ignores the Avatar Actor
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitboxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxElevationOffset);

	GetWorld()->OverlapMultiByChannel(OverlapResults, HitboxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& Result : OverlapResults)
	{
		if (!Result.GetActor()) continue;

		ActorsHit.AddUnique(Result.GetActor());
	}


	if (bDrawDebugs)
		DrawHitBoxOverlapDebugs(OverlapResults, HitboxLocation);

	return ActorsHit;
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

void UGAS_Primary::DrawHitBoxOverlapDebugs(const TArray<FOverlapResult>& OverlapResults, const FVector& HitboxLocation) const
{
	DrawDebugSphere(GetWorld(), HitboxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (Result.GetActor())
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 100.f;
			DrawDebugSphere(GetWorld(), DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
		}
	}
}
