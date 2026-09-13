// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "Characters/GAS_BaseCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GAS_BlueprintLibrary.generated.h"

UENUM(Blueprinttype)
enum class EHitDirection : uint8
{
	Left,
	Right,
	Forward,
	Back,
 };

USTRUCT(BlueprintType)
struct FClosesActorsWithTagResult
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;
	
	UPROPERTY(BlueprintReadWrite)
	float Distance{0.f};
};


UCLASS()
class GAS_TUTORIAL_API UGAS_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure)
	static EHitDirection GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator);
	
	UFUNCTION(BlueprintPure)
	static FName GetHitDirectionName(const EHitDirection& HitDirection);
	
	UFUNCTION(BlueprintCallable)
	static FClosesActorsWithTagResult FindClosestActorWithTag(const UObject* WorldContextObject, const FVector& Origin, const FName& Tag = FName("Player"));
};
