// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "InvaderSquad.h"
#include "SIPawn.h"
#include "SIPlayerController.h"

//UE4 Engine headers
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ASIGameModeBase::ASIGameModeBase()
	: nInvaderRows{ 5 }
	, nInvaderCols{ 7 }
	, pointsPerInvader{ 3000 }
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
		// Change defaults before Spawn
		AInvaderSquad* squad = Cast<AInvaderSquad>(InvaderSquadClass->GetDefaultObject());
		if (squad) {
			squad->SetCols( nInvaderCols);
			squad->SetRows(nInvaderRows);

			
			this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));
		}
	}

	// Change pawn parameters
	UWorld* TheWorld;
	TheWorld = GetWorld();
	ASIPawn* pawn = Cast<ASIPawn>((UGameplayStatics::GetPlayerController(TheWorld, 0))->GetPawn());
	if (pawn) {
		
		pawn->pointsPerInvader = pointsPerInvader;
		pawn->pointsPerSquad = pointsPerSquad;
		
	}

	// Delegate bindings:
	this->NewSquad.BindUObject(this, &ASIGameModeBase::OnNewSquad);
	this->PlayerZeroLifes.BindUObject(this, &ASIGameModeBase::OnPlayerZeroLifes);
	
}

void ASIGameModeBase::RegenerateSquad() {

	if (this->spawnedInvaderSquad != nullptr)
				this->spawnedInvaderSquad->Destroy();
	if (InvaderSquadClass) {
		// Change defaults before Spawn
		AInvaderSquad* squad = Cast<AInvaderSquad>(InvaderSquadClass->GetDefaultObject());
		if (squad) {
			squad->SetCols(nInvaderCols);
			squad->SetRows(nInvaderRows);

			this->spawnedInvaderSquad = Cast<AInvaderSquad>(GetWorld()->SpawnActor(InvaderSquadClass, &spawnLocation));
		}
	}
}

void ASIGameModeBase::EndGame() {
	if (this->spawnedInvaderSquad != nullptr) 
		this->spawnedInvaderSquad->Destroy();
	
	// Close level and open menu level
	UGameplayStatics::OpenLevel(this, FName("Menu"));

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



