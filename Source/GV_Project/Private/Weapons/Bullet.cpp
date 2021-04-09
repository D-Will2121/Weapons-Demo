// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameInterface.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "MusicBullet.h"
#include "Figaro.h"
#include "Engine/Engine.h"
#include <Editor\UnrealEd\Public\EditorWorldExtension.h>


// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	BulletMesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
//	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;

}


void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{




}


void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// If this bullet is a music bullet call the music bullet collision function.
	if (Type.Equals(TEXT("MusicBullet"), ESearchCase::CaseSensitive))
	{
		CollisionHandlingMusicBullet(HitComponent, OtherActor);
	}
	else
	{
		// Checks to see if the actor hit has a game interface and does damage to it if it's true.
		IGameInterface* Interface = Cast<IGameInterface>(OtherActor);
		if (Interface)
		{
			Interface->Execute_HurtActor(OtherActor, this, Damage);
		}
		Destroy();

	}

}



