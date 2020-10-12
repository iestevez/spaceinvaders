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
	/**
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		MyGameMode = Cast<ASIGameModeBase>(GameMode);

		
	}
	*/
	
}

// Called every frame
void ASIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("SIRight"), this, &ASIPawn::OnMove);
	PlayerInputComponent->BindAction(TEXT("SIFire"), IE_Pressed, this, &ASIPawn::OnFire);

}

void ASIPawn::OnMove(float value) {
	float deltaTime = GetWorld()->GetDeltaSeconds();
	//FVector location = GetActorLocation();
	float delta = velocity * value * deltaTime;
	FVector dir;
	if (isXHorizontal)
		dir = FVector(1.0f, 0.0f, 0.0f);
	//location.X += delta;
	else
		dir = FVector(0.0f, 1.0f, 0.0f);
	//	location.Y += delta;
	AddMovementInput(dir, delta);
	//SetActorLocation(location);

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
	/*
	spawnedBullet = (ABullet*)GetWorld()->SpawnActor(ABullet::StaticClass(), &spawnLocation);
	spawnedBullet->bulletType = BulletType::PLAYER;
	spawnedBullet->velocity = bulletVelocity;
	spawnedBullet->dir = GetActorForwardVector();
	*/
}


void ASIPawn::NotifyActorBeginOverlap(AActor* OtherActor) {
	// Debug
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s entered me"), *(OtherActor->GetName())));
	FName actorTag;


	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		if (OtherActor->IsA(ABullet::StaticClass())) {
			ABullet* bullet = Cast<ABullet>(OtherActor);
			if (bullet->bulletType == BulletType::INVADER) {
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Player killed")));
				OtherActor->Destroy();
				MyGameMode->PlayerDestroyed.ExecuteIfBound();
				
			}
		}


	}

}
