// Copyright Epic Games, Inc. All Rights Reserved.

#include "MasterProject21Character.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/EngineTypes.h"  
#include "DrawDebugHelpers.h"    

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMasterProject21Character

AMasterProject21Character::AMasterProject21Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMasterProject21Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMasterProject21Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMasterProject21Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMasterProject21Character::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMasterProject21Character::OnShoot);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMasterProject21Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMasterProject21Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMasterProject21Character::OnShoot(const FInputActionValue& Value)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !GetMesh()) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * ShotRange);

	FHitResult TargetHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FVector TargetPoint;

	if (GetWorld()->LineTraceSingleByChannel(TargetHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		TargetPoint = TargetHit.ImpactPoint;
	}
	else
	{
		TargetPoint = TraceEnd;
	}

	// 2 : 발사위치 정하기
	// "LeftElbowSocket" 으로 팔꿈치에서 발사하도록 소켓 Add Socket 블루프린터 추가
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("LeftElbowSocket"));

	FVector FireDirBase = (TargetPoint - MuzzleLocation).GetSafeNormal();

	// --- 3단계: 샷건 발사
	for (int32 i = 0; i < PelletCount; i++)
	{
		// 계산된 방향에 탄퍼짐 적용
		FVector FireDir = FMath::VRandCone(FireDirBase, FMath::DegreesToRadians(SpreadAngle));
		FVector PelletEnd = MuzzleLocation + (FireDir * ShotRange);

		FHitResult PelletHit;
		if (GetWorld()->LineTraceSingleByChannel(PelletHit, MuzzleLocation, PelletEnd, ECC_Visibility, QueryParams))
		{
			// 팔꿈치에서 충격 지점까지 라인 그리기
			DrawDebugLine(GetWorld(), MuzzleLocation, PelletHit.ImpactPoint, FColor::Red, false, 2.0f, 0, 1.0f);

			UGameplayStatics::ApplyPointDamage(PelletHit.GetActor(), 10.f, FireDir, PelletHit, PC, this, nullptr);
		}
		else
		{
			// 아무것도 안 맞았을 때 사거리 끝까지 라인 그리기
			DrawDebugLine(GetWorld(), MuzzleLocation, PelletEnd, FColor::Red, false, 2.0f, 0, 1.0f);
		}
	}

	// 1. 목표 고점 높이기 (확 튀어 오를 지점)
	TargetRecoilPitch += RecoilKickAmount;

	// 2. 복구 지점(원점)도 조금씩 높이기 (반동이 쌓이는 효과)
	// RecoilKickAmount의 40%(1 - RecoveryRate)만큼은 바닥으로 안 내려가게 막음
	PermanentRecoilOffset += RecoilKickAmount * (1.0f - RecoveryRate);
}

void AMasterProject21Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 목표치(Target)를 복구 지점(PermanentOffset)으로 서서히 내림
	// 이제 0으로 가는 게 아니라, 쌓인 오프셋까지만 내려갑니다.
	TargetRecoilPitch = FMath::FInterpTo(TargetRecoilPitch, PermanentRecoilOffset, DeltaTime, RecoilReturnSpeed);

	// 2. 현재 반동값을 목표치로 추적 (SnapSpeed를 높여서 팍! 튀게 만듦)
	float PrevRecoil = CurrentRecoilPitch;
	CurrentRecoilPitch = FMath::FInterpTo(CurrentRecoilPitch, TargetRecoilPitch, DeltaTime, RecoilSnapSpeed);

	// 3. 차이만큼 적용
	AddControllerPitchInput(CurrentRecoilPitch - PrevRecoil);
}