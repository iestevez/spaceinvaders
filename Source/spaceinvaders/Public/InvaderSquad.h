// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvader.h"
#include "InvaderSquad.generated.h"



UCLASS()
class SPACEINVADERS_API AInvaderSquad : public AActor
{
	GENERATED_BODY()

public:

	AInvaderSquad();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;


public:
	//--------------------------------------------------------
	// Root Scene Component to modify location, rotation,...
	//--------------------------------------------------------
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class USceneComponent* Root;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category="Squad movement")
		float horizontalVelocity = 1000.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category="Squad movement")
		float verticalVelocity = 1000.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category="Squad movement")
		InvaderMovementType state = InvaderMovementType::STOP;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Squad movement")
		//bool isXHorizontal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad movement")
		float freeJumpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Squad Spawner")
		TSubclassOf<class AInvader> invaderClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Squad Spawner")
		class AInvader* invaderTemplate;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Squad Spawner")
		float extraSeparation;
	
	UFUNCTION(BlueprintCallable)
		void RemoveInvader(int32 ind);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	UFUNCTION(BlueprintCallable)
	void UpdateSquadState(float delta);

	UFUNCTION(BlueprintCallable)
		void SetRows(int32 nrows);

	UFUNCTION(BlueprintCallable)
		void SetCols(int32 ncols);

	UFUNCTION(BlueprintCallable)
		int32 GetRows();

	UFUNCTION(BlueprintCallable)
		int32 GetCols();

	UFUNCTION(BlueprintCallable)
		int32 GetNumberOfMembers();

protected:
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ASIGameModeBase* MyGameMode;

	
	
	UFUNCTION()
		void SquadOnLeftSide();

	UFUNCTION()
		void SquadOnRightSide();

	UFUNCTION()
		void SquadOnDownSide();

	UFUNCTION()
		void SquadFinishesDown();

	


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	InvaderMovementType previousState = InvaderMovementType::STOP;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		TArray<class AInvader*> SquadMembers;


private:
	
		int32 nRows;

		int32 nCols;

		int32 numberOfMembers;

	UPROPERTY(VisibleAnywhere)
		float timeFromLastFreeJump;

	
	// Values for initializing defaults
	static const int32 defaultNRows = 5;
	static const int32 defaultNCols = 7;
	static  constexpr const float defaultHorizontalVelocity = 1000.0f;
	static  constexpr const float defaultVerticalVelocity = 1000.0f;
	static  constexpr const float defaultExtraSeparation = 0.0f;


};
