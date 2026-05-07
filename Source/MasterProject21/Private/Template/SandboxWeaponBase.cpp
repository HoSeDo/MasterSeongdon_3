#include "Template/SandboxWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

void ASandboxWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}

void ASandboxWeaponBase::Fire()
{
	if (!CanFire)
	{
		UE_LOG(LogTemp, Warning, TEXT("아직 다음 발사까지 대기 중입니다!"));
		return;
	}

	if (!CheckAmmo())
	{
		UE_LOG(LogTemp, Warning, TEXT("탄약이 부족합니다!"));
		return;
	}

	SandboxFire();

	Super::Fire();
}

void ASandboxWeaponBase::Reload()
{
	CurrentAmmo = MaxAmmo;
	HandleFireDelay();
}

bool ASandboxWeaponBase::CheckAmmo()
{
	return AmmoPerFire <= CurrentAmmo;
}

void ASandboxWeaponBase::LinetraceOneShot(FVector Direction)
{
	//강제로 모든 변수를 0으로 초기화해라
	FHitResult Hit(ForceInit);

	FVector Start = FirePoint->GetComponentLocation();
	FVector End = Start + (Direction * Range);

	if (!UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start, End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{ this, GetOwner() },
		EDrawDebugTrace::ForDuration,
		Hit,
		true,
		FLinearColor::Red, FLinearColor::Green, 5))
		return;

	UGameplayStatics::ApplyDamage(Hit.GetActor(), DamagePerHit, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
}

void ASandboxWeaponBase::PlaySound(USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
}

void ASandboxWeaponBase::UpdateAmmo()
{
	CurrentAmmo -= AmmoPerFire;
}