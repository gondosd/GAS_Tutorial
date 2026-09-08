// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS_BaseCharacter.h"
#include "GAS_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract)
class GAS_TUTORIAL_API AGAS_PlayerCharacter : public AGAS_BaseCharacter
{
	GENERATED_BODY()

public:
	AGAS_PlayerCharacter();

	
	private:
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
};
