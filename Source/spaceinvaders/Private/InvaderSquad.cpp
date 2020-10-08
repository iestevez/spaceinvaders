// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderSquad.h"

// Sets default values
AInvaderSquad::AInvaderSquad()
	: nRows(5)
	,nCols(5)
	,velocity(1.0f)
	,direction(1)
	,startPoint(FVector(-500.0f,-500.0f,0.0f))
	,endPoint(FVector(500.0f,500.0f,0.0f))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Initialize();
	

}

AInvaderSquad::AInvaderSquad(FVector& start, FVector& end, int32 p, int32 q) 
	: nRows(p)
	, nCols(q)
	, velocity(1.0f)
	, direction(1)
	, startPoint(start)
	, endPoint(end)

{
	Initialize();

}

AInvaderSquad::Initialize() {
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInvaderSquad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInvaderSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

