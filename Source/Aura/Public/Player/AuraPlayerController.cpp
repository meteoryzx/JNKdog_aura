// Copyright JNKdog Personal


#include "AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;//在多人游戏中，复制指响应服务器上数据更新并将其发送到客户端。
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
