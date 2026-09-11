// Copyright Daniel Göndös 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS_WidgetComponent.generated.h"


class UGAS_AttributeSet;
class UGAS_AbilitySystemComponent;
class AGAS_BaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_TUTORIAL_API UGAS_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;

private:
	TWeakObjectPtr<AGAS_BaseCharacter> GAS_Character;
	TWeakObjectPtr<UGAS_AttributeSet> AttributeSet;
	TWeakObjectPtr<UGAS_AbilitySystemComponent> AbilitySystemComponent;

	void InitAbilitySystemData();
	bool IsASCInitialized() const;

	void InitializeAttributeDelegate();
	void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;
	
	UFUNCTION()
	void OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);
	

	UFUNCTION()
	void BindToAttributeChanges();
	
};
