// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS_Projectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS()
class GAS_TUTORIAL_API AGAS_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AGAS_Projectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|Projectile")
	void SpawnImpactEffects_BP();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Damage", meta = (ExposeOnSpawn, ClampMin = 0.0));
	float Damage{10.f};

private:
	
	UPROPERTY(VisibleAnywhere, Category = "GAS|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

};
