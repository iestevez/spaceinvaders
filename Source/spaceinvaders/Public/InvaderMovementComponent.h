// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceInvader.h"

#include "InvaderMovementComponent.generated.h"






UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADERS_API UInvaderMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInvaderMovementComponent();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float horizontalVelocity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float verticalVelocity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		InvaderMovementType state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isXHorizontal;


	// Down movement parameters:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float descendingStep; // Length of the descending step


	// Free jump parameters:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FTransform> targetPoints; // The movement will be composed by interpolation using a sequence of reference poses stored in this array.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 numberOfTargetPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float freeJumpRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float freeJumpVelocity; // Velocity in the first stage of the free jump

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float deltaAlphaInterpolation; //1/(Number of updates between two target points)

	
	
	
protected:

	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void GenerateTargetPoints();

	UFUNCTION(BlueprintCallable)
		FTransform InterpolateWithTargetPoints(FTransform transform, float covered);
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	InvaderMovementType previousState; // Store state in previous frame (to know when a state is beginning)

	// Down movement state variables:
	float descendingProgress = 0.0f; // Store progress in the Down state

	// Free jump movement state variables:
	FTransform originTransform;
	float alphaInterpolation;
	int32 currentTargetPoint=0; // It stores the index of the first reference pose (the other is currentTargetPoint+1)
	float finalAngle; // Orientation of the invader to start the final attack

	
	class ASIGameModeBase* MyGameMode; // to call delegates

		
};
