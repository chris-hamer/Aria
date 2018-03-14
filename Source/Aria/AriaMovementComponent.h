// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AriaMovementComponent.generated.h"

/**
*
*/
UCLASS()
class ARIA_API UAriaMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()


		virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	float GetSimulationTimeStep(float RemainingTime, int32 Iterations);
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	FHitResult DoGroundTrace();
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	FHitResult DoGroundSweep();
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	TArray<FHitResult> DoGroundSweepMulti();
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	FHitResult DoGroundTraceSweep();
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	void AdjustFloor();
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	FVector GetRampDelta(FVector Delta);
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	bool CanStepUp(const FHitResult& Hit);
	UFUNCTION(BlueprintCallable, Category = "Shards Character Utilities")
	bool StepUp(const FVector& Delta, const FHitResult &InHit);

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") float HalfHeight = 88.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") float Radius = 34.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") float MaxSlopeAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") float MaxStepHeight = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") float CurrentFloorDist = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") FHitResult GroundSweep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") bool DisableGroundSnapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings") bool OnTheGround;
	FVector FloorLocation;

	FHitResult GroundHitResult;
	FHitResult PrevGroundHitResult;

};
