// Copyright Daniel Göndös 2026


#include "Utils/GAS_BlueprintLibrary.h"

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
