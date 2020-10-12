// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "InvaderSquad.h"


ASIGameModeBase::ASIGameModeBase()
	: isXHorizontal{ false }

{

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
	InvaderSquadClass = AInvaderSquad::StaticClass();
	
}

void ASIGameModeBase::BeginPlay() {

	Super::BeginPlay();

	//Spawn a squad of invaders
	
	
	this->spawnedInvaderSquad=GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation);

	// Delegate bindings:
	ASIGameModeBase::SquadSuccessful.BindUObject(this, &ASIGameModeBase::OnSquadSuccessful);
	ASIGameModeBase::SquadDissolved.BindUObject(this, &ASIGameModeBase::OnSquadDissolved);
	ASIGameModeBase::PlayerDestroyed.BindUObject(this, &ASIGameModeBase::OnPlayerDestroyed);
	
}

void ASIGameModeBase::RegenerateSquad() {

	if (this->spawnedInvaderSquad != nullptr) {

		this->spawnedInvaderSquad->Destroy();
	}
	
	this->spawnedInvaderSquad = GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation);

}

void ASIGameModeBase::EndGame() {
	if (this->spawnedInvaderSquad != nullptr) {
		this->spawnedInvaderSquad->Destroy();
	}
}

void ASIGameModeBase::OnSquadSuccessful() {
   --this->playerLifes;
   
   if (this->playerLifes > 0)
	   RegenerateSquad();
   else
	   EndGame();
}

void ASIGameModeBase::OnSquadDissolved() {
	++this->ASIGameModeBase::nRounds;
	ASIGameModeBase::RegenerateSquad();
}

void ASIGameModeBase::OnInvaderDestroyed() {

}

void ASIGameModeBase::OnPlayerDestroyed() {
	--this->playerLifes;
	if (this->playerLifes == 0)
		EndGame();
	else
		RegenerateSquad();

}


