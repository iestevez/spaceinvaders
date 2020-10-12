// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "SpaceInvader.h"
#include "SIPawn.h"
#include "SIPlayerController.h"
//#include "InvaderSquad.h"
#include "SIGameModeBase.generated.h"

/**
 * 
 */

// Forward Declarations
//class AInvaderSquad;

// Delegates of this game:
DECLARE_DELEGATE(FStandardDelegateSignature)
DECLARE_DELEGATE_OneParam(FOneParamDelegateSignature, int32)


UCLASS()
class SPACEINVADERS_API ASIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY()
		UClass* InvaderSquadClass;

	UPROPERTY()
		FVector startPoint;

	UPROPERTY()
		FVector endPoint;

	UPROPERTY()
		int32 nInvaderRows;

	UPROPERTY()
		int32 nInvaderCols;

	UPROPERTY()
		int32 nRounds = 0;

	UPROPERTY()
		int32 playerLifes = 2;
	
	UPROPERTY()
		int32 pointsPerInvader = 1000;

	UPROPERTY()
		int32 playerPoints = 0;

	UPROPERTY()
		bool isXHorizontal = true;

	UPROPERTY()
		FVector spawnLocation = FVector(300.0f,-1800.0f,150.0f);


	// Delegates
	FStandardDelegateSignature SquadOnLeftSide;
	FStandardDelegateSignature SquadOnRightSide;
	FStandardDelegateSignature SquadOnDownSide;
	FStandardDelegateSignature SquadFinishesDown;
	FStandardDelegateSignature SquadDissolved;
	FStandardDelegateSignature SquadSuccessful;
	FOneParamDelegateSignature InvaderDestroyed;
	FStandardDelegateSignature PlayerDestroyed;

	ASIGameModeBase();

	void EndGame();

protected:
	virtual void BeginPlay();
	void OnSquadSuccessful();
	void OnSquadDissolved();
	void OnInvaderDestroyed();
	void OnPlayerDestroyed();
private:
	UPROPERTY()
		AActor* spawnedInvaderSquad;

	void RegenerateSquad();
	

};
