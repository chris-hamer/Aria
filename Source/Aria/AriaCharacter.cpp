// Fill out your copyright notice in the Description page of Project Settings.

#include "AriaCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"

// Sets default values
AAriaCharacter::AAriaCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	//CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetEnableGravity(false);
	CapsuleComponent->SetLinearDamping(0.0f);
	CapsuleComponent->BodyInstance.bLockRotation = true;
	CapsuleComponent->BodyInstance.bLockXRotation = true;
	CapsuleComponent->BodyInstance.bLockYRotation = true;
	CapsuleComponent->BodyInstance.bLockZRotation = true;
	CapsuleComponent->BodyInstance.PositionSolverIterationCount = 16;
	CapsuleComponent->BodyInstance.VelocitySolverIterationCount = 16;
	CapsuleComponent->bShouldUpdatePhysicsVolume = true;
	CapsuleComponent->SetupAttachment(RootComponent);
	SetActorEnableCollision(true);

	Model = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Model"));
	Model->SetupAttachment(CapsuleComponent);
	Model->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0), FRotator(0.0f, -90.0f, 0.0f));

	MovementComponent = CreateDefaultSubobject<UAriaMovementComponent>(TEXT("Movement Component"));
	MovementComponent->Radius = 34.0f;
	MovementComponent->HalfHeight = 88.0f;
	MovementComponent->SetUpdatedComponent(CapsuleComponent);

}

// Called when the game starts or when spawned
void AAriaCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAriaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRootMotionMovementParams RootMotion = Model->ConsumeRootMotion();

	if (RootMotion.bHasRootMotion)
	{
		RootMotion.ScaleRootMotionTranslation(1.0f);
		FTransform WorldRootMotion = Model->ConvertLocalRootMotionToWorld(RootMotion.GetRootMotionTransform());
		CapsuleComponent->SetWorldLocation(CapsuleComponent->GetComponentLocation() + WorldRootMotion.GetTranslation(), false, NULL, ETeleportType::TeleportPhysics);
	}

}

// Called to bind functionality to input
void AAriaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AAriaCharacter::TurnModel()
{
	// Horrible quaternion voodoo. Viewer discretion is advised.
	// I'm honestly still not quite sure what I did.
	TargetDirection.Roll = 0.0f;
	TargetDirection.Pitch = 0.0f;
	FQuat test = FQuat::FindBetween(Model->GetComponentRotation().Vector(), TargetDirection.Vector());
	float angle = 0.0f;

	// We just need the angle between the vectors, not the axis (which should point upwards anyway).
	FVector dummy;
	test.ToAxisAndAngle(dummy, angle);

	// Turn faster if we're dashing.
	float multiply = 1.0f; //(Player->dashing ? 3.0f : 1.0f);

						   // Snap to the target angle if we're close enough, otherwise just keep turning.
	test = FQuat(dummy, FMath::DegreesToRadians(multiply*TurnRate)*GetWorld()->GetDeltaSeconds());
	float angle2 = 0.0f;
	test.ToAxisAndAngle(dummy, angle2);
	if (FMath::Abs(angle2) > FMath::Abs(angle)) {
		Model->SetWorldRotation(TargetDirection);
	} else {
		Model->AddLocalRotation(test);
	}
}