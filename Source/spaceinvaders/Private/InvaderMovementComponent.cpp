// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SIGameModeBase.h"


// Sets default values for this component's properties
UInvaderMovementComponent::UInvaderMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Defaults:
	state = InvaderMovementType::STOP;
	isXHorizontal = false;
	horizontalVelocity = 1000.0f;
	verticalVelocity = 1000.0f;
	descendingStep = 100.0f;
	freeJumpRadius = 300.0f;
	freeJumpVelocity = 1000.0f;
	deltaAlphaInterpolation = 1.0f/30.0f;
	numberOfTargetPoints = 5;
	previousState = InvaderMovementType::STOP;
	
	// ...
}


// Called when the game starts
void UInvaderMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the GameMode
	UWorld* TheWorld;
	
	TheWorld = GetWorld();
	if (TheWorld) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		MyGameMode = Cast<ASIGameModeBase>(GameMode);
	}

	finalAngle = FMath::RandRange(-30.0f, 30.0f);

	// ...
	
}


// Called every frame
#pragma optimize("", off)
void UInvaderMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Parent = GetOwner(); //Parent is the actor who owns this component.
	
	if (!Parent)
		return;

	float deltaHorizontal = horizontalVelocity * DeltaTime; // Increment in horizontal and vertical dimensions given DeltaTime and parameterized velocities
	float deltaVertical = verticalVelocity * DeltaTime;
	
	float deltaX = 0.0f; // These deltas determine the change of the actor position
	float deltaY = 0.0f;

	// deltaX and deltaY are calculated differently for each movement type
	// The movement type is in the public variable state
	// previousState is updated
	switch (state) {

	case InvaderMovementType::STOP:
		deltaX = 0.0f; //No variation if STOP
		deltaY = 0.0f;
		previousState = InvaderMovementType::STOP;
		break;

	case InvaderMovementType::RIGHT:
		deltaX = isXHorizontal ? deltaHorizontal : 0.0f;
		deltaY = !isXHorizontal ? deltaHorizontal : 0.0f;
		previousState = InvaderMovementType::RIGHT;
		break;

	case InvaderMovementType::LEFT:
		deltaX = isXHorizontal ? -deltaHorizontal : 0.0f;
		deltaY = !isXHorizontal ? -deltaHorizontal : 0.0f;
		previousState = InvaderMovementType::LEFT;
		break;

		// Down movement: this is an automatic movement that has to finish automatically
		// It is based on an internal variable, descendinfProgress, that is updated.

	case InvaderMovementType::DOWN:
		if (previousState != InvaderMovementType::DOWN)
			descendingProgress = 0.0f;
		if (descendingProgress > descendingStep) {
			deltaVertical = 0.0f;
			MyGameMode->SquadFinishesDown.ExecuteIfBound();
		}
		deltaX = isXHorizontal ? 0.0f : -deltaVertical;
		deltaY = !isXHorizontal ? 0.0f : -deltaVertical;
		descendingProgress += deltaVertical;
		previousState = InvaderMovementType::DOWN;
		break;

		// Free jump movement: this is an automatic complex movement.

	case InvaderMovementType::FREEJUMP:
		deltaX = 0.0f; // This movement is not based on deltaX, deltaY, but in general transformations
		deltaY = 0.0f;
		
		//FTransform currentTransform = Parent->GetActorTransform();
		//FVector currentLocation = currentTransform.GetLocation();

		if (previousState != InvaderMovementType::FREEJUMP) { // First time we enter in FREEJUMP
			GenerateTargetPoints();
			currentTargetPoint = 0;
			if (numberOfTargetPoints > 0) {
				originTransform = Parent->GetActorTransform(); // First originTransform for interpolation is actor transform
				alphaInterpolation = 0.0f;
				
			}

			previousState = InvaderMovementType::FREEJUMP;
		}

		// Now the movement is programatically defined.
		// There are two stages:
		// First stage: an automatic movement defined by a sequence of target transforms
		// currentTargetPoint is the index of the current transform
		if (currentTargetPoint < numberOfTargetPoints) {
			FTransform newtransform = InterpolateWithTargetPoints(originTransform, alphaInterpolation); // New transform calculated by interpolation between current and currentTargetPoint.
			// The actor receive the new transform					
			Parent->SetActorTransform(newtransform);

			alphaInterpolation += deltaAlphaInterpolation;
			if (alphaInterpolation > 1.0f) { // target has been reached with interpolation
				++currentTargetPoint;
				alphaInterpolation = 0.0f; // To start a new segment of linear interpolation


				if (currentTargetPoint < numberOfTargetPoints) // new originTransform is previous target
					originTransform = this->targetPoints[currentTargetPoint - 1];
				// If this was the last target we add a random rotation that it will define the direction of the movement in the second stage of the free jump
				else {
					Parent->AddActorLocalRotation(FRotator(0.0f, finalAngle, 0.0f));
				}

			}


		}

		// Second stage: the actor is simply moved in the forward direction 

		else {

			FVector parentLocation = Parent->GetActorLocation();
			FVector forward = Parent->GetActorForwardVector();
			FVector right = Parent->GetActorRightVector();
			parentLocation += freeJumpVelocity  * DeltaTime * forward;

			Parent->SetActorLocation(parentLocation);
			

		}
	}





	// Apply calculated deltaX deltaY for those movements based on them
	if (Parent && state!=InvaderMovementType::FREEJUMP) {
		
		FVector parentLocation = Parent->GetActorLocation();
		parentLocation.X += deltaX;
		parentLocation.Y += deltaY;
		Parent->SetActorLocation(parentLocation);
	
	}
}
#pragma optimize("", on)

