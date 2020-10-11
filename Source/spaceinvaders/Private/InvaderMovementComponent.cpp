// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInvaderMovementComponent::UInvaderMovementComponent()
	: state{ InvaderMovementType::STOP }
	, previousState {InvaderMovementType::STOP}
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
	}

	if (Parent) {
		
		FVector parentLocation = Parent->GetActorLocation();
		parentLocation.X += deltaX;
		parentLocation.Y += deltaY;
		Parent->SetActorLocation(parentLocation);
	}
}

