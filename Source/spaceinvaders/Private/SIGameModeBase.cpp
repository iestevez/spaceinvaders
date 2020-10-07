// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "SIPawn.h"
#include "SIPlayerController.h"
#include "Invader.h"

ASIGameModeBase::ASIGameModeBase() {

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
}

void ASIGameModeBase::BeginPlay() {


	// TODO Mecánica de Spawn de un Blueprint Class
	//FTransform SpawnLocation;
	//TSubclassOf<class AInvader> bpInvader;
	//static ConstructorHelpers::FObjectFinder<UBlueprint> AssetBPInvader(TEXT("Blueprint'/Game/Blueprints/BP_Invader.BP_Invader'"));
	//if (AssetBPInvader.Object) {
	//	bpInvader = (UClass*)AssetBPInvader.Object->GeneratedClass;
	//}
	
	
	//GetWorld()->SpawnActor<AInvader>(bpInvader, &SpawnLocation);
}
