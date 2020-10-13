// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPawn.h"
#include "SIGameModeBase.h"

// Sets default values
ASIPawn::ASIPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isXHorizontal = false;
	bulletTemplate = NewObject<ABullet>();
	bulletTemplate->bulletType = BulletType::PLAYER;
	


}

// Called when the game starts or when spawned
void ASIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		MyGameMode->InvaderDestroyed.AddUObject(this, &ASIPawn::OnInvaderDestroyed);
		MyGameMode->SquadSuccessful.BindUObject(this, &ASIPawn::OnSquadSuccessful);
		MyGameMode->SquadDissolved.BindUObject(this, &ASIPawn::OnSquadDissolved);
		
	}
	

	
}

// Called every frame
void ASIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("SIRight"), this, &ASIPawn::OnMove);
	PlayerInputComponent->BindAction(TEXT("SIFire"), IE_Pressed, this, &ASIPawn::OnFire);

}

void ASIPawn::OnMove(float value) {
	float deltaTime = GetWorld()->GetDeltaSeconds();
	
	float delta = velocity * value * deltaTime;
	FVector dir;
	if (isXHorizontal)
		dir = FVector(1.0f, 0.0f, 0.0f);
	
	else
		dir = FVector(0.0f, 1.0f, 0.0f);
	
	AddMovementInput(dir, delta);
	

}

void ASIPawn::OnFire() {
	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	ABullet* spawnedBullet;
	bulletTemplate->velocity = bulletVelocity;
	bulletTemplate->dir = GetActorForwardVector();
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Template = bulletTemplate;
	spawnedBullet = (ABullet*)GetWorld()->SpawnActor<ABullet>(spawnLocation,spawnRotation,spawnParameters);
	
}


int32 ASIPawn::GetPlayerPoints()
{
	return this->playerPoints;
}

int32 ASIPawn::GetPlayerLifes()
{
	return int32(this->playerLifes);
}

void ASIPawn::NotifyActorBeginOverlap(AActor* OtherActor) {
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s entered me"), *(OtherActor->GetName())));
	FName actorTag;


	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		if (OtherActor->IsA(ABullet::StaticClass())) {
			ABullet* bullet = Cast<ABullet>(OtherActor);
			if (bullet->bulletType == BulletType::INVADER) {
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Player killed")));
				OtherActor->Destroy();
				OnPlayerDestroyed();
				
			}
		}


	}

}

void ASIPawn::OnPlayerDestroyed() {
	UWorld* TheWorld;
	TheWorld = GetWorld();
	ASIGameModeBase* MyGame = Cast<ASIGameModeBase>(UGameplayStatics::GetGameMode(TheWorld));
	if (MyGame) {
		--this->playerLifes;
		if (this->playerLifes == 0)
			MyGame->PlayerZeroLifes.ExecuteIfBound();
	}
}


// Delegate responses:
void ASIPawn::OnInvaderDestroyed(int32 id) {
	UWorld* TheWorld;
	TheWorld = GetWorld();
	ASIGameModeBase* MyGame = Cast<ASIGameModeBase>(UGameplayStatics::GetGameMode(TheWorld));
	if (MyGame) {
		this->playerPoints += MyGame->pointsPerInvader;
	}
}


void ASIPawn::OnSquadSuccessful() {
	UWorld* TheWorld;
	TheWorld = GetWorld();
	ASIGameModeBase* MyGame = Cast<ASIGameModeBase>(UGameplayStatics::GetGameMode(TheWorld));
	if (MyGame) {
		--this->playerLifes;
		MyGame->NewSquad.ExecuteIfBound(this->playerLifes);
	}
}

void ASIPawn::OnSquadDissolved() {
	UWorld* TheWorld;
	TheWorld = GetWorld();
	ASIGameModeBase* MyGame = Cast<ASIGameModeBase>(UGameplayStatics::GetGameMode(TheWorld));
	if (MyGame) {
		this->playerPoints += MyGame->pointsPerSquad;
		MyGame->NewSquad.ExecuteIfBound(this->playerLifes);
	}
}

