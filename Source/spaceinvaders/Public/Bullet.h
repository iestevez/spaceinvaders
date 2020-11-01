// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "SpaceInvader.h"
#include "Bullet.generated.h"

UCLASS()
class SPACEINVADERS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
		BulletType bulletType = BulletType::PLAYER;

	UPROPERTY()
		class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;


	UPROPERTY()
		class UBoxComponent* TriggerBox;

	UPROPERTY()
		FVector dir;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
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

	UFUNCTION(BlueprintCallable)
	void SetBulletMesh(class UStaticMesh* staticMesh=nullptr,  FString path = TEXT(""),  FVector scale = FVector(1.0f,1.0f,1.0f));

private:
	
	bool isXHorizontal = true;
	
	FName autoDestroyTags[4] = { TEXT("downLimit"),TEXT("rightLimit"),TEXT("leftLimit"),TEXT("upperLimit") };

	static constexpr const TCHAR* defaultStaticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

};
