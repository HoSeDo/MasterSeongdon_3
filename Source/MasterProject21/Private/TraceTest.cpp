#include "TraceTest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ATraceTest::ATraceTest()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void ATraceTest::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATraceTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartAsyncTrace();

}


void ATraceTest::StartAsyncTrace()

{

	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &ATraceTest::OnAsyncTraceCompleted);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.WorldDynamic = ECR_Block;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Multi,
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		ECC_Visibility,
		QueryParams,
		ResponseParams,
		&TraceDelegate

	);
}

void ATraceTest::OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	for (const FHitResult& Hit : Data.OutHits)
	{
		AActor* HitActor = Hit.GetActor();

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Multi Hit Actor : %s"), *HitActor->GetName()));


		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Green, false, 2.f);
	}


}