#include "Template/WeaponTemplateBase.h"

void AWeaponTemplateBase::Fire()
{
	if (!CanFire) return;

	if (CheckAmmo())
	{
		PlayEffects();
		ProcessFiring();
		UpdateAmmo();
		Super::Fire();
		return;
	}

	Reload();
}

void AWeaponTemplateBase::Reload_Implementation()
{
	CurrentAmmo = MaxAmmo;
	HandleFireDelay();
}

bool AWeaponTemplateBase::CheckAmmo_Implementation()
{
	return AmmoPerFire <= CurrentAmmo;
}

void AWeaponTemplateBase::UpdateAmmo_Implementation()
{
	CurrentAmmo -= AmmoPerFire;
}