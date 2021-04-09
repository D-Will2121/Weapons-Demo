// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GadgetInterface.h"
#include "Gadgets.generated.h"

UCLASS()
class GV_PROJECT_API AGadgets : public AActor, public IGadgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGadgets();

	bool IsUsing;
	int32 Upgrade1Level;
	int32 Upgrade2Level;
	int32 Upgrade1Cost;
	int32 Upgrade2Cost;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* Tech;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Upgrade")
		int32 Upgrade1(int32 Gold);
	virtual int32 Upgrade1_Implementation(int32 Gold);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Upgrade")
		int32 Upgrade2(int32 Gold);
	virtual int32 Upgrade2_Implementation(int32 Gold);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tech")
		void UseTech();
	virtual void UseTech_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tech")
		void StopTech();
	virtual void StopTech_Implementation();


};
