// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Templates/SubclassOf.h"
//#include "Bullet.h"
//#include "SIGameModeBase.h"

#include "SIPawn.generated.h"

UCLASS()
class SPACEINVADERS_API ASIPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float velocity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletVelocity = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isXHorizontal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class ABullet> bulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* pfxExplosion;

	
	

	UFUNCTION(BlueprintPure, Category = "Player")
		int32 GetPlayerPoints();

	UFUNCTION(BlueprintPure, Category = "Player")
		int32 GetPlayerLifes();

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// Sets default values for this pawn's properties
	ASIPawn();

protected:

	UFUNCTION(BlueprintCallable)
		void SetStaticMesh(class UStaticMesh* staticMesh = nullptr,  FString path = TEXT(""),  FVector scale = FVector(1.0f,1.0f,1.0f));
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnInvaderDestroyed(int32 id);
	void OnPlayerDestroyed();
	void OnSquadSuccessful();
	void OnSquadDissolved();
	void OnMove(float value);
	void OnFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Player")
		int32 playerPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Player")
		int32 playerLifes;

private:

	UPROPERTY()
	class ABullet* bulletTemplate; // used as template for spawning
	// Constant default values

	static constexpr const TCHAR* defaultStaticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	
};
