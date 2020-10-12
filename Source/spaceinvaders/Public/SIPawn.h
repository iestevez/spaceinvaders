// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Bullet.h"
//#include "SIGameModeBase.h"

#include "SIPawn.generated.h"

UCLASS()
class SPACEINVADERS_API ASIPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float velocity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletVelocity = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isXHorizontal = true;
	UPROPERTY()
		ABullet* bulletTemplate; // used as template for spawning

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// Sets default values for this pawn's properties
	ASIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMove(float value);

	void OnFire();

private:
	
};
