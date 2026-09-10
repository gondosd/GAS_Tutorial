// Copyright Daniel Göndös 2026


#include "AbilitySystem/Abilities/Enemy/GAS_HitReact.h"

void UGAS_HitReact::CacheHitDirectionVectors(AActor* Instigator)
{
	AvatarForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	
	ToInstigator = (Instigator->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation());
	ToInstigator.Normalize();
}
