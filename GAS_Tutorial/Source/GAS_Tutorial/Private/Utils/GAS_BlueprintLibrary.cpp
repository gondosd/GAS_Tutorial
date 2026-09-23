// Copyright Daniel Göndös 2026


#include "Utils/GAS_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GAS_AttributeSet.h"
#include "Characters/GAS_BaseCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/GAS_Tags.h"
#include "Kismet/GameplayStatics.h"

EHitDirection UGAS_BlueprintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float Dot = FVector::DotProduct(TargetForward, ToInstigator);
	if (Dot < -0.5f)
		return EHitDirection::Back;

	if (Dot < 0.5f)
	{
		//Either Left or right
		const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);
		if (Cross.Z < 0.f)
			return EHitDirection::Left;
		return EHitDirection::Right;
	}

	return EHitDirection::Forward;
}

FName UGAS_BlueprintLibrary::GetHitDirectionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
	case EHitDirection::Left: return FName("Left");
	case EHitDirection::Right: return FName("Right");
	case EHitDirection::Forward: return FName("Forward");
	case EHitDirection::Back: return FName("Back");
	}
	return FName("None");
}

FClosesActorsWithTagResult UGAS_BlueprintLibrary::FindClosestActorWithTag(const UObject* WorldContextObject, const FVector& Origin, const FName& Tag)
{
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, Tag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (auto Actor : ActorsWithTag)
	{
		if (!Actor) continue;
		AGAS_BaseCharacter* BaseCharacter = Cast<AGAS_BaseCharacter>(Actor);
		if (!BaseCharacter || !BaseCharacter->IsAlive()) continue;

		const float Distance = FVector::Dist(BaseCharacter->GetActorLocation(), Origin);

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	FClosesActorsWithTagResult Result;
	Result.Actor = ClosestActor;
	Result.Distance = ClosestDistance;
	return Result;
}

void UGAS_BlueprintLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, FGameplayEventData& Payload,
                                                    const FGameplayTag& DataTag, float Damage, UObject* OptionalParticleSystem)
{
	AGAS_BaseCharacter* PlayerCharacter = Cast<AGAS_BaseCharacter>(Target);
	if (!PlayerCharacter || !PlayerCharacter->IsAlive()) return;

	UGAS_AttributeSet* AttributeSet = Cast<UGAS_AttributeSet>(PlayerCharacter->GetAttributeSet());
	if (!AttributeSet) return;

	const bool bLethal = (AttributeSet->GetHealth() - Damage) <= 0.0f;

	const FGameplayTag EventTag = bLethal ? GASTags::Events::Player::Death : GASTags::Events::Player::HitReact;

	Payload.OptionalObject = OptionalParticleSystem;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, EventTag, Payload);

	UAbilitySystemComponent* TargetASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!TargetASC)return;

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage);

	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

TArray<AActor*> UGAS_BlueprintLibrary::HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOffset,
                                                         bool bDrawDebugs)
{
	if (!AvatarActor) return TArray<AActor*>();

	//Ensure that the overlap test ignores the Avatar Actor
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitboxLocation = AvatarActor->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxElevationOffset);

	UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
	World->OverlapMultiByChannel(OverlapResults, HitboxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& Result : OverlapResults)
	{
		AGAS_BaseCharacter* BaseCharacter = Cast<AGAS_BaseCharacter>(Result.GetActor());
		if (!BaseCharacter) continue;
		if (!BaseCharacter->IsAlive()) continue;

		ActorsHit.AddUnique(BaseCharacter);
	}


	if (bDrawDebugs)
		DrawHitBoxOverlapDebugs(AvatarActor, OverlapResults, HitboxLocation, HitBoxRadius);

	return ActorsHit;
}

void UGAS_BlueprintLibrary::DrawHitBoxOverlapDebugs(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults,
                                                    const FVector& HitboxLocation, float HitBoxRadius)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	DrawDebugSphere(World, HitboxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (Result.GetActor())
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 100.f;
			DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
		}
	}
}
