// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "InvaderSquad.h"
#include "Invader.h"
#include "InvaderMovementComponent.h"
#include "SIGameModeBase.h"



AInvaderSquad::AInvaderSquad() 
	: nRows(AInvaderSquad::defaultNRows)
	, nCols(AInvaderSquad::defaultNCols)
	, velocity(AInvaderSquad::defaultVelocity)
	, direction(1)
	, startPoint(AInvaderSquad::defaultStartPoint)
	, endPoint(AInvaderSquad::defaultEndPoint)
	, extraSeparation(AInvaderSquad::defaultExtraSeparation)
	, isXHorizontal {false}
	, numberOfMembers {nRows*nCols}

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
	UWorld* TheWorld = GetWorld();
	ASIGameModeBase* MyGameMode;
	// Bind to delegates
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		MyGameMode = Cast<ASIGameModeBase>(GameMode);

		if (MyGameMode != nullptr) {
			MyGameMode->SquadOnRightSide.BindUObject(this, &AInvaderSquad::SquadOnRightSide);
			MyGameMode->SquadOnLeftSide.BindUObject(this, &AInvaderSquad::SquadOnLeftSide);
			MyGameMode->SquadFinishesDown.BindUObject(this, &AInvaderSquad::SquadFinishesDown);
			MyGameMode->SquadOnDownSide.BindUObject(this, &AInvaderSquad::SquadOnDownSide);
		}
	}
	
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
	UpdateSquadState();

}

void AInvaderSquad::UpdateSquadState() {
	for (auto invader : SquadMembers) {
		//------------------------------------
		if (invader) {
			// First, we get de movement component
			UInvaderMovementComponent* imc = (UInvaderMovementComponent*) invader->GetComponentByClass(UInvaderMovementComponent::StaticClass());
			// Now, its state is updated
			if (imc) {
				imc->horizontalVelocity = horizontalVelocity;
				imc->verticalVelocity = verticalVelocity;
				imc->isXHorizontal = isXHorizontal;
				imc->state = state;
			}
		}

			//------------------------------------
	}
}


// Handling events

void AInvaderSquad::SquadOnRightSide() {
	previousState = InvaderMovementType::RIGHT;
	state = InvaderMovementType::DOWN;
}

void AInvaderSquad::SquadOnLeftSide() {
	previousState = InvaderMovementType::LEFT;
	state = InvaderMovementType::DOWN;
}

void AInvaderSquad::SquadFinishesDown() {
	static int32 countActions = 0;
	++countActions;
	if (countActions >= numberOfMembers) {

		countActions = 0;
		switch (previousState) {
		case InvaderMovementType::RIGHT:
			state = InvaderMovementType::LEFT;
			break;
		case InvaderMovementType::LEFT:
			state = InvaderMovementType::RIGHT;
			break;
		default:
			state = InvaderMovementType::STOP;

		}
	}
}

void AInvaderSquad::SquadOnDownSide() {
	


}



// Static Members Initialization
FVector AInvaderSquad::defaultStartPoint = FVector(300.0f, -1800.0f, 0.0f);
FVector AInvaderSquad::defaultEndPoint = FVector(500.0f, 500.0f, 0.0f);

