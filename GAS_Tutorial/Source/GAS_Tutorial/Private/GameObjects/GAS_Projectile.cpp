// Copyright Daniel Göndös 2026


#include "GameObjects/GAS_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/GAS_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/GAS_Tags.h"
#include "Utils/GAS_BlueprintLibrary.h"

AGAS_Projectile::AGAS_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	bReplicates = true;
}

void AGAS_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AGAS_PlayerCharacter* PlayerCharacter = Cast<AGAS_PlayerCharacter>(OtherActor);
	if (!PlayerCharacter || !PlayerCharacter->IsAlive()) return;

	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!AbilitySystemComponent || !HasAuthority()) return;

	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = PlayerCharacter;
	UGAS_BlueprintLibrary::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, GASTags::SetByCaller::Projectile, Damage);

	SpawnImpactEffects_BP();
	Destroy();
}
