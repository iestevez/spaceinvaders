// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ASIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASIPlayerController();

	virtual void BeginPlay() override;

private:
	UPROPERTY()
		TArray<class AActor*> m_cameras;
	FName m_mainCameraTag;
	
};
