// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/CollisionProfile.h"
#include "Components/BoxComponent.h"
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

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Sets default values for this actor's properties
	AInvader();

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

private:
	
	
	static constexpr const TCHAR* defaultStaticMeshName = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	FName leftSideTag = TEXT("leftLimit");
	FName rightSideTag = TEXT("rightLimit");
	FName downSideTag = TEXT("downLimit");
	

	

};
