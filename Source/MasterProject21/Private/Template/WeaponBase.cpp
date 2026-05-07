#include "Template/WeaponBase.h"
#include "Components/ArrowComponent.h"

AWeaponBase::AWeaponBase()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(RootComponent);

	AmmoPerFire = 1;
	CurrentAmmo = 0;
	MaxAmmo = 12;
	RoF = 1.f;
	CanFire = true;
	Range = 1000.f;
	DamagePerHit = 100.f;
}

void AWeaponBase::Fire()
{
	CanFire = false;
	GetWorld()->GetTimerManager().SetTimer(TimerFireDelay, this, &AWeaponBase::HandleFireDelay, 1.f / RoF, false);
	return;
}

void AWeaponBase::HandleFireDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerFireDelay);
	CanFire = true;
}

