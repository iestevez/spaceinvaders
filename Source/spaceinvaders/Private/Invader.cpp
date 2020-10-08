// Fill out your copyright notice in the Description page of Project Settings.


#include "Invader.h"

// Sets default values
AInvader::AInvader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");

	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	Mesh->AttachTo(Root);

}

// Called when the game starts or when spawned
void AInvader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

