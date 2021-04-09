// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameInterface.h"
#include "AI.generated.h"

UCLASS()
class GV_PROJECT_API AAI : public ACharacter, public IGameInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* Droid_BT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
		TSubclassOf<class ABullet> LavaPuddle;

	class ABullet* Puddle;

	bool Alive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnDeath();
	virtual void OnDeath_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndFocus();
	virtual void EndFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void HurtActor(AActor* Caller, float Damage);
	virtual void HurtActor_Implementation(AActor* Caller, float Damage);



};
