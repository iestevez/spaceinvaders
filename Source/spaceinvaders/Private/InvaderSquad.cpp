// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderSquad.h"




AInvaderSquad::AInvaderSquad() 
	: nRows(AInvaderSquad::defaultNRows)
	, nCols(AInvaderSquad::defaultNCols)
	, velocity(AInvaderSquad::defaultVelocity)
	, direction(1)
	, extraSeparation(AInvaderSquad::defaultExtraSeparation)
	, isXHorizontal {false}
	, numberOfMembers {nRows*nCols}

{
	// Create Components in actor

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root; // We need a RootComponent to have a base transform
	Initialize();

}

void AInvaderSquad::Initialize() {
	PrimaryActorTick.bCanEverTick = true;
	invaderTemplate = NewObject<AInvader>(); // template for spawning
}

// Called when the game starts or when spawned
void AInvaderSquad::BeginPlay()
{
	Super::BeginPlay();
	UWorld* TheWorld = GetWorld();
	
	// Bind to delegates
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		MyGameMode = Cast<ASIGameModeBase>(GameMode);

		if (MyGameMode != nullptr) {
			MyGameMode->SquadOnRightSide.BindUObject(this, &AInvaderSquad::SquadOnRightSide);
			MyGameMode->SquadOnLeftSide.BindUObject(this, &AInvaderSquad::SquadOnLeftSide);
			MyGameMode->SquadFinishesDown.BindUObject(this, &AInvaderSquad::SquadFinishesDown);
			MyGameMode->SquadOnDownSide.BindUObject(this, &AInvaderSquad::SquadOnDownSide);
			MyGameMode->InvaderDestroyed.AddUObject(this, &AInvaderSquad::RemoveInvader);
		}
	}
	
	//Spawn Invaders
	
	FVector actorLocation = GetActorLocation();
	FVector spawnLocation= actorLocation;
	FRotator spawnRotation = FRotator(0.0f, 180.0f, 0.0f); // Invader Forward is oposite to Player Forward (Yaw rotation)
	FActorSpawnParameters spawnParameters;
	int32 count = 0;
	
	for (int i = 0; i < this->nCols; i++)
	{

		for (int j = 0; j < this->nRows; j++)
		{
			invaderTemplate->positionInSquad = count;
			AInvader* spawnedInvader;
			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParameters.Template = invaderTemplate;
			spawnedInvader = GetWorld()->SpawnActor<AInvader>(spawnLocation,spawnRotation,spawnParameters);
			spawnedInvader->positionInSquad = count;
			++count;
			SquadMembers.Add(spawnedInvader);
			spawnLocation.X += AInvader::boundRadius * 2 + this->extraSeparation;
		}
		spawnLocation.X = actorLocation.X;

		spawnLocation.Y += AInvader::boundRadius * 2 + this->extraSeparation;
	}

	this->state = InvaderMovementType::RIGHT;

	
}

void AInvaderSquad::Destroyed() {
	Super::Destroyed();
	for (AInvader* invader : SquadMembers) {
		if (invader != nullptr)
			invader->Destroy();
	}
}

// Called every frame
void AInvaderSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSquadState();

}

void AInvaderSquad::UpdateSquadState() {
	for (auto invader : SquadMembers) {
		//------------------------------------
		if (invader) { // very important, first nullptr is checked!.
			// First, we get de movement component
			UInvaderMovementComponent* imc = (UInvaderMovementComponent*) invader->GetComponentByClass(UInvaderMovementComponent::StaticClass());
			// Now, its state is updated
			if (imc) {
				imc->horizontalVelocity = horizontalVelocity;
				imc->verticalVelocity = verticalVelocity;
				imc->isXHorizontal = isXHorizontal;
				imc->state = state;
			}
		}

			//------------------------------------
	}
}


// Handling events

void AInvaderSquad::SquadOnRightSide() {
	previousState = InvaderMovementType::RIGHT;
	state = InvaderMovementType::DOWN;
}

void AInvaderSquad::SquadOnLeftSide() {
	previousState = InvaderMovementType::LEFT;
	state = InvaderMovementType::DOWN;
}

void AInvaderSquad::SquadFinishesDown() {
	static int32 countActions = 0;
	++countActions;
	if (countActions >= numberOfMembers) {

		countActions = 0;
		switch (previousState) {
		case InvaderMovementType::RIGHT:
			state = InvaderMovementType::LEFT;
			break;
		case InvaderMovementType::LEFT:
			state = InvaderMovementType::RIGHT;
			break;
		default:
			state = InvaderMovementType::STOP;

		}
	}
}

void AInvaderSquad::SquadOnDownSide() {
	
	if (MyGameMode != nullptr)
		MyGameMode->SquadSuccessful.ExecuteIfBound();

}

void AInvaderSquad::RemoveInvader(int32 ind) {
	SquadMembers[ind] = nullptr;
}



// Static Members Initialization
//FVector AInvaderSquad::defaultStartPoint = FVector(300.0f, -1800.0f, 0.0f);
//FVector AInvaderSquad::defaultEndPoint = FVector(500.0f, 500.0f, 0.0f);//

