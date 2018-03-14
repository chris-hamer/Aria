// Fill out your copyright notice in the Description page of Project Settings.

#include "AriaMovementComponent.h"
#include "Engine.h"

void UAriaMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

FHitResult UAriaMovementComponent::DoGroundSweep()
{
	float TraceLength = (OnTheGround ? 1000.0f : 100.0f);
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = UpdatedComponent->GetComponentLocation() - TraceLength * FVector::UpVector;
	float boxheight = HalfHeight*0.25f;
	FCollisionShape GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.707f, Radius * 0.707f, boxheight));
	FCollisionQueryParams GroundTraceQuery;
	FCollisionResponseParams GroundTraceResponse;
	FHitResult GroundResult;
	GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	if (GroundResult.bStartPenetrating) {
		GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.407f, Radius * 0.407f, boxheight));
		GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	}
	FHitResult FirstResult = GroundResult;
	GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), 0), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	if (GroundResult.bStartPenetrating) {
		GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.407f, Radius * 0.407f, boxheight));
		GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), 0), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	}
	//if (FMath::Abs((CapsuleBottom - GroundResult.ImpactPoint).Z) > FMath::Abs((CapsuleBottom - FirstResult.ImpactPoint).Z)) {
	if (GroundResult.ImpactPoint.Z > FirstResult.ImpactPoint.Z) {
		GroundResult = FirstResult;
	}
	//End = GroundSweep.ImpactPoint - TraceLength * FVector::UpVector;
	//GetWorld()->LineTraceSingleByChannel(GroundSweep, Start, End, ECollisionChannel::ECC_Visibility, GroundTraceQuery, GroundTraceResponse);
	return GroundResult;
}

FHitResult UAriaMovementComponent::DoGroundTrace()
{
	float TraceLength = (OnTheGround ? 1000.0f : 100.0f);
	FVector Start = UpdatedComponent->GetComponentLocation();
	////FVector End = GroundSweep.ImpactPoint - TraceLength * FVector::UpVector;
	FVector End = UpdatedComponent->GetComponentLocation() - TraceLength * FVector::UpVector;
	FCollisionQueryParams GroundTraceQuery;
	FCollisionResponseParams GroundTraceResponse;
	FHitResult GroundResult;
	GetWorld()->LineTraceSingleByChannel(GroundResult, Start, End, ECollisionChannel::ECC_Visibility, GroundTraceQuery, GroundTraceResponse);
	//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FString::SanitizeFloat(GroundResult.bBlockingHit));
	return GroundResult;
}

FHitResult UAriaMovementComponent::DoGroundTraceSweep()
{
	float TraceLength = (OnTheGround ? 1000.0f : 100.0f);
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = UpdatedComponent->GetComponentLocation() - TraceLength * FVector::UpVector;
	FVector CapsuleBottom = Start - HalfHeight * FVector::UpVector;
	FCollisionShape GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.707f, Radius * 0.707f, HalfHeight));
	FCollisionQueryParams GroundTraceQuery;
	FCollisionResponseParams GroundTraceResponse;
	FHitResult GroundResult;
	GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	if (GroundResult.bStartPenetrating) {
		GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.407f, Radius * 0.407f, HalfHeight));
		GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	}
	FHitResult FirstResult = GroundResult;
	GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), 0), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	if (GroundResult.bStartPenetrating) {
		GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.407f, Radius * 0.407f, HalfHeight));
		GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), 0), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	}
	//if (FMath::Abs((CapsuleBottom - GroundResult.ImpactPoint).Z) > FMath::Abs((CapsuleBottom - FirstResult.ImpactPoint).Z)) {
	if (GroundResult.ImpactPoint.Z > FirstResult.ImpactPoint.Z) {
		GroundResult = FirstResult;
	}
	Start = GroundResult.ImpactPoint + MaxStepHeight * FVector::UpVector;
	End = GroundResult.ImpactPoint - MaxStepHeight * FVector::UpVector;
	//GetWorld()->LineTraceSingleByChannel(GroundResult, Start, End, ECollisionChannel::ECC_Visibility, GroundTraceQuery, GroundTraceResponse);
	return GroundResult;
}

float UAriaMovementComponent::GetSimulationTimeStep(float RemainingTime, int32 Iterations)
{
	if (RemainingTime > 0.05f)
	{
		if (Iterations < 8)
		{
			// Subdivide moves to be no longer than MaxSimulationTimeStep seconds
			RemainingTime = FMath::Min(0.05f, RemainingTime * 0.5f);
		}
		else {

		}
	}

	// no less than MIN_TICK_TIME (to avoid potential divide-by-zero during simulation).
	return FMath::Max(1e-6f, RemainingTime);
}

