// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Templates/SubclassOf.h"
#include "SIGameModeBase.h"

#include "SIPawn.generated.h"

// Space Invader Pawn Class

// Delegates of this game:
//DECLARE_DELEGATE(FStandardDelegateSignature)
//DECLARE_MULTICAST_DELEGATE_OneParam(FOneParamMulticastDelegateSignature, int32);
//DECLARE_DELEGATE_OneParam(FOneParamDelegateSignature, int32)

UCLASS()
class SPACEINVADERS_API ASIPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	// Points per invader
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pointsPerInvader;

	// Points per squad
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pointsPerSquad;

	// References to Involved delegates
	FStandardDelegateSignature* SquadDissolved;
	FStandardDelegateSignature* SquadSuccessful;
	FOneParamMulticastDelegateSignature* InvaderDestroyed;

	FOneParamDelegateSignature* NewSquad;
	FStandardDelegateSignature* PlayerZeroLifes;

	// Velocity of the pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float velocity = 1000.0f;

	// Velocity of the player bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletVelocity = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isXHorizontal = true;


	// Bullets
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class ABullet> bulletClass;

	// Explosion effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* PFXExplosion;

	



	// Getters

	UFUNCTION(BlueprintPure, Category = "Player")
		int32 GetPlayerPoints();

	UFUNCTION(BlueprintPure, Category = "Player")
		int32 GetPlayerLifes();


	// Overlaps
	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// Sets default values for this pawn's properties
	ASIPawn();


	// Bind to delegate
	void OnInvaderDestroyed(int32 id);

	// Bind to delegate
	void OnSquadSuccessful();
	// Bind to delegate:
	void OnSquadDissolved();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Timer to contro waiting after destruction
	FTimerHandle timerHandle;

	//Audios
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AudioShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AudioExplosion;


protected:

	// It could be possible to change the static mesh component during run time.
	UFUNCTION(BlueprintCallable)
		void SetStaticMesh(class UStaticMesh* staticMesh = nullptr,  FString path = TEXT(""),  FVector scale = FVector(1.0f,1.0f,1.0f));

	// After the pawn is destroyed, waits and then all this happen.
	UFUNCTION(BlueprintCallable)
		void PostPlayerDestroyed();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void OnPlayerDestroyed();
	

	void OnMove(float value);
	
	void OnFire();
	
	void OnPause();

	

private:

	// Audio component
	UPROPERTY()
		class UAudioComponent* AudioComponent;

	
	UPROPERTY() //This is necessary to control the reference counter of the ABullet and avoid garbage collector action
		class ABullet* bulletTemplate; // used as template for spawning
	// Constant default values
		
	int32 playerPoints;
		
	int32 playerLifes;

	// To set a frozen state (no moving and firing capabilities)
	bool bFrozen;

	//To pause the Game
	bool bPause;

	static constexpr const TCHAR* defaultStaticMeshPath = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	
};
