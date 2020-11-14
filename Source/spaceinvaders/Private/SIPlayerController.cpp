// Fill out your copyright notice in the Description page of Project Settings.


#include "SIPlayerController.h"


ASIPlayerController::ASIPlayerController()
	: m_mainCameraTag("MainCamera")
{
	m_cameras.Reserve(5);
}

void ASIPlayerController::BeginPlay() {
	// Set the camera tagged as "MainCamera" as the view target.
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), m_mainCameraTag, m_cameras);
	if (m_cameras.Num() > 0)
		this->SetViewTarget(m_cameras[0]);
}

