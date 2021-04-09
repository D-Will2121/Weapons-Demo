// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"
#include "Camera/CameraComponent.h"
#include "MusicBullet.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AWeapons::AWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunBody"));
	RootComponent = Gun;

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Fire Point"));

	MusicBullet = CreateDefaultSubobject<AMusicBullet>(TEXT("Music Bullet"));
}

// Called when the game starts or when spawned
void AWeapons::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AWeapons::Upgrade1_Implementation(int32 Gold)
{
	return Gold;
}

int32 AWeapons::Upgrade2_Implementation(int32 Gold)
{
	return Gold;
}

void AWeapons::Shoot_Implementation(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet)
{

}

void AWeapons::EndShoot_Implementation()
{
}


