// Copyright Daniel Göndös 2026


#include "AbilitySystem/GAS_AbilitySystemComponent.h"

UGAS_AbilitySystemComponent::UGAS_AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UGAS_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UGAS_AbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

