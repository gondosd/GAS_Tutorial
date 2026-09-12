// Copyright Daniel Göndös 2026


#include "GAS_Tutorial/Public/Characters/GAS_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AGAS_BaseCharacter::AGAS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//Tick and refresh bone transforms whether rendered or not - for bone uptades on a dedicated server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void AGAS_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, bAlive);
}

UAbilitySystemComponent* AGAS_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AGAS_BaseCharacter::GiveStartupAbilities()
{
	if (!GetAbilitySystemComponent()) return;
	
	for (const TSubclassOf<UGameplayAbility>& Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

void AGAS_BaseCharacter::InitializeAttributes()
{
	checkf(InitializeAttributeEffect, TEXT("InitializeAttributesEffect not set"));

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributeEffect, 1.f, ContextHandle);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AGAS_BaseCharacter::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	if (AttributeChangeData.NewValue < 0.f)
		HandleDeath();
}

void AGAS_BaseCharacter::HandleDeath()
{
	bAlive = false;
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,  FString ::Printf(TEXT("%s has died!"), *GetName()));
}

void AGAS_BaseCharacter::HandleRespawn()
{
	bAlive = true;
}