bool UAriaMovementComponent::CanStepUp(const FHitResult& Hit)
{
	if (!Hit.IsValidBlockingHit())
	{
		return false;
	}

	//if (!Hit.IsValidBlockingHit() || !HasValidData() || MovementMode == MOVE_Falling)
	//{
	//	return false;
	//}

	//// No component for "fake" hits when we are on a known good base.
	//const UPrimitiveComponent* HitComponent = Hit.Component.Get();
	//if (!HitComponent)
	//{
	//	return true;
	//}

	//if (!HitComponent->CanCharacterStepUp(CharacterOwner))
	//{
	//	return false;
	//}

	//// No actor for "fake" hits when we are on a known good base.
	//const AActor* HitActor = Hit.GetActor();
	//if (!HitActor)
	//{
	//	return true;
	//}

	//if (!HitActor->CanBeBaseForCharacter(CharacterOwner))
	//{
	//	return false;
	//}

	return true;
}

bool UAriaMovementComponent::StepUp(const FVector& Delta, const FHitResult &InHit)
{
	return false;
	FVector GravDir = FVector(0, 0, -1);

	if (!CanStepUp(InHit) || MaxStepHeight <= 0.f)
	{
		return false;
	}

	const FVector OldLocation = UpdatedComponent->GetComponentLocation();

	// Don't bother stepping up if top of capsule is hitting something.
	const float InitialImpactZ = InHit.ImpactPoint.Z;
	if (InitialImpactZ > OldLocation.Z + (HalfHeight - Radius))
	{
		return false;
	}

	if (GravDir.IsZero())
	{
		return false;
	}

	float StepTravelUpHeight = MaxStepHeight;
	float StepTravelDownHeight = StepTravelUpHeight;
	const float StepSideZ = -1.f * FVector::DotProduct(InHit.ImpactNormal, GravDir);
	float PawnInitialFloorBaseZ = OldLocation.Z - HalfHeight;
	float PawnFloorPointZ = PawnInitialFloorBaseZ;

	// Since we float a vAriable amount off the floor, we need to enforce max step height off the actual point of impact with the floor.
	const float FloorDist = FMath::Max(0.f, CurrentFloorDist);
	PawnInitialFloorBaseZ -= FloorDist;
	StepTravelUpHeight = FMath::Max(StepTravelUpHeight - FloorDist, 0.f);
	StepTravelDownHeight = (MaxStepHeight + 2.4f*2.f);

	//const bool bHitVerticalFace = !IsWithinEdgeTolerance(InHit.Location, InHit.ImpactPoint, Radius);
	PawnFloorPointZ = GroundSweep.ImpactPoint.Z;

	// Don't step up if the impact is below us, accounting for distance from floor.
	if (InitialImpactZ <= PawnInitialFloorBaseZ)
	{
		return false;
	}

	// Scope our movement updates, and do not apply them until all intermediate moves are completed.
	FScopedMovementUpdate ScopedStepUpMovement(UpdatedComponent, EScopedUpdate::DeferredUpdates);

	// step up - treat as vertical wall
	FHitResult SweepUpHit(1.f);
	const FQuat PawnRotation = UpdatedComponent->GetComponentQuat();
	MoveUpdatedComponent(-GravDir * StepTravelUpHeight, PawnRotation, true, &SweepUpHit);

	if (SweepUpHit.bStartPenetrating)
	{
		// Undo movement
		ScopedStepUpMovement.RevertMove();
		return false;
	}

	// step fwd
	FHitResult Hit(1.f);
	MoveUpdatedComponent(Delta, PawnRotation, true, &Hit);

	// Check result of forward movement
	if (Hit.bBlockingHit)
	{
		if (Hit.bStartPenetrating)
		{
			// Undo movement
			ScopedStepUpMovement.RevertMove();
			return false;
		}

		// If we hit something above us and also something ahead of us, we should notify about the upward hit as well.
		// The forward hit will be handled later (in the bSteppedOver case below).
		// In the case of hitting something above but not forward, we are not blocked from moving so we don't need the notification.
		if (SweepUpHit.bBlockingHit && Hit.bBlockingHit)
		{
			HandleImpact(SweepUpHit);
		}

		// pawn ran into a wall
		HandleImpact(Hit);
		if (IsFalling())
		{
			return true;
		}

		// adjust and try again
		const float ForwardHitTime = Hit.Time;
		const float ForwardSlideAmount = SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);

		if (IsFalling())
		{
			ScopedStepUpMovement.RevertMove();
			return false;
		}

		// If both the forward hit and the deflection got us nowhere, there is no point in this step up.
		if (ForwardHitTime == 0.f && ForwardSlideAmount == 0.f)
		{
			ScopedStepUpMovement.RevertMove();
			return false;
		}
	}

	// Step down
	MoveUpdatedComponent(GravDir * StepTravelDownHeight, UpdatedComponent->GetComponentQuat(), true, &Hit);

	// If step down was initially penetrating abort the step up
	if (Hit.bStartPenetrating)
	{
		ScopedStepUpMovement.RevertMove();
		return false;
	}

	if (Hit.IsValidBlockingHit())
	{
		// See if this step sequence would have allowed us to travel higher than our max step height allows.
		const float DeltaZ = Hit.ImpactPoint.Z - PawnFloorPointZ;
		if (DeltaZ > MaxStepHeight)
		{
			//UE_LOG(LogCharacterMovement, VeryVerbose, TEXT("- Reject StepUp (too high Height %.3f) up from floor base %f to %f"), DeltaZ, PawnInitialFloorBaseZ, NewLocation.Z);
			ScopedStepUpMovement.RevertMove();
			return false;
		}

		// Reject unwalkable surface normals here.
		//if (!IsWalkable(Hit))
		if (false)
		{
			// Reject if normal opposes movement direction
			const bool bNormalTowardsMe = (Delta | Hit.ImpactNormal) < 0.f;
			if (bNormalTowardsMe)
			{
				//UE_LOG(LogCharacterMovement, VeryVerbose, TEXT("- Reject StepUp (unwalkable normal %s opposed to movement)"), *Hit.ImpactNormal.ToString());
				ScopedStepUpMovement.RevertMove();
				return false;
			}

			// Also reject if we would end up being higher than our starting location by stepping down.
			// It's fine to step down onto an unwalkable normal below us, we will just slide off. Rejecting those moves would prevent us from being able to walk off the edge.
			if (Hit.Location.Z > OldLocation.Z)
			{
				//UE_LOG(LogCharacterMovement, VeryVerbose, TEXT("- Reject StepUp (unwalkable normal %s above old position)"), *Hit.ImpactNormal.ToString());
				ScopedStepUpMovement.RevertMove();
				return false;
			}
		}

		// Reject moves where the downward sweep hit something very close to the edge of the capsule. This maintains consistency with FindFloor as well.
		//if (!IsWithinEdgeTolerance(Hit.Location, Hit.ImpactPoint, Radius))
		//{
		//	//UE_LOG(LogCharacterMovement, VeryVerbose, TEXT("- Reject StepUp (outside edge tolerance)"));
		//	ScopedStepUpMovement.RevertMove();
		//	return false;
		//}

		// Don't step up onto invalid surfaces if traveling higher.
		if (DeltaZ > 0.f && !CanStepUp(Hit))
		{
			//UE_LOG(LogCharacterMovement, VeryVerbose, TEXT("- Reject StepUp (up onto surface with !CanStepUp())"));
			ScopedStepUpMovement.RevertMove();
			return false;
		}

		// See if we can validate the floor as a result of this step down. In almost all cases this should succeed, and we can avoid computing the floor outside this method.
		//if (OutStepDownResult != NULL)
		//{
		//	FindFloor(UpdatedComponent->GetComponentLocation(), StepDownResult.FloorResult, false, &Hit);

		//	// Reject unwalkable normals if we end up higher than our initial height.
		//	// It's fine to walk down onto an unwalkable surface, don't reject those moves.
		//	if (Hit.Location.Z > OldLocation.Z)
		//	{
		//		// We should reject the floor result if we are trying to step up an actual step where we are not able to perch (this is rare).
		//		// In those cases we should instead abort the step up and try to slide along the stair.
		//		if (!StepDownResult.FloorResult.bBlockingHit && StepSideZ < MAX_STEP_SIDE_Z)
		//		{
		//			ScopedStepUpMovement.RevertMove();
		//			return false;
		//		}
		//	}

		//	StepDownResult.bComputedFloor = true;
		//}
	}

	return true;
}

