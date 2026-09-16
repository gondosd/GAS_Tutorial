// Copyright Daniel Göndös 2026


#include "GameObjects/GAS_Projectile.h"

#include "AbilitySystemComponent.h"
#include "Characters/GAS_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);
	//TODO: use the damage variable for the amount of damage cause.
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	SpawnImpactEffects_BP();
	Destroy();
}
