// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GV_PROJECT_API IGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnDeath();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void HurtActor(AActor* Caller, float Damage);

};
