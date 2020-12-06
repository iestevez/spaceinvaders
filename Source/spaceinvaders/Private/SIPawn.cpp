// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPawn.h"
#include "SIGameModeBase.h"
#include "Bullet.h"
#include "Invader.h"
// UE4 HEaders

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"




// Sets default values
ASIPawn::ASIPawn() 
	: playerPoints{ 0 }
	, playerLifes{ 2 }
	, pointsPerInvader{1000}
	, pointsPerSquad{10000}
	, bFrozen {false}
	, bPause {false}
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isXHorizontal = false;
	bulletClass = ABullet::StaticClass();
	PFXExplosion = nullptr;
	
	SetStaticMesh(); // Default mesh (SetStaticMesh with no arguments)

	// Audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Set a static mesh.
void ASIPawn::SetStaticMesh(UStaticMesh* staticMesh, FString path, FVector scale) {
	UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	const TCHAR* tpath;
	tpath = ASIPawn::defaultStaticMeshPath; // default route
	if (!Mesh) // No Mesh component
		return;

	if (!staticMesh) {
		if (!path.IsEmpty())
			tpath = *path;
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(tpath);
		staticMesh = MeshAsset.Object;
	}
	if (staticMesh) {
		Mesh->SetStaticMesh(staticMesh);

		Mesh->SetRelativeScale3D(FVector(1.0f,1.0f,1.0f));
	}
}

// Called when the game starts or when spawned
void ASIPawn::BeginPlay()
{
	Super::BeginPlay();

	// Generate a Bullet Template of the correct class
	if (bulletClass->IsChildOf<ABullet>())
		bulletTemplate = NewObject<ABullet>(this, bulletClass);
	else
		bulletTemplate = NewObject<ABullet>(this);

	//bulletTemplate = NewObject<ABullet>();
	bulletTemplate->bulletType = BulletType::PLAYER;
	
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		if (MyGameMode) {
			InvaderDestroyed = &MyGameMode->InvaderDestroyed;
			SquadDissolved = &MyGameMode->SquadDissolved;
			SquadSuccessful = &MyGameMode->SquadSuccessful;
			NewSquad = &MyGameMode->NewSquad;
			PlayerZeroLifes = &MyGameMode->PlayerZeroLifes;

			InvaderDestroyed->AddUObject(this, &ASIPawn::OnInvaderDestroyed);
			SquadSuccessful->BindUObject(this, &ASIPawn::OnSquadSuccessful);
			SquadDissolved->BindUObject(this, &ASIPawn::OnSquadDissolved);
		}
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
	PlayerInputComponent->BindAction(TEXT("SIPause"), IE_Pressed, this, &ASIPawn::OnPause);

}

void ASIPawn::OnPause() {
	bPause = !bPause;
	//if(bPause)
}

void ASIPawn::OnMove(float value) {
	
	if (bFrozen)
		return;

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
	if (bFrozen)
		return;
	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	ABullet* spawnedBullet;
	bulletTemplate->velocity = bulletVelocity;
	bulletTemplate->dir = GetActorForwardVector();
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Template = bulletTemplate;
	spawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(bulletClass,&spawnLocation,&spawnRotation,spawnParameters));
	if (AudioComponent != nullptr && AudioShoot!=nullptr) {
		AudioComponent->SetSound(AudioShoot);
		AudioComponent->Play();
	}
		
	
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
	
	if (!bFrozen) {
			// Collision with an enemy
			if (OtherActor->IsA(ABullet::StaticClass())) {
				ABullet* bullet = Cast<ABullet>(OtherActor);
				if (bullet->bulletType == BulletType::INVADER) {
					OtherActor->Destroy();
					OnPlayerDestroyed();
				}
			}
			// Collision with an invader
			if (OtherActor->IsA(AInvader::StaticClass())) {
				AInvader* invader = Cast<AInvader>(OtherActor);
				OtherActor->Destroy();
				OnPlayerDestroyed();

			}
	}

}

void ASIPawn::OnPlayerDestroyed() {
	UWorld* TheWorld;
	TheWorld = GetWorld();
	
	if (TheWorld ) {
		bFrozen = true; // Pawn can'tmove or fire while being destroyed
		--this->playerLifes;
		UStaticMeshComponent* LocalMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
		// Hide Static Mesh Component
		if (LocalMeshComponent != nullptr) {
			LocalMeshComponent->SetVisibility(false);
		}
		if (PFXExplosion != nullptr) {
			UGameplayStatics::SpawnEmitterAtLocation(TheWorld, PFXExplosion, this->GetActorTransform(), true);
		}
		//Audio
		if (AudioComponent != nullptr && AudioExplosion != nullptr) {
			AudioComponent->SetSound(AudioExplosion);
			AudioComponent->Play();
		}
		// Wait:
		TheWorld->GetTimerManager().SetTimer(timerHandle,this, &ASIPawn::PostPlayerDestroyed,3.0f,false);
	}
}

void ASIPawn::PostPlayerDestroyed() {
	
	if (this->playerLifes == 0) {
		if(this->PlayerZeroLifes)
			this->PlayerZeroLifes->ExecuteIfBound();
		return;
	}
	
	UStaticMeshComponent* LocalMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	// Hide Static Mesh Component
	if (LocalMeshComponent != nullptr) {
		LocalMeshComponent->SetVisibility(true);
	}
	// Unfrozing
	bFrozen = false; 
	
}


// Delegate responses:
void ASIPawn::OnInvaderDestroyed(int32 id) {
	this->playerPoints += this->pointsPerInvader;
}


void ASIPawn::OnSquadSuccessful() {
		--this->playerLifes;
		if(this->NewSquad)
			this->NewSquad->ExecuteIfBound(this->playerLifes);
}

void ASIPawn::OnSquadDissolved() {
		this->playerPoints += this->pointsPerSquad;
		if(this->NewSquad)
			this->NewSquad->ExecuteIfBound(this->playerLifes);
}