void UAriaMovementComponent::AdjustFloor()
{
	if (!GroundSweep.IsValidBlockingHit()) {
		return;
	}
	float Dist = (UpdatedComponent->GetComponentLocation() - HalfHeight * FVector::UpVector - GroundSweep.ImpactPoint).Z - 5.0f;
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector(0, 0, -Dist), FQuat::Identity, true, Hit);
	if (Hit.ImpactNormal.Z < 0.707f) {
		Hit.ImpactNormal = FVector::VectorPlaneProject(Hit.ImpactNormal, FVector::UpVector).GetSafeNormal();
		//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FVector(0, 0, -Dist).ToString() + "     000       " + FString::SanitizeFloat(Hit.ImpactNormal.Z));
	}
	//SafeMoveUpdatedComponent(FVector(0, 0, 5.0f), FQuat::Identity, true, Hit);
	if (Hit.ImpactNormal.Z < 0.707f) {
		Hit.ImpactNormal = FVector::VectorPlaneProject(Hit.ImpactNormal, FVector::UpVector).GetSafeNormal();
		//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FVector(0, 0, 5.0f).ToString() + "     000       " + FString::SanitizeFloat(Hit.ImpactNormal.Z));
	}
	CurrentFloorDist = (UpdatedComponent->GetComponentLocation() - HalfHeight * FVector::UpVector - GroundSweep.ImpactPoint).Z - 5.0f;
}

