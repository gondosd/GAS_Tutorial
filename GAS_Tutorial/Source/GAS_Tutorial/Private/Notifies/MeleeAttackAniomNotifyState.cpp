// Copyright Daniel Göndös 2026


#include "Notifies/MeleeAttackAniomNotifyState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KismetTraceUtils.h"
#include "Characters/GAS_BaseCharacter.h"
#include "GameplayTags/GAS_Tags.h"
#include "Kismet/KismetMathLibrary.h"

void UMeleeAttackAniomNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;

	SendEventsToActors(MeshComp, PerformSphereTrace(MeshComp));
}

TArray<FHitResult> UMeleeAttackAniomNotifyState::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	TArray<struct FHitResult> OutHits;

	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector Start = SocketTransform.GetLocation();
	const FVector ExtendedSocketDirection = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator() * SocketExtensionOffset);
	const FVector End = Start - ExtendedSocketDirection;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return OutHits;

	bool const bHit = World->SweepMultiByChannel(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereTraceRadius),
		Params,
		ResponseParams);

	if (bDrawDebugs)
	{
		DrawDebugSphereTraceMulti(
			World,
			Start,
			End,
			SphereTraceRadius,
			EDrawDebugTrace::ForDuration,
			bHit,
			OutHits,
			FColor::Red,
			FColor::Green,
			5.f);
	}

	return OutHits;

	//This was my implementation after this comment

	// FVector Start;
	// FRotator SocketRotation;
	// MeshComp->GetSocketWorldLocationAndRotation(SocketName, Start, SocketRotation);
	// const FVector End = Start - (SocketRotation.Vector() * SocketExtensionOffset);
	//
	// TArray<FHitResult> OutHits;
	// TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(MeshComp->GetOwner());
	// UKismetSystemLibrary::SphereTraceMulti(MeshComp,
	//                                        Start,
	//                                        End,
	//                                        SphereTraceRadius,
	//                                        UEngineTypes::ConvertToTraceType(ECC_Visibility),
	//                                        false,
	//                                        ActorsToIgnore,
	//                                        bDrawDebugs ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
	//                                        OutHits,
	//                                        true,
	//                                        FLinearColor::Red,
	//                                        FLinearColor::Green,
	//                                        5.f);
	//
	// return OutHits;
}

void UMeleeAttackAniomNotifyState::SendEventsToActors(const USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits)
{
	for (const FHitResult& Hit : Hits)
	{
		FGameplayEffectContextHandle EffectContext;

		if (const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
		{
			const UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
			if (!ASC) continue;
			EffectContext = ASC->MakeEffectContext();
		}

		EffectContext.AddHitResult(Hit);

		AGAS_BaseCharacter* PlayerCharacter = Cast<AGAS_BaseCharacter>(Hit.GetActor());

		if (!PlayerCharacter || !PlayerCharacter->IsAlive()) continue;

		FGameplayEventData Payload;
		Payload.Target = PlayerCharacter;
		Payload.ContextHandle = EffectContext;
		Payload.Instigator = MeshComp->GetOwner();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), GASTags::Events::Enemy::MeleeTraceHit, Payload);
	}
}
