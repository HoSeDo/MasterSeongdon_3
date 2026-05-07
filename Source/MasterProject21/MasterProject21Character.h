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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ShotRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 PelletCount = 10;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float SpreadAngle = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float RecoilPitch = -1.0f; 

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


	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
			
private:

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilKickAmount = -5.0f;   

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilSnapSpeed = 30.0f;    

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilReturnSpeed = 5.0f;   

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RecoveryRate = 0.6f;        

	// --- 내부 계산 변수 ---
	float TargetRecoilPitch = 0.0f;  
	float CurrentRecoilPitch = 0.0f;  
	float PermanentRecoilOffset = 0.0f;

};

