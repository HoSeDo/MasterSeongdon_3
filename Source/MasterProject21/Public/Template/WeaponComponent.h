#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SandboxWeaponBase.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERPROJECT21_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 블루프린트에서 보거나 호출할 수 있게 설정
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	ASandboxWeaponBase* CurrentWeapon;

	// 무기 생성 및 장착 (함수 이름은 마음대로 정하셔도 됩니다)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SpawnAndEquipWeapon(TSubclassOf<ASandboxWeaponBase> WeaponClass);

	// 발사 명령
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TryFire();



		
};
