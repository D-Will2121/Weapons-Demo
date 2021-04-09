// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "RocketBullet.h"
#include "Kismet/GameplayStatics.h"
#include "kismet/KismetMathLibrary.h"
#include "MusicBullet.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"

	// Rocket Launcher, still in development.

ARocketLauncher::ARocketLauncher(const class FObjectInitializer& PCIP)
{
	Start = GetActorLocation();
	Damage = 100;
	FireRate = 1.0f;
	MaxAmmo = 6;
	Range = 15000;
	ReloadSpeed = 2.0f;
}

void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();

	Ammo = MaxAmmo;

}


void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ARocketLauncher::Upgrade1_Implementation(int32 Gold)
{
	return Gold;

}

int32 ARocketLauncher::Upgrade2_Implementation(int32 Gold)
{
	return Gold;
}

void ARocketLauncher::Shoot_Implementation(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet)
{
	
}

void ARocketLauncher::EndShoot_Implementation()
{
}

void ARocketLauncher::ShootFunction()
{
	
}

void ARocketLauncher::SpawnBullet()
{
	
}

void ARocketLauncher::Reload()
{
}



