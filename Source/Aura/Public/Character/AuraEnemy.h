// Copyright JNKdog Personal

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface//继承IEnemyInterface类，敌人类需要重写这个函数
{                                                                           //如果不重写这里的纯虚函数，则抽象类无法实例化
	GENERATED_BODY()
public:
	virtual void HighlightAction () override;//重写函数
	virtual void UnHighlightAction () override;
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
};
