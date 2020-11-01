// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "InvaderSquad.h"
#include "SIPawn.h"
#include "SIPlayerController.h"

//UE4 Engine headers
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ASIGameModeBase::ASIGameModeBase()
	: isXHorizontal{ false }
	, nInvaderRows{ 5 }
	, nInvaderCols{ 7 }
	, pointsPerInvader{ 1000 }
	, pointsPerSquad{ 10000 }


{

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
	InvaderSquadClass = AInvaderSquad::StaticClass();

	
}

void ASIGameModeBase::BeginPlay() {

	Super::BeginPlay();

	//Spawn a squad of invaders
	
	if (InvaderSquadClass) {
		this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));
		//Cast<AIvaderSquad>(this->spawnedInvaderSquad)->nRows = nInvaderRows;
		//this->spawnedInvaderSquad->nCols = nInvaderCols;
	
	}




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
	
	this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));

}

void ASIGameModeBase::EndGame() {
	if (this->spawnedInvaderSquad != nullptr) {
		this->spawnedInvaderSquad->Destroy();
	}
	UGameplayStatics::OpenLevel(this,FName("Menu"));
	// Close level and open menu level
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



