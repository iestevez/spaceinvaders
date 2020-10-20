// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UInvaderMovementComponent::UInvaderMovementComponent()
	: state{ InvaderMovementType::STOP }
	, previousState{ InvaderMovementType::STOP }
	, numberOfTargetPoints{ 7 }
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
void UInvaderMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Parent = GetOwner();

	float deltaHorizontal = horizontalVelocity * DeltaTime;
	float deltaVertical = verticalVelocity * DeltaTime;
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	switch (state) {
	case InvaderMovementType::STOP:
		deltaX = 0.0f;
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
	case InvaderMovementType::FREEJUMP:
		if (previousState != InvaderMovementType::FREEJUMP) {
			GenerateTargetPoints();
			currentTargetPoint = 0;
			previousState = InvaderMovementType::FREEJUMP;
		}
		if (currentTargetPoint < numberOfTargetPoints) {
			if (Parent) {
				FTransform transform = Parent->GetActorTransform();
				FTransform newtransform = InterpolateWithTargetPoints(transform, DeltaTime,verticalVelocity/100);
				FVector newlocation = newtransform.GetLocation();
				FVector distanceToTarget = this->targetPoints[currentTargetPoint].GetLocation() - newlocation;
				if (distanceToTarget.Size() < 10.0f) {
					++currentTargetPoint;
					
				}
				Parent->SetActorTransform(newtransform);
				if (currentTargetPoint == numberOfTargetPoints) {
					Parent->AddActorLocalRotation(FRotator(0.0f, finalAngle, 0.0f));
				}
			}
			
		} 
		else {
			
			if (Parent) {

				FVector parentLocation = Parent->GetActorLocation();
				FVector forward = Parent->GetActorForwardVector();
				FVector right = Parent->GetActorRightVector();
				
				
				

				if (!isXHorizontal)
					parentLocation += deltaVertical * forward;
				else
					parentLocation += deltaVertical * right;
				Parent->SetActorLocation(parentLocation);
			}

		}



	}

	if (Parent) {
		
		FVector parentLocation = Parent->GetActorLocation();
		parentLocation.X += deltaX;
		parentLocation.Y += deltaY;
		Parent->SetActorLocation(parentLocation);
	}
}

void UInvaderMovementComponent::GenerateTargetPoints() {
	AActor* Parent = GetOwner();
	FTransform initialTransform;
	FVector initialLocation;
	FVector initialScale;
	FQuat initialQuaternion;
	FVector forward;
	if (Parent) {

		initialTransform = Parent->GetActorTransform();
		initialLocation = initialTransform.GetLocation();
		initialScale = initialTransform.GetScale3D();
		initialQuaternion = initialTransform.GetRotation();
		forward = Parent->GetActorForwardVector();


	}
	float radio = 300.0f;
	FVector center = initialLocation;
	center.X += radio;
	if (numberOfTargetPoints > 0) {
		float theta = 0.0f;
		float deltaTheta = 360.0f/numberOfTargetPoints;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("X is %g Y is %g"), initialLocation.X, initialLocation.Y));

		FVector newLocation = initialLocation;
		FTransform newTransform = initialTransform;
		for (int32 i = 0; i < (numberOfTargetPoints-1); i++) {
			
			newLocation.X = center.X - radio * FMath::Cos(theta);
			newLocation.Y = center.Y + radio * FMath::Sin(theta);
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("X is %g Y is %g"), newLocation.X, newLocation.Y));
			newTransform.SetLocation(newLocation);
			targetPoints.Add(newTransform);
			theta += deltaTheta;
		}
	
	}

	this->targetPoints.Add(initialTransform);

}

FTransform UInvaderMovementComponent::InterpolateWithTargetPoints(FTransform transform, float delta, float speed){

	FTransform target = this->targetPoints[currentTargetPoint];
	FVector targetLocation = target.GetLocation();
	FVector originLocation = transform.GetLocation();

	FVector newLocation = FMath::VInterpTo(originLocation, targetLocation, delta, speed);

	FTransform newTransform = transform;
	newTransform.SetLocation(newLocation);
	return newTransform;
}