// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
//#include "SpaceInvader.h"
#include "SIPawn.h"
#include "SIPlayerController.h"
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

	
	
	UPROPERTY(EditAnyWhere, Category="Game parameters")
		int32 pointsPerInvader = 1000;

	UPROPERTY(EditAnywhere, Category = "Game parameters")
		int32 pointsPerSquad = 10000;

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
	FOneParamMulticastDelegateSignature InvaderDestroyed;
	FStandardDelegateSignature PlayerDestroyed;
	FOneParamDelegateSignature NewSquad;
	FStandardDelegateSignature PlayerZeroLifes;

	ASIGameModeBase();

	void EndGame();

protected:
	virtual void BeginPlay();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (BlueprintProtected="true"))
		TSubclassOf<class UUserWidget> SIHUDClass;

	UPROPERTY()
		class UUserWidget* hudWidget;
	
	// Delegate bindings
	void OnNewSquad(int32 lifes);
	void OnPlayerZeroLifes();

private:
	UPROPERTY()
		AActor* spawnedInvaderSquad;

	void RegenerateSquad();
	

};
