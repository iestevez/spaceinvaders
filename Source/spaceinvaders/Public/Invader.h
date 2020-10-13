// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "SpaceInvader.h"
#include "InvaderMovementComponent.h"
#include "Invader.generated.h"

UCLASS()
class SPACEINVADERS_API AInvader : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY()
		UStaticMeshComponent* Mesh;

	UPROPERTY()
		UInvaderMovementComponent* Movement;

	UPROPERTY()
		UBoxComponent* TriggerBox;

	UPROPERTY()
		float fireRate = 0.0001f;

	UPROPERTY()
		float bulletVelocity = 3000.0f;

	UPROPERTY()
		ABullet* bulletTemplate;
	
	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;



	// Sets default values for this actor's properties
	AInvader();

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector getOrigin();
	float getRadius();
	static void setInvaderMesh(const TCHAR* path=AInvader::defaultStaticMeshName);
	static  UStaticMesh* staticMesh;
	static FVector boundOrigin;
	static float  boundRadius;
	int32 positionInSquad = 0;

private:

	
	float timeFromLastShot=0.0f;
	
	
	static constexpr const TCHAR* defaultStaticMeshName = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	FName leftSideTag = TEXT("leftLimit");
	FName rightSideTag = TEXT("rightLimit");
	FName downSideTag = TEXT("downLimit");
	

	

};
