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
	this->NewSquad.BindUObject(this, &ASIGameModeBase::OnNewSquad);
	this->PlayerZeroLifes.BindUObject(this, &ASIGameModeBase::OnPlayerZeroLifes);

	// Create and make the hud vissible

	if (SIHUDClass != nullptr) {
		hudWidget = CreateWidget<UUserWidget>(GetWorld(), SIHUDClass);
		if (hudWidget != nullptr)
			hudWidget->AddToViewport();
	}

	
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




void ASIGameModeBase::OnPlayerZeroLifes() {
	EndGame();
}

void ASIGameModeBase::OnNewSquad(int32 lifes) {
	if (lifes > 0)
		RegenerateSquad();
	else
		EndGame();
}



