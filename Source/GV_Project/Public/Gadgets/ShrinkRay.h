// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gadgets/Gadgets.h"
#include "ShrinkRay.generated.h"

/**
 *
 */
UCLASS()
class GV_PROJECT_API AShrinkRay : public AGadgets
{
	GENERATED_BODY()
		AShrinkRay(const class FObjectInitializer& PCIP);

public:

	int32 Upgrade1(int32 Gold);
	virtual int32 Upgrade1_Implementation(int32 Gold);

	int32 Upgrade2(int32 Gold);
	virtual int32 Upgrade2_Implementation(int32 Gold);

	void UseTech();
	virtual void UseTech_Implementation();

	void StopTech();
	virtual void StopTech_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shrink Ray")
		FVector CharSize;
};
