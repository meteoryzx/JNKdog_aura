// Copyright JNKdog Personal


#include "AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;//在多人游戏中，复制指响应服务器上数据更新并将其发送到客户端。
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;//创建一个 FHitResult 变量 CursorHit，用于存储碰撞检测的结果。
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//调用 GetHitResultUnderCursor 函数，进行光标下的碰撞检测。参数 ECC_Visibility 表示使用可见性通道进行检测；
	//第二个参数 false 表示不需要进行复杂的遮挡检测；结果将存储在 CursorHit 中。
	if(!CursorHit.bBlockingHit) return;
	//检查 CursorHit 的 bBlockingHit 属性，如果为 false（即光标下没有发生碰撞），则直接返回，结束函数执行。
	//这意味着后续的逻辑只有在光标下有物体被检测到时才会执行。
	LastActor = ThisActor;//在头文件中创建了这两个指针，分别代表上一个指针指着的目标和这次的目标。
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());//将这次点击的结果储存到这次目标的指针中。
	/**
	 * 检测这个指针，有多种情况
	 *  A. LastActor为null，ThisActor为valid
	 *     -啥也不做
	 *  B. LastActor为null，ThisActor为valid
	 *     -高亮ThisActor
	 *  C. LastActor为valid，ThisActor为null
	 *     -取消高亮LastActor
	 *  D. 都为valid，但LastActor ！= ThisActor
	 *     -取消高亮LastActor，高亮ThisActor
	 *  E. 都为valid，且Last Actor = ThisActor
	 *     -啥也不做
	 */

	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			//Case B
			ThisActor->HighlightAction();
		}
		else
		{
			//Case A，啥也不做。
		}
	}
	else  //LastActor is valid
	{
		if(ThisActor == nullptr)//Case C
		{
			LastActor->UnHighlightAction();
		}
		else//Case D
		{
		    if(LastActor != ThisActor)
		    {
			    LastActor->UnHighlightAction();
		    	ThisActor->HighlightAction();
		    }
		    else
		    {
			    //Case E 啥也不做
		    }
		}
	}
	
} 

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);//检查AuraContext指针是否存在，若指针没有被设置，则停止执行。
	UEnhancedInputLocalPlayerSubsystem* Subsystem =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//从当前本地玩家获取 UEnhancedInputLocalPlayerSubsystem 子系统。这是一个用于增强输入处理的类，允许更复杂的输入管理和绑定。
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);

	bShowMouseCursor = true;//显示鼠标光标
	DefaultMouseCursor = EMouseCursor::Default;//设置默认鼠标光标类型

	FInputModeGameAndUI InputModedata;//创建一个输入模式，允许同时处理游戏输入和用户界面输入。
	InputModedata.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);//设置鼠标不被锁定到视口中，这在处理用户界面时很有用。
	InputModedata.SetHideCursorDuringCapture(false);//在输入捕获期间显示鼠标光标。
	SetInputMode(InputModedata);// 应用这个输入模式，使游戏能够同时响应游戏和 UI 输入。
	
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();//通过 Super::SetupInputComponent()，可以确保父类中的输入绑定逻辑不会被忽略或覆盖。

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)//此函数获取玩家的 2D 移动输入，以及根据当前控制器的旋转计算出玩家移动的方向。
{//此处的FVector2D用于储存两个浮点数。
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();//创建一个名为InputAxisVector的常量，获取玩家输入的移动方向。
    const FRotator Rotation = GetControlRotation();//创建一个名为 Rotation 的 FRotator 类型常量，用于存储控制器的旋转信息。
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);//获取玩家视角方向，根据玩家的视角设置移动方向。

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//通过角色的旋转角度计算出角色的前进方向（X轴方向的单位向量），用它作为角色当前前进方向。
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//同上，获得正确的轴向量。

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{//通过 GetPawn() 函数获取当前控制的 Pawn，并将其赋值给 ControlledPawn 指针。
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		//将某个方向上的输入传递给引擎，促使角色朝这个方向移动。
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

}


