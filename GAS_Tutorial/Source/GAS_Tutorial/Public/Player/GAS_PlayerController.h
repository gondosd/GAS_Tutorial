// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAS_PlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Abstract)
class GAS_TUTORIAL_API AGAS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Input")
	TObjectPtr<UInputAction> LookAction;
	
	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
};
