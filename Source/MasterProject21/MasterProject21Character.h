// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MasterProject21Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMasterProject21Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** 발사 시 사용할 입력 액션 (에디터에서 할당) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** 샷건 설정값 */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ShotRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 PelletCount = 10;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float SpreadAngle = 5.0f;

	/** 반동 설정값 */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float RecoilPitch = -1.0f; // 위로 튀는 정도

public:
	AMasterProject21Character();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	

protected:

	virtual void Tick(float DeltaTime) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void OnShoot(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
			
private:
	// --- 샷건 반동 강화 설정 ---
	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilKickAmount = -5.0f;    // 한 번 쏠 때 확 튀는 양 (더 크게!)

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilSnapSpeed = 30.0f;    // 총구가 위로 튀는 속도 (엄청 빠르게!)

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilReturnSpeed = 5.0f;   // 돌아오는 속도 (천천히)

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RecoveryRate = 0.6f;        // 0.6이면 60%만 복구되고 40%는 화면에 남음

	// --- 내부 계산 변수 ---
	float TargetRecoilPitch = 0.0f;   // 도달해야 할 고점
	float CurrentRecoilPitch = 0.0f;  // 현재 적용된 반동
	float PermanentRecoilOffset = 0.0f; // 복구된 후 머무를 지점 (계속 쌓임)

};

