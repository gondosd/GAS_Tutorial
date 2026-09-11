// Copyright Daniel Göndös 2026


#include "UI/GAS_WidgetComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GAS_AbilitySystemComponent.h"
#include "AbilitySystem/GAS_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/GAS_BaseCharacter.h"
#include "UI/GAS_AttributeWidget.h"


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
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair); //for checking the owned widget object

		//loop over all children on the owned widget, and do the same for them. 
		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, Pair);
		});
	}
}

void UGAS_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	UGAS_AttributeWidget* AttributeWidget = Cast<UGAS_AttributeWidget>(WidgetObject);
	if (!AttributeWidget) return; //We only care about GAS_AttributeWidgets
	if (!AttributeWidget->MatchesAttribute(Pair)) return; //Only subscribe for matching attributes

	AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get()); //for initial values

	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(
		[this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
		{
			AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get()); //For changes during the game.
		});
	
	//Göndi comment: Its not in the tutorial but my issue is, that this is not following if the max value is being changed. maybe if i add this:
	/*AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
		[this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
		{
			AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get()); //For changes during the game.
		});*/
}
