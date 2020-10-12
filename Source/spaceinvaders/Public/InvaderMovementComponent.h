// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvader.h"
#include "SIGameModeBase.h"
#include "InvaderMovementComponent.generated.h"


//Forward declarations
//class ASIGameModeBase;




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADERS_API UInvaderMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInvaderMovementComponent();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float horizontalVelocity=1000.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float verticalVelocity = 1000.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		InvaderMovementType state = InvaderMovementType::STOP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isXHorizontal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float descendingStep = 100.0f;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	InvaderMovementType previousState;
	float descendingProgress = 0.0f;

	ASIGameModeBase* MyGameMode; // to call delegates

		
};
