// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MainVillian.h"
#include "Figaro.generated.h"

/**
 * 
 */
UCLASS()
class GV_PROJECT_API AFigaro : public AMainVillian
{
	GENERATED_BODY()
	AFigaro(const class FObjectInitializer& PCIP);

protected:

	virtual void BeginPlay();


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		TSubclassOf<class AMusicBullet> MusicBulletClass;

	FString MusicBullet;

	float SpeedUp;

	float RunSpeed;

	float Reset;

	FTimerHandle SpeedTimer;

	float BaseSpeed;

	float OldSpeedUp;

	void RunSpeedUp();

	void EndLoop();

	void RunSpeedDown();

	void Fire();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