FVector UAriaMovementComponent::GetRampDelta(FVector Delta)
{
	//FHitResult help = DoGroundTrace();
	if (!OnTheGround) {
		return Delta;
	}
	FVector GroundNormal = GroundSweep.ImpactNormal;
	if (GroundNormal.Z < (1.f - KINDA_SMALL_NUMBER) && GroundNormal.Z > KINDA_SMALL_NUMBER) {
		const float FloorDotDelta = (GroundNormal | Delta);
		FVector RampMovement(Delta.X, Delta.Y, -FloorDotDelta / GroundNormal.Z);

		return RampMovement;
	}

	return Delta;
}

TArray<FHitResult> UAriaMovementComponent::DoGroundSweepMulti()
{
	float TraceLength = (OnTheGround ? 1000.0f : 100.0f);
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = UpdatedComponent->GetComponentLocation() - TraceLength * FVector::UpVector;
	float boxheight = HalfHeight*1.0f;
	boxheight = 1.0f;
	FCollisionShape GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.707f, Radius * 0.707f, boxheight));
	FCollisionQueryParams GroundTraceQuery;
	GroundTraceQuery.bFindInitialOverlaps = true;
	FCollisionResponseParams GroundTraceResponse;
	FHitResult GroundResult;
	TArray<FHitResult> Results;
	TArray<FHitResult> Results2;
	GetWorld()->SweepMultiByChannel(Results, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	Results2.Append(Results);
	if (Results.Num() > 0 && Results.Last().Actor != NULL) {
		GroundTraceQuery.AddIgnoredActor(Results.Last().Actor.Get());
		if (Results.Last().ImpactNormal.Z > 0.707f) {
			return Results2;
		}
	}
	GetWorld()->SweepMultiByChannel(Results, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	Results2.Append(Results);
	//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FString::SanitizeFloat(Results.Num()));
	GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	if (GroundResult.bStartPenetrating) {
		GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.407f, Radius * 0.407f, boxheight));
		GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), PI * 0.25f), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	}
	FHitResult FirstResult = GroundResult;
	GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), 0), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	if (GroundResult.bStartPenetrating) {
		GroundTraceCube = FCollisionShape::MakeBox(FVector(Radius * 0.407f, Radius * 0.407f, boxheight));
		GetWorld()->SweepSingleByChannel(GroundResult, Start, End, FQuat(FVector(0.f, 0.f, -1.f), 0), ECollisionChannel::ECC_Visibility, GroundTraceCube, GroundTraceQuery, GroundTraceResponse);
	}
	//if (FMath::Abs((CapsuleBottom - GroundResult.ImpactPoint).Z) > FMath::Abs((CapsuleBottom - FirstResult.ImpactPoint).Z)) {
	if (GroundResult.ImpactPoint.Z > FirstResult.ImpactPoint.Z) {
		GroundResult = FirstResult;
	}
	//End = GroundSweep.ImpactPoint - TraceLength * FVector::UpVector;
	//GetWorld()->LineTraceSingleByChannel(GroundSweep, Start, End, ECollisionChannel::ECC_Visibility, GroundTraceQuery, GroundTraceResponse);
	return Results2;
}

void UAriaMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OnTheGround) {
		//GroundSweep = DoGroundTraceSweep();
		//AdjustFloor();
	}

	FVector Accel = ConsumeInputVector();
	FVector Input = Velocity + Accel*DeltaTime;

	TArray<FHitResult> GroundSweep2 = DoGroundSweepMulti();
	for (int i = 0; i < GroundSweep2.Num(); i++)
	{
		FVector f1 = FVector::VectorPlaneProject(GroundSweep2[i].ImpactNormal, FVector::UpVector).GetSafeNormal();
		//FVector dx = FVector::VectorPlaneProject((GroundSweep2[i].ImpactPoint - UpdatedComponent->GetComponentLocation()).GetSafeNormal(), FVector::UpVector).GetSafeNormal();
		if (OnTheGround &&
			GroundSweep2[i].ImpactNormal.Z < FMath::Cos(MaxSlopeAngle*(PI / 180)) &&
			FVector::DotProduct(Input.GetSafeNormal(), f1) < -0.1f) {
			Input = FVector::VectorPlaneProject(Input, f1);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FString::SanitizeFloat(GroundSweep2[i].ImpactNormal.Z) + "           " + GroundSweep2[i].Actor->GetName() + "        " + (GroundSweep2[i].ImpactPoint - UpdatedComponent->GetComponentLocation()).GetSafeNormal().ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, GroundSweep2[i].Actor->GetName() + "           " +(GroundSweep2[i].ImpactPoint).ToString()+"            "+ UpdatedComponent->GetComponentLocation().ToString());
	}

	//GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FString::SanitizeFloat(GroundSweep.ImpactNormal.Z));

	FVector OriginalLocation = UpdatedComponent->GetComponentLocation();

	FHitResult MoveHit;

	float remainingTime = DeltaTime;
	int Iterations = 0;
	while ((remainingTime >= 1e-6f) && (Iterations < 8)) {
		Iterations++;
		float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		FVector Delta = timeTick * Input;
		FHitResult Hit(1.f);
		remainingTime -= timeTick;
		FVector RampVector = GetRampDelta(Delta);

		////GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, FString::SanitizeFloat(Delta.Size()));

		SafeMoveUpdatedComponent(RampVector, FQuat::Identity, true, MoveHit);
		//if (MoveHit.ImpactNormal.Z < 0.707f) {
		//	MoveHit.ImpactNormal = FVector::VectorPlaneProject(MoveHit.ImpactNormal, FVector::UpVector).GetSafeNormal();
		//	GEngine->AddOnScreenDebugMessage(-1, 234, FColor::Cyan, RampVector.ToString() + "     000       " + FString::SanitizeFloat(MoveHit.ImpactNormal.Z));
		//}

		float LastMoveTimeSlice = timeTick;

		if (MoveHit.bStartPenetrating) {
			HandleImpact(MoveHit);
			SlideAlongSurface(Delta, 1.f, MoveHit.Normal, MoveHit, true);
		}
		else if (MoveHit.IsValidBlockingHit()) {
			float PercentTimeApplied = MoveHit.Time;

			if ((MoveHit.Time > 0.f) && (MoveHit.Normal.Z > KINDA_SMALL_NUMBER)) {
				// Another walkable ramp.
				float InitialPercentRemaining = 1.f - PercentTimeApplied;
				RampVector = GetRampDelta(Delta * InitialPercentRemaining);
				LastMoveTimeSlice = InitialPercentRemaining * LastMoveTimeSlice;
				SafeMoveUpdatedComponent(RampVector, UpdatedComponent->GetComponentQuat(), true, MoveHit);

				const float SecondHitPercent = MoveHit.Time * InitialPercentRemaining;
				PercentTimeApplied = FMath::Clamp(PercentTimeApplied + SecondHitPercent, 0.f, 1.f);

				HandleImpact(MoveHit, LastMoveTimeSlice, RampVector);
				SlideAlongSurface(Delta, 1.f - PercentTimeApplied, MoveHit.Normal, MoveHit, true);
			}

			if (MoveHit.IsValidBlockingHit()) {
				// Normal things
				if (OnTheGround) {
					StepUp(Delta * (1.f - PercentTimeApplied), Hit);
				}
				HandleImpact(MoveHit, LastMoveTimeSlice, RampVector);
				SlideAlongSurface(Delta, 1.f - PercentTimeApplied, MoveHit.Normal, MoveHit, true);
			}
		}
	}

	Velocity = (UpdatedComponent->GetComponentLocation() - OriginalLocation) / DeltaTime;
}