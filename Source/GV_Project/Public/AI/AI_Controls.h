// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AI_Controls.generated.h"

/**
 *
 */
UCLASS()
class GV_PROJECT_API AAI_Controls : public AAIController
{
	GENERATED_BODY()
public:
	//void BeginPlay() override;

	AAI_Controls(FObjectInitializer const& object_initializer = FObjectInitializer::Get());

	void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	//UAISenseConfig_Sight* SightConfig;

	FVector LastKnownPlayerPosition;
	FVector PlayerPosition;
	bool bCanSeePlayer;
	bool memory;
	AActor* Actor;
	FTimerHandle ticker;
	int MaxAged;

	UFUNCTION()
		void OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class AActor* Player;

private:

	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BehaviorTree;

	void CoolDown();
};