// Copyright Daniel Göndös 2026


#include "UI/GAS_WidgetComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAS_AbilitySystemComponent.h"
#include "AbilitySystem/GAS_AttributeSet.h"
#include "Characters/GAS_BaseCharacter.h"


void UGAS_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystemData();

	if (!IsASCInitialized())
		GAS_Character->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
}

void UGAS_WidgetComponent::InitAbilitySystemData()
{
	GAS_Character = Cast<AGAS_BaseCharacter>(GetOwner());
	AbilitySystemComponent = Cast<UGAS_AbilitySystemComponent>(GAS_Character->GetAbilitySystemComponent());
	AttributeSet = Cast<UGAS_AttributeSet>(GAS_Character->GetAttributeSet());
}

bool UGAS_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UGAS_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UGAS_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UGAS_AttributeSet>(AS);
	
	//TODO: check if the Attribute set has been initialized with the first GE 
	//If not, bind to some delegate that will be broadcast when it is initialized
}
