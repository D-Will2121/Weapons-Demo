// Fill out your copyright notice in the Description page of Project Settings.


#include "Gadgets.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AGadgets::AGadgets()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tech = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunBody"));
	RootComponent = Tech;

}

// Called when the game starts or when spawned
void AGadgets::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGadgets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AGadgets::Upgrade1_Implementation(int32 Gold)
{
	return int32();
}

int32 AGadgets::Upgrade2_Implementation(int32 Gold)
{
	return int32();
}

void AGadgets::UseTech_Implementation()
{
}

void AGadgets::StopTech_Implementation()
{
}

