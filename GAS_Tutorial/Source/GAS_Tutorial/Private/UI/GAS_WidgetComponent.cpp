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
	{
		GAS_Character->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
		return;
	}
	
	InitializeAttributeDelegate();
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

void UGAS_WidgetComponent::InitializeAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}

void UGAS_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UGAS_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UGAS_AttributeSet>(AS);

	if (!IsASCInitialized()) return;

	InitializeAttributeDelegate();
}

void UGAS_WidgetComponent::BindToAttributeChanges()
{
	//TODO: Listen for changes to Gameplay Attributes and update our widgets accordingly
}
