// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"

//#include "SpaceInvader.h"
// Sets default values
ABullet::ABullet()
	: bulletType {BulletType::PLAYER}
	
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBoxComponent");

	RootComponent = Root; // We need a RootComponent to have a base transform
     SetBulletMesh();

	
	//Bullets will overlap, blocking behaviour is not desirable
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Mesh->AttachTo(Root);
	TriggerBox->AttachTo(Mesh);
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	location += DeltaTime * velocity * dir;
	SetActorLocation(location);
	

}


void ABullet::NotifyActorBeginOverlap(AActor* OtherActor) {
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s entered me"), *(OtherActor->GetName())));
	for (FName tag : autoDestroyTags)
		if (OtherActor->ActorHasTag(tag))
			Destroy();

	


}

void ABullet::SetBulletMesh(UStaticMesh* staticMesh, FString path, FVector scale) {
	const TCHAR* tpath;
	tpath = ABullet::defaultStaticMeshPath; // default route
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
		Mesh->SetRelativeScale3D(scale);
		
	}
}


