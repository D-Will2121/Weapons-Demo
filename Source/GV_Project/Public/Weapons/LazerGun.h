// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapons.h"
#include "WeaponInterface.h"
#include "TimerManager.h"
#include "LazerGun.generated.h"

/**
 *
 */
UCLASS()
class GV_PROJECT_API ALazerGun : public AWeapons
{
	GENERATED_BODY()
		ALazerGun(const class FObjectInitializer& PCIP);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		UParticleSystemComponent* LazerParticle;

	FVector LazerStart;

	FVector LazerEnd;

	int32 Upgrade1(int32 Gold);
	virtual int32 Upgrade1_Implementation(int32 Gold);

	int32 Upgrade2(int32 Gold);
	virtual int32 Upgrade2_Implementation(int32 Gold);

	void Shoot(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet);
	virtual void Shoot_Implementation(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet);

	void EndShoot();
	virtual void EndShoot_Implementation();

	TSubclassOf<class AAI> EnemyAI;

	bool isHit;
	FTimerHandle Timer;
	FTimerHandle Reloading;
	UCameraComponent* Camera;
	FHitResult Hit;
	FRotator Rotation;
	FVector Start;
	FVector ForwardVector;
	FVector End;
	FCollisionQueryParams TraceParams;

	void ShootFunction();
	void ShootLazer();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reloading")
		void Reload();
	virtual void Reload_Implementation();


};

