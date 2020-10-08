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
	// Sets default values for this actor's properties
	AInvaderSquad();

	AInvaderSquad(FVector& start, FVector& end, int32 p, int32 q);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize();
	void ChangeDirection();
	

private:
	FVector startPoint;
	FVector endPoint;
	int32 nRows;
	int32 nCols;
	float velocity;
	int32 direction;

	TArray<AInvader*> SquadMembers;

};
