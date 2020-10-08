// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SIGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FVector startPoint;

	UPROPERTY()
		FVector endPoint;

	UPROPERTY()
		int32 nInvaderRows;

	UPROPERTY()
		int32 nInvaderCols;


	ASIGameModeBase();
	virtual void BeginPlay();
	

};
