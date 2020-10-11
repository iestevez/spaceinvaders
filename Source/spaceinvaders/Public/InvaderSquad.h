// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Invader.h"
#include "InvaderSquad.generated.h"

UCLASS()
class SPACEINVADERS_API AInvaderSquad : public AActor
{
	GENERATED_BODY()

public:

	AInvaderSquad();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float horizontalVelocity = 1000.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float verticalVelocity = 1000.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		InvaderMovementType state = InvaderMovementType::STOP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isXHorizontal = true;

	UFUNCTION()
		void SquadOnLeftSide();

	UFUNCTION()
		void SquadOnRightSide();

	UFUNCTION()
		void SquadOnDownSide();

	UFUNCTION()
		void SquadFinishesDown();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize();
	void ChangeDirection();

	void UpdateSquadState();


private:
	FVector startPoint;
	FVector endPoint;
	int32 nRows;
	int32 nCols;
	int32 numberOfMembers;
	float extraSeparation;
	float velocity;
	int32 direction;

	static const int32 defaultNRows = 5;
	static const int32 defaultNCols = 7;
	static  FVector defaultStartPoint;
	static FVector defaultEndPoint;
	static  constexpr const float defaultVelocity = 1.0f;
	static  constexpr const float defaultExtraSeparation = 0.0f;

	InvaderMovementType previousState = InvaderMovementType::STOP;

	TArray<AInvader*> SquadMembers;

};
