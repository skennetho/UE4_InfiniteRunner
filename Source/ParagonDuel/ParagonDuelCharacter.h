// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParagonDuelCharacter.generated.h"

UCLASS(config=Game)
class AParagonDuelCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	virtual void Tick(float DeltaTime) override;

public:
	AParagonDuelCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
	bool IsMovable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
	float RunSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner, meta = (AllowPrivateAccess = "true"))
	bool CanTurn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner, meta = (AllowPrivateAccess = "true"))
	bool IsDead = false;

	FRotator DesiredRotator = FRotator(0,0,0);
	float InterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner, meta = (AllowPrivateAccess = "true"))
	int TotalCoin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner, meta = (AllowPrivateAccess = "true"))
	float Distance = 0.0f;

private: 
	FVector Temp;
public:
	void Run();
	void ToggleRun();

	void TurnCorner();
	void TurnLeft();
	void TurnRight();
	void MoveLeftRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Coin")
	void AddCoin();

	UFUNCTION(BlueprintCallable, Category = "Coin")
	void ClearCoin();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

