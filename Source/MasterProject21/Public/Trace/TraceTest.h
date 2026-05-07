#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraceTest.generated.h"

UCLASS()
class MASTERPROJECT21_API ATraceTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATraceTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartAsyncTrace();

};
