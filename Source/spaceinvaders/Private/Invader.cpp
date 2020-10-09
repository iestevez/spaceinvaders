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


	if (AInvader::staticMesh == nullptr) 
		setInvaderMesh();

	if(AInvader::staticMesh!=nullptr)
		Mesh->SetStaticMesh(AInvader::staticMesh);

	Mesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	Mesh->AttachTo(Root);

	FBoxSphereBounds meshBounds = Mesh->GetStaticMesh()->GetBounds();
	this->boundOrigin = meshBounds.Origin;
	this->boundRadius = meshBounds.SphereRadius;

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

//Getters and setters

FVector AInvader::getOrigin() {
	return this->boundOrigin;

}

float AInvader::getRadius() {
	return this->boundRadius;
}






// Static members initialization

void AInvader::setInvaderMesh(const TCHAR* path) {

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(path);
	AInvader::staticMesh = MeshAsset.Object;
}


UStaticMesh* AInvader::staticMesh = nullptr;

FVector AInvader::boundOrigin = FVector(0.0f, 0.0f, 0.0f);
float AInvader::boundRadius = 0.0f;
