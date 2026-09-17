// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAttackAniomNotifyState.generated.h"

UCLASS()
class GAS_TUTORIAL_API UMeleeAttackAniomNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp) const;

	static void SendEventsToActors(const USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Debugs", meta = (AllowPrivateAccess = true))
	bool bDrawDebugs = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Trace", meta = (AllowPrivateAccess = true))
	FName SocketName = FName("FX_Trail_01_R");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Trace", meta = (AllowPrivateAccess = true))
	float SocketExtensionOffset = 40.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Trace", meta = (AllowPrivateAccess = true))
	float SphereTraceRadius = 60.f;
};
