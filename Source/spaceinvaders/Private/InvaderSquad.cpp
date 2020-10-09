// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderSquad.h"
#include "Invader.h"




AInvaderSquad::AInvaderSquad() 
	: nRows(AInvaderSquad::defaultNRows)
	, nCols(AInvaderSquad::defaultNCols)
	, velocity(AInvaderSquad::defaultVelocity)
	, direction(1)
	, startPoint(AInvaderSquad::defaultStartPoint)
	, endPoint(AInvaderSquad::defaultEndPoint)
	, extraSeparation(AInvaderSquad::defaultExtraSeparation)

{
	Initialize();

}

void AInvaderSquad::Initialize() {
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInvaderSquad::BeginPlay()
{
	Super::BeginPlay();

	
	//Spawn Invaders
	
	FVector spawnLocation=this->startPoint;
	
	for (int i = 0; i < this->nCols; i++)
	{

		for (int j = 0; j < this->nRows; j++)
		{

			AInvader* spawnedInvader;
			spawnedInvader = (AInvader*)GetWorld()->SpawnActor(AInvader::StaticClass(), &spawnLocation);
			SquadMembers.Add(spawnedInvader);
			spawnLocation.X += AInvader::boundRadius * 2 + this->extraSeparation;
		}
		spawnLocation.X = this->startPoint.X;

		spawnLocation.Y += AInvader::boundRadius * 2 + this->extraSeparation;
	}

	
}

// Called every frame
void AInvaderSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto invader : SquadMembers) {
		//------------------------------------


		//------------------------------------
	}

}


// Static Members Initialization
FVector AInvaderSquad::defaultStartPoint = FVector(300.0f, -1800.0f, 0.0f);
FVector AInvaderSquad::defaultEndPoint = FVector(500.0f, 500.0f, 0.0f);

