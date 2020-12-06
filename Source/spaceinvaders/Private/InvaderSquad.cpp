// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderSquad.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "Invader.h"
#include "InvaderMovementComponent.h"
#include "SIGameModeBase.h"




AInvaderSquad::AInvaderSquad() 
	: nRows{ AInvaderSquad::defaultNRows }
	, nCols{AInvaderSquad::defaultNCols}
	, horizontalVelocity {AInvaderSquad::defaultHorizontalVelocity}
	, verticalVelocity {AInvaderSquad::defaultVerticalVelocity}
	, extraSeparation(AInvaderSquad::defaultExtraSeparation)
	//, isXHorizontal {false}
	, numberOfMembers {nRows*nCols}
	, freeJumpRate{ 0.0001 }
	, timeFromLastFreeJump {0.0}
	

{
	// Create Components in actor

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root; // We need a RootComponent to have a base transform
	Initialize();

}

void AInvaderSquad::SetRows(int32 nrows) {
	this->nRows = nrows;
	this->numberOfMembers = this->nRows * this->nCols;
}

void  AInvaderSquad::SetCols(int32 ncols) {
	this->nCols = ncols;
	this->numberOfMembers = this->nRows * this->nCols;
}

int32 AInvaderSquad::GetRows() {
	return this->nRows;
}

int32 AInvaderSquad::GetCols() {
	return this->nCols;
}

int32 AInvaderSquad::GetNumberOfMembers() {
	return this->numberOfMembers;
}
void AInvaderSquad::Initialize() {
	PrimaryActorTick.bCanEverTick = true;
	
	
}




// Called when the game starts or when spawned
void AInvaderSquad::BeginPlay()
{
	Super::BeginPlay();

	// Set Invader Template with Default Value for invaderClass
	if (invaderClass->IsChildOf<AInvader>())
		invaderTemplate = NewObject<AInvader>(this, invaderClass->GetFName(), RF_NoFlags, invaderClass.GetDefaultObject());
	else
		invaderTemplate = NewObject<AInvader>();
	//invaderTemplate = NewObject<AInvader>(); // template for spawning
	//invaderTemplate->SetInvaderMesh(invaderStaticMesh);
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
	AInvader* spawnedInvader;
	float radiusX=0.0f;
	float radiusY = 0.0f;
	for (int i = 0; i < this->nCols; i++)
	{

		for (int j = 0; j < this->nRows; j++)
		{
			//invaderTemplate->SetPositionInSquad(count);
			
			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParameters.Template = invaderTemplate;
			spawnedInvader = GetWorld()->SpawnActor<AInvader>(spawnLocation,spawnRotation,spawnParameters);
			spawnedInvader->SetPositionInSquad(count);
			++count;
			SquadMembers.Add(spawnedInvader);
			float r = spawnedInvader->GetBoundRadius();
			if (r > radiusX)
				radiusX = r;
			if (r > radiusY)
				radiusY = r;
			spawnLocation.X += radiusX * 2 + this->extraSeparation;
		}
		spawnLocation.X = actorLocation.X;

		spawnLocation.Y += radiusY * 2 + this->extraSeparation;
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
	UpdateSquadState(DeltaTime);

}

void AInvaderSquad::UpdateSquadState(float delta) {
	
	TArray<AInvader*> survivors;
	
	for (auto invader : SquadMembers) {
		//------------------------------------
		if (invader) { // very important, first nullptr is checked!.
			
			
			// First, we get de movement component
			UInvaderMovementComponent* imc = (UInvaderMovementComponent*) invader->GetComponentByClass(UInvaderMovementComponent::StaticClass());
			
			// Now, its state is updated
			if (imc) {
				if (imc->state != InvaderMovementType::FREEJUMP)
					survivors.Emplace(invader);
				imc->horizontalVelocity = horizontalVelocity;
				imc->verticalVelocity = verticalVelocity;
				//imc->isXHorizontal = isXHorizontal;
				if(imc->state!=InvaderMovementType::FREEJUMP) // The state of the squad is copied to the invader state (except for those in FREEJUMP)
					imc->state = state;
			}
		}

			//------------------------------------
	}
	this->timeFromLastFreeJump += delta;
	float val = FMath::RandRange(0.0f, 1.0f);
	int32 countSurvivors = survivors.Num();
	if (countSurvivors>0 && val < (1.0 - FMath::Exp(-freeJumpRate * this->timeFromLastFreeJump))) {
		int32 ind = FMath::RandRange(0, countSurvivors - 1); // Randomly select one of the living invaders
		UInvaderMovementComponent* imc = (UInvaderMovementComponent*)survivors[ind]->GetComponentByClass(UInvaderMovementComponent::StaticClass());
		if (imc) {
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s on FreeJump"), *(imc->GetName())));
			survivors[ind]->fireRate *= 100;
			imc->state = InvaderMovementType::FREEJUMP;
		}
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
	--this->numberOfMembers;
	if (this->numberOfMembers == 0) {
		if (MyGameMode != nullptr) {
			MyGameMode->SquadDissolved.ExecuteIfBound(); // parameter larger than 0 to avoid finishing game!
			
		}
	}
}