// This method produce the sequence of target transform for interpolation
#pragma optimize("", off)

void UInvaderMovementComponent::GenerateTargetPoints() {
	AActor* Parent = GetOwner();
	FTransform initialTransform;
	FVector initialLocation;
	FVector initialScale;
	
	FQuat initialQuaternion;
	FVector forward;
	if (!Parent) {
		numberOfTargetPoints = 0;
		return;
	}

	initialTransform = Parent->GetActorTransform();
	initialLocation = initialTransform.GetLocation();
	initialScale = initialTransform.GetScale3D();
	initialQuaternion = initialTransform.GetRotation();
	forward = Parent->GetActorForwardVector();


	

	// The first stage movement is a circle
	// Calculate center of the circle from actor location
	float radio = freeJumpRadius;
	FVector center = initialLocation;
	if (isXHorizontal)
		center.Y += radio;
	else
		center.X += radio;


	if (numberOfTargetPoints > 0) {
		float theta = 0.0f;
		float deltaTheta = 2*PI/numberOfTargetPoints;
		
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("X is %g Y is %g"), initialLocation.X, initialLocation.Y));

		FVector newLocation = initialLocation;
		FRotator rotation;
		FTransform newTransform = initialTransform;
		for (int32 i = 0; i < numberOfTargetPoints; i++) {
			float pc = FMath::Cos(theta);
			float ps = FMath::Sin(theta);
			newLocation.X = center.X - radio * FMath::Cos(theta);
			newLocation.Y = center.Y + radio * FMath::Sin(theta);
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("X is %g Y is %g"), newLocation.X, newLocation.Y));
			newTransform.SetLocation(newLocation);
			// Change the rotation of the actor to follow the tangent of the circle
			if (i != (numberOfTargetPoints - 1)) {
				// FRotator requires angles in degrees!
				rotation = FRotator(0.0f, -(theta*180.0f/PI) - 90, 0.0f);
				FQuat newQuaternion = rotation.Quaternion()*initialQuaternion;
				newTransform.SetRotation(newQuaternion);
			}
			else
				newTransform.SetRotation(initialQuaternion); //Last transformation 
			targetPoints.Add(newTransform);
			theta += deltaTheta;
		}
	
	}

	

}
#pragma optimize("", on)

FTransform UInvaderMovementComponent::InterpolateWithTargetPoints(FTransform origin, float fraction){

	FVector originLocation = origin.GetLocation();
	FQuat originRotation = origin.GetRotation();
	FVector targetLocation;
	FQuat targetRotation;
	if (currentTargetPoint >= 0 && currentTargetPoint < numberOfTargetPoints) {
		targetLocation = targetPoints[currentTargetPoint].GetLocation();
		targetRotation = targetPoints[currentTargetPoint].GetRotation();
	}
	else
		return origin;

	FVector newLocation = UKismetMathLibrary::VLerp(originLocation, targetLocation, fraction);
	FQuat newRotation = FQuat::Slerp(originRotation, targetRotation, fraction);

	FTransform newTransform = origin;
	newTransform.SetLocation(newLocation);
	newTransform.SetRotation(newRotation);
	return newTransform;
}