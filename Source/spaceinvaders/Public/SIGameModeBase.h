// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "Blueprint/UserWidget.h"
//#include "SpaceInvader.h"
//#include "SIPawn.h"
//#include "SIPlayerController.h"
//#include "InvaderSquad.h"
#include "SIGameModeBase.generated.h"

/**
 * 
 */


// Delegates of this game:
DECLARE_DELEGATE(FStandardDelegateSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOneParamMulticastDelegateSignature, int32);
DECLARE_DELEGATE_OneParam(FOneParamDelegateSignature, int32)


UCLASS()
class SPACEINVADERS_API ASIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	//------------------------------------------------
	//Spawned squad
	//------------------------------------------------
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		TSubclassOf<class AInvaderSquad> InvaderSquadClass;

	//------------------------------------------------
	//Point where the squad is spawned at
	//---------------------------------------------
	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category= "Level Layout")
		FVector spawnLocation = FVector(300.0f, -1800.0f, 150.0f);
	
	//------------------------------------------------
	//Layout of the squad: number of rows
	//-----------------------------------------------
	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category = "Level Layout")
		int32 nInvaderRows;

	//-----------------------------------------------
	//Layout of the squad: number of columns
	//-----------------------------------------------
	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category = "Level Layout")
		int32 nInvaderCols;


	//-------------------------------------------------
	// Level is oriented with its horizontal dimension along the X axis
	//-------------------------------------------------------
	//UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category= "Level Layout")
		//bool isXHorizontal = true;


	//-----------------------------------------------
	//Number of rounds to win the game (TODO)
	//-----------------------------------------------
	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="Game parameters")
		int32 nRounds = 0;
	
	//-----------------------------------------------
	//Points to add  the score up due to invader destruction
	//-----------------------------------------------
	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category="Game parameters")
		int32 pointsPerInvader = 1000;

	//-----------------------------------------------
	//Add  this points up to the score when a squad is finished
	//-----------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game parameters")
		int32 pointsPerSquad = 10000;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AInvaderSquad* spawnedInvaderSquad;

	
	// Delegates
	FStandardDelegateSignature SquadOnLeftSide;
	FStandardDelegateSignature SquadOnRightSide;
	FStandardDelegateSignature SquadOnDownSide;
	FStandardDelegateSignature SquadFinishesDown;
	FStandardDelegateSignature SquadDissolved;
	FStandardDelegateSignature SquadSuccessful;
	FOneParamMulticastDelegateSignature InvaderDestroyed;
	
	FOneParamDelegateSignature NewSquad;
	FStandardDelegateSignature PlayerZeroLifes;

	ASIGameModeBase();

	void EndGame();

protected:
	virtual void BeginPlay();
	
	
	UFUNCTION(BlueprintCallable)
		void RegenerateSquad();
	
	// Delegate bindings
	UFUNCTION(BlueprintCallable)
		void OnNewSquad(int32 lifes);
	
	UFUNCTION(BlueprintCallable)
		void OnPlayerZeroLifes();

};
