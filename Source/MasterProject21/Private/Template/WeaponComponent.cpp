#include "Template/WeaponComponent.h"
#include "GameFramework/Character.h"


void UWeaponComponent::SpawnAndEquipWeapon(TSubclassOf<ASandboxWeaponBase> WeaponClass)
{
	if (!WeaponClass || !GetWorld()) return;

	// 1. 기존 무기가 있다면 제거
	if (CurrentWeapon) CurrentWeapon->Destroy();

	// 2. 새로운 무기 스폰
	FActorSpawnParameters Params;
	Params.Owner = GetOwner(); // 캐릭터를 주인으로 설정
	Params.Instigator = Cast<APawn>(GetOwner());

	CurrentWeapon = GetWorld()->SpawnActor<ASandboxWeaponBase>(WeaponClass, Params);

	// 3. 캐릭터 손에 붙이기
	if (CurrentWeapon)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			// "hand_r_socket"은 스켈레탈 메시의 소켓 이름입니다. (미리 만들어둬야 함)
			CurrentWeapon->AttachToComponent(Character->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				FName("hand_r_socket"));
		}
	}
}

void UWeaponComponent::TryFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}
