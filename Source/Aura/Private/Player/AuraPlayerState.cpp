// Copyright JNKdog Personal


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);//构建能力系统组件
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);//玩家操控的使用Mixed

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");//构建属性集
	NetUpdateFrequency = 100.f;//每秒和服务器更新频率，使用GAS后可以设置的高一些
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


