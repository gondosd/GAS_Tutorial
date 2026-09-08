// Copyright Daniel Göndös 2026


#include "GAS_Tutorial/Public/Characters/GAS_BaseCharacter.h"

AGAS_BaseCharacter::AGAS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//Tick and refresh bone transforms whether rendered or not - for bone uptades on a dedicated server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}
