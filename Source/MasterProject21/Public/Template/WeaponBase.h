#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class MASTERPROJECT21_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> FirePoint;

	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoPerFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponBase")
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponBase")
	int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerHit;


	UPROPERTY(BlueprintReadWrite)
	bool CanFire;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerFireDelay;

	UFUNCTION()
	void HandleFireDelay();
};
