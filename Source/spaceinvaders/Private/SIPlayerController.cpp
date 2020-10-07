// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ASIPlayerController::ASIPlayerController()
	: m_mainCameraTag("MainCamera")
{
	m_cameras.Reserve(5);
}

void ASIPlayerController::BeginPlay() {

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), m_mainCameraTag, m_cameras);
	if (m_cameras.Num() > 0)
		this->SetViewTarget(m_cameras[0]);
}

