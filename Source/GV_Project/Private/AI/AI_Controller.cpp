// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "AI.h"


AAI_Controller::AAI_Controller() 
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAI_Controller::BeginPlay() 
{
	Super::BeginPlay();

	
}

void AAI_Controller::OnPossess(APawn* Pawn) 
{
	Super::OnPossess(Pawn);

	
}

void AAI_Controller::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);


}

FRotator AAI_Controller::GetControlRotation() const 
{
	FRotator rotate;
	return rotate;
}

void AAI_Controller::OnPawnDetected(const TArray<AActor*> &DetectedPawns) 
{
	
}

