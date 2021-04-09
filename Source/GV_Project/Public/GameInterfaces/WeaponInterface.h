// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GV_PROJECT_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Upgrade")
		int32 Upgrade1(int32 Gold);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Upgrade")
		int32 Upgrade2(int32 Gold);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firing")
		void Shoot(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firing")
		void EndShoot();
};
