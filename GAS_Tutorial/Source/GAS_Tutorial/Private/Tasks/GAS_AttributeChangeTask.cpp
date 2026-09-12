// Copyright Daniel Göndös 2026


#include "Tasks/GAS_AttributeChangeTask.h"

#include "AbilitySystemComponent.h"

UGAS_AttributeChangeTask* UGAS_AttributeChangeTask::ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	UGAS_AttributeChangeTask* WaitForAttributeChangeTask = NewObject<UGAS_AttributeChangeTask>();
	WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangeTask->AttributeToListenFor = Attribute;
	
	if (!AbilitySystemComponent) //Göndi comment. this check should be as a first line, and no remove from root, because its not added anywhere
	{
		WaitForAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangeTask, &ThisClass::AttributeChanged);
	
	return WaitForAttributeChangeTask;
}

void UGAS_AttributeChangeTask::EndTask()
{
	//Göndi Comment: this Endtask function is not called i dont understand why do we need it.
	if (ASC.IsValid())
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UGAS_AttributeChangeTask::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
