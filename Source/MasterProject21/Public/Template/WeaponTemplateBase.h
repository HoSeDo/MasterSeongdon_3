#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponTemplateBase.generated.h"


UCLASS()
class MASTERPROJECT21_API AWeaponTemplateBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	virtual void Fire() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload();
	virtual void Reload_Implementation();

protected:

	UFUNCTION(BlueprintNativeEvent)
	bool CheckAmmo();
	virtual bool CheckAmmo_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void ProcessFiring();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEffects();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateAmmo();
	virtual void UpdateAmmo_Implementation();

};
