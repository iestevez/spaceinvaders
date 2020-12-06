// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Kismet/GameplayStatics.h"
//#include "Components/BoxComponent.h"

//#include "SpaceInvader.h"
//#include "InvaderMovementComponent.h"
#include "Invader.generated.h"

UCLASS()
class SPACEINVADERS_API AInvader : public AActor
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		class UInvaderMovementComponent* Movement;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class UBoxComponent* TriggerBox;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* PFXExplosion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float fireRate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float bulletVelocity = 3000.0f;

	
	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
		float GetBoundRadius();

	UFUNCTION(BlueprintCallable)
		FVector GetBoundOrigin();


	// Sets default values for this actor's properties
	AInvader();

	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void SetPositionInSquad(int32 index);

	UFUNCTION(BlueprintCallable)
		int32 GetPositionInSquad();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//-------------------------------------------------------------------------------------
	// Change static mesh of Mesh component
	// @param staticMesh (UStaticMesh*,default=nullptr) Pointer to UStaticMesh
	// @param path (FString, default=TEXT("")) Asset path
	// ------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void SetInvaderMesh(class UStaticMesh* staticMesh=nullptr, const FString path= TEXT(""), FVector scale=FVector(1.0f,1.0f,1.0f));

	//Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AudioShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AudioExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AudioJet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class ABullet> bulletClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		class ABullet* bulletTemplate;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		//class  UStaticMesh* staticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector boundOrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float  boundRadius;

	

private:

	UPROPERTY()
		class UAudioComponent* AudioComponent;

	UPROPERTY()
		class UAudioComponent* AudioComponentJet;

	UPROPERTY()
		class UInvaderMovementComponent* InvaderMovementComponent;

	UPROPERTY(VisibleAnywhere)
		int32 positionInSquad = 0;
	
	UPROPERTY(VisibleInstanceOnly)
		float timeFromLastShot=0.0f;
	
	UPROPERTY(VisibleAnywhere)
		FName leftSideTag; 
	
	UPROPERTY(VisibleAnywhere)
		FName rightSideTag; 
	
	UPROPERTY(VisibleAnywhere)
		FName downSideTag; 

	UFUNCTION()
		void InvaderDestroyed();

	UFUNCTION()
		void PostInvaderDestroyed();

	// Timer to contro waiting after destruction
	FTimerHandle timerHandle;

	bool bFrozen;

	
	// Static literals of the class

	static constexpr const TCHAR* defaultStaticMeshName = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	static constexpr const TCHAR* leftSideTagString = TEXT("leftLimit");
	static constexpr const TCHAR* rightSideTagString = TEXT("rightLimit");
	static constexpr const TCHAR* downSideTagString = TEXT("downLimit");
	

	

};
