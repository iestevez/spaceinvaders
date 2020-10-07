// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameModeBase.h"
#include "SIPawn.h"
#include "SIPlayerController.h"

ASIGameModeBase::ASIGameModeBase() {

	DefaultPawnClass = ASIPawn::StaticClass();
	PlayerControllerClass = ASIPlayerController::StaticClass();
}

