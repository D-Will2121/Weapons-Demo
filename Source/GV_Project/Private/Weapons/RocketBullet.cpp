// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketBullet.h"
#include "GameInterface.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"


void ARocketBullet::BeginPlay()
{
	Super::BeginPlay();

	BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ARocketBullet::OnOverlapBegin);
	BulletMesh->OnComponentHit.AddDynamic(this, &ARocketBullet::OnHit);


}

void ARocketBullet::CollisionHandling(UPrimitiveComponent* HitComponent, AActor* OtherActor)
{
	IGameInterface* Interface = Cast<IGameInterface>(OtherActor);
	if (Interface) 
	{
		Interface->Execute_HurtActor(OtherActor, this, 10.f);
	}

	Destroy();
}

void ARocketBullet::Tick(float DeltaTime)
{
}

void ARocketBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ARocketBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	CollisionHandling(HitComponent,OtherActor);
}


