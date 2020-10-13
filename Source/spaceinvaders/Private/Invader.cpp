// Fill out your copyright notice in the Description page of Project Settings.


#include "Invader.h"

// Sets default values
AInvader::AInvader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components in actor

	Root = CreateDefaultSubobject<USceneComponent>("Root");

	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBoxComponent");

	Movement = CreateDefaultSubobject<UInvaderMovementComponent>("InvaderMoveComponent");



	RootComponent = Root; // We need a RootComponent to have a base transform


	if (AInvader::staticMesh == nullptr) 
		setInvaderMesh();

	if(AInvader::staticMesh!=nullptr)
		Mesh->SetStaticMesh(AInvader::staticMesh);

	//Mesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	
	Mesh->AttachTo(Root);
	TriggerBox->AttachTo(Mesh);
	AddOwnedComponent(Movement); // Because UInvaderMovementComponent is only an Actor Component and not a Scene Component can't Attach To.

	FBoxSphereBounds meshBounds = Mesh->GetStaticMesh()->GetBounds();
	this->boundOrigin = meshBounds.Origin;
	this->boundRadius = meshBounds.SphereRadius;

	

}

// Called when the game starts or when spawned
void AInvader::BeginPlay()
{
	Super::BeginPlay();
	// Bullet template for spawning
	this->bulletTemplate = NewObject<ABullet>();
	this->bulletTemplate->bulletType = BulletType::INVADER;
	
}

// Called every frame
void AInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->timeFromLastShot += DeltaTime;
	
	// Fire?
	float val = FMath::RandRange(0.0f, 1.0f);
	if (val < (1.0-FMath::Exp(-fireRate*this->timeFromLastShot))) {
			Fire();
			
	}

	
	

}

void AInvader::Fire() {
	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	ABullet* spawnedBullet;
	if (this->bulletTemplate) {
	this->bulletTemplate->velocity = bulletVelocity;
	this->bulletTemplate->dir = GetActorForwardVector();
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Template = this->bulletTemplate;
	spawnedBullet = (ABullet*)GetWorld()->SpawnActor<ABullet>(spawnLocation, spawnRotation, spawnParameters);
	this->timeFromLastShot = 0.0f;
	}
}

//Getters and setters

FVector AInvader::getOrigin() {
	return this->boundOrigin;

}

float AInvader::getRadius() {
	return this->boundRadius;
}


void AInvader::NotifyActorBeginOverlap(AActor* OtherActor) {
	// Debug
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s entered me"), *(OtherActor->GetName())));
	FName actorTag;
	
	
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		if (OtherActor->ActorHasTag(leftSideTag))
			MyGameMode->SquadOnLeftSide.ExecuteIfBound();
		else if (OtherActor->ActorHasTag(rightSideTag))
			MyGameMode->SquadOnRightSide.ExecuteIfBound();
		else if (OtherActor->ActorHasTag(downSideTag))
			MyGameMode->SquadOnDownSide.ExecuteIfBound();
		// Invader destruction
		if (OtherActor->IsA(ABullet::StaticClass())) {
			ABullet* bullet = Cast<ABullet>(OtherActor);
			if (bullet->bulletType == BulletType::PLAYER) {
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Invader %d killed"), this->positionInSquad));
				OtherActor->Destroy();
				MyGameMode->InvaderDestroyed.Broadcast(this->positionInSquad);
				Destroy();
			}
		}


	}

}



// Static members initialization

void AInvader::setInvaderMesh(const TCHAR* path) {

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(path);
	AInvader::staticMesh = MeshAsset.Object;
}


UStaticMesh* AInvader::staticMesh = nullptr;

FVector AInvader::boundOrigin = FVector(0.0f, 0.0f, 0.0f);
float AInvader::boundRadius = 0.0f;
