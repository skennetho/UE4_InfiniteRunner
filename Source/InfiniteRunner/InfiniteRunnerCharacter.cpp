// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfiniteRunnerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AInfiniteRunnerCharacter

AInfiniteRunnerCharacter::AInfiniteRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInfiniteRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("RunToggle", IE_Pressed, this, &AInfiniteRunnerCharacter::ToggleRun);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AInfiniteRunnerCharacter::TurnRight);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AInfiniteRunnerCharacter::TurnLeft);

	PlayerInputComponent->BindAxis("LeftRight", this, &AInfiniteRunnerCharacter::MoveLeftRight);

}

void AInfiniteRunnerCharacter::Tick(float DeltaTime)
{
	if (IsDead)
		return;

	TurnCorner();
	if (IsMovable) 
		Run();
	
	// calculate moved distance only ground
	/*Temp = this->GetVelocity();
	Distance += FMath::Sqrt((Temp.X*Temp.X) + (Temp.Y*Temp.Y)) * DeltaTime;*/
}

void AInfiniteRunnerCharacter::ToggleRun()
{
	IsMovable = !IsMovable;
}

void AInfiniteRunnerCharacter::TurnCorner()
{
	if (Controller != nullptr) {
		FRotator Rotation = Controller->GetControlRotation();

		if (Rotation == DesiredRotator) return;

		Rotation = FMath::RInterpTo(Rotation, DesiredRotator, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		Controller->SetControlRotation(Rotation);
	}
}

void AInfiniteRunnerCharacter::TurnLeft()
{
	if (CanTurn) {
		DesiredRotator = Controller->GetControlRotation();
		DesiredRotator += FRotator(0.f, -90.f, 0.f);
		CanTurn = false;
	}
	else {
		MoveLeftRight(-1.f);
	}
}

void AInfiniteRunnerCharacter::TurnRight()
{
	if (CanTurn) {
		DesiredRotator = Controller->GetControlRotation();
		DesiredRotator += FRotator(0.f, 90.f, 0.f);
		CanTurn = false;
	}
	else {
		MoveLeftRight(1.f);
	}
}

void AInfiniteRunnerCharacter::MoveLeftRight(float Value) {
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AInfiniteRunnerCharacter::AddCoin()
{
	UE_LOG(LogTemp, Warning, TEXT("TotalCoins achieve : %d"), TotalCoin);
	TotalCoin++;
}

void AInfiniteRunnerCharacter::ClearCoin()
{
	TotalCoin = 0;
}

void AInfiniteRunnerCharacter::Run()
{
	if ((Controller != nullptr) && (RunSpeed > 0.f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, RunSpeed);
	}
}