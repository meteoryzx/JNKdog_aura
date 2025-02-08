// Copyright JNKdog Personal


#include "Character/AuraEnemy.h"
 
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
//为网格设置碰撞响应
AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	//当一个敌人被创建时，它的网格会设置为阻挡与可见性通道的碰撞。这意味着任何与这个敌人碰撞的物体都会被阻挡在外。
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	//为当前对象（通常是角色或玩家状态）创建一个新的 AbilitySystemComponent，并将其初始化为默认值
	AbilitySystemComponent->SetIsReplicated(true);//将该组件标记为需要进行网络同步（replication）。
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);//AI控制的使用Mnimal

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");//构建属性集
}

void AAuraEnemy::HighlightAction()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightAction()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}
 