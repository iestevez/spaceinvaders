// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"

#include "SpaceInvader.h"
#include "Bullet.generated.h"

UCLASS()
class SPACEINVADERS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY()
		BulletType bulletType = BulletType::PLAYER;

	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY()
		UStaticMeshComponent* Mesh;


	UPROPERTY()
		UBoxComponent* TriggerBox;

	UPROPERTY()
		FVector dir;

	UPROPERTY()
		float velocity;
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void setBulletMesh(const TCHAR* path = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

private:
	
	bool isXHorizontal = true;
	UStaticMesh* staticMesh;
	FName autoDestroyTags[4] = { TEXT("downLimit"),TEXT("rightLimit"),TEXT("leftLimit"),TEXT("upLimit") };

};
