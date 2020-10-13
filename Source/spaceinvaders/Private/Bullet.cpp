// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

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

	if (ABullet::staticMesh == nullptr)
		setBulletMesh();

	if (ABullet::staticMesh != nullptr)
		Mesh->SetStaticMesh(ABullet::staticMesh);

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

void ABullet::setBulletMesh(const TCHAR* path) {

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(path);
	ABullet::staticMesh = MeshAsset.Object;
	if (ABullet::staticMesh != nullptr)
		Mesh->SetStaticMesh(ABullet::staticMesh);

}


