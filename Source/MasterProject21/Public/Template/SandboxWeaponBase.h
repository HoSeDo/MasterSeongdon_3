#pragma once

#include "CoreMinimal.h"
#include "Template/WeaponBase.h"
#include "SandboxWeaponBase.generated.h"

UCLASS()
class MASTERPROJECT21_API ASandboxWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	void BeginPlay();

	virtual void Fire() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SandboxFire();

	UFUNCTION(BlueprintCallable)
	void Reload();

protected:

	UFUNCTION(BlueprintCallable)
	bool CheckAmmo();

	UFUNCTION(BlueprintCallable)
	void LinetraceOneShot(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();
};
