// Copyright Daniel Göndös 2026


#include "GAS_Tutorial/Public/Characters/GAS_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/GAS_PlayerState.h"


AGAS_PlayerCharacter::AGAS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
}

UAbilitySystemComponent* AGAS_PlayerCharacter::GetAbilitySystemComponent() const
{
	const AGAS_PlayerState* GAS_PlayerState = Cast<AGAS_PlayerState>(GetPlayerState());
	if (!IsValid(GAS_PlayerState))	return nullptr;
	
	return GAS_PlayerState->GetAbilitySystemComponent();
}

void AGAS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!GetAbilitySystemComponent() || !HasAuthority()) return;
	
	//this runs on server
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	GiveStartupAbilities();
}

void AGAS_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (!GetAbilitySystemComponent()) return;
	
	//this runs on clients
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
}
