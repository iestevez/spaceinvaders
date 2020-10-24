// Fill out your copyright notice in the Description page of Project Settings.


#include "Invader.h"
#include "SIGameModeBase.h"
#include "SpaceInvader.h"
#include "InvaderMovementComponent.h"
#include "Bullet.h"


// UE4 HEaders
//#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AInvader::AInvader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	
	bulletClass = ABullet::StaticClass();
	

	// Inmutable FNames for limits
	leftSideTag = FName(AInvader::leftSideTagString);
	rightSideTag = FName(AInvader::rightSideTagString);
	downSideTag = FName(AInvader::downSideTagString);
		
	// Create Components in actor

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBoxComponent");
	Movement = CreateDefaultSubobject<UInvaderMovementComponent>("InvaderMoveComponent");

	RootComponent = Root; // We need a RootComponent to have a base transform

	SetInvaderMesh();
	
	// Component hierarchy
	Mesh->AttachToComponent(Root,FAttachmentTransformRules::KeepWorldTransform);
	TriggerBox->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);
	AddOwnedComponent(Movement); // Because UInvaderMovementComponent is only an Actor Component and not a Scene Component can't Attach To.

	fireRate = 0.0001f;
}

// Called when the game starts or when spawned
void AInvader::BeginPlay()
{
	Super::BeginPlay();
	// Bullet template for spawning

	// Generate a Bullet Template of the correct class
	if (bulletClass->IsChildOf<ABullet>())
		bulletTemplate = NewObject<ABullet>(this, bulletClass->GetFName(),RF_NoFlags,bulletClass.GetDefaultObject());
	else
		bulletTemplate = NewObject<ABullet>();

	bulletTemplate->bulletType = BulletType::INVADER;

	//this->bulletTemplate = NewObject<ABullet>();
	//this->bulletTemplate->bulletType = BulletType::INVADER;
	
}

void AInvader::SetPositionInSquad(int32 index)
{
	this->positionInSquad = index;
}

int32 AInvader::GetPositionInSquad()
{
	return int32(this->positionInSquad);
}

// Called every frame
void AInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->timeFromLastShot += DeltaTime;
	
	// Fire?
	float val = FMath::RandRange(0.0f, 1.0f);
	if (val < (1.0-FMath::Exp(-fireRate*this->timeFromLastShot))) 
			Fire();
	
	
	

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

FVector AInvader::GetBoundOrigin() {
	return this->boundOrigin;

}

float AInvader::GetBoundRadius() {
	return this->boundRadius;
}


void AInvader::NotifyActorBeginOverlap(AActor* OtherActor) {
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s entered me"), *(OtherActor->GetName())));
	FName actorTag;
	
	
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr) {
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ASIGameModeBase* MyGameMode = Cast<ASIGameModeBase>(GameMode);
		if (OtherActor->ActorHasTag(leftSideTag))
			MyGameMode->SquadOnLeftSide.ExecuteIfBound();
		else if (OtherActor->ActorHasTag(rightSideTag))
			MyGameMode->SquadOnRightSide.ExecuteIfBound();
		else if (OtherActor->ActorHasTag(downSideTag)) {
			// First, we get de movement component
			UInvaderMovementComponent* imc = (UInvaderMovementComponent*)this->GetComponentByClass(UInvaderMovementComponent::StaticClass());
			if(imc->state!=InvaderMovementType::FREEJUMP)
				MyGameMode->SquadOnDownSide.ExecuteIfBound();
			else {
				MyGameMode->InvaderDestroyed.Broadcast(this->positionInSquad);
				Destroy();
			}
		}
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




void AInvader::SetInvaderMesh(UStaticMesh* newStaticMesh, const FString path, FVector scale ) {
	const TCHAR* tpath;
	tpath = AInvader::defaultStaticMeshName; // default route
	if (!Mesh) // No Mesh component
		return;

	if (!newStaticMesh) {
		if (!path.IsEmpty())
			tpath = *path;
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(tpath);
		newStaticMesh = MeshAsset.Object;
	}
	if(newStaticMesh){
	Mesh->SetStaticMesh(newStaticMesh);
	Mesh->SetRelativeScale3D(scale);
	FBoxSphereBounds meshBounds = Mesh->Bounds;
	//FBoxSphereBounds meshBounds = newStaticMesh->GetBounds();
	boundOrigin = meshBounds.Origin;
	boundRadius = meshBounds.SphereRadius;
	}
}




