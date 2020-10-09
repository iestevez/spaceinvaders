// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "SIPawn.h"
#include "SIPlayerController.h"
#include "InvaderSquad.h"

ASIGameModeBase::ASIGameModeBase() {

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
}

void ASIGameModeBase::BeginPlay() {

	Super::BeginPlay();

	//Spawn a squad of invaders
	FTransform SpawnLocation;
	AInvaderSquad* spawnedInvaderSquad;
	spawnedInvaderSquad=(AInvaderSquad* )GetWorld()->SpawnActor(AInvaderSquad::StaticClass(), &SpawnLocation);
	
}
