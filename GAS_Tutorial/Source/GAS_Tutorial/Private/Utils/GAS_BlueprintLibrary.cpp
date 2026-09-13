// Copyright Daniel Göndös 2026


#include "Utils/GAS_BlueprintLibrary.h"

#include "Characters/GAS_BaseCharacter.h"
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
