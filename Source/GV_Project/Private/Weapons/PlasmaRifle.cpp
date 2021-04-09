// Fill out your copyright notice in the Description page of Project Settings.


#include "PlasmaRifle.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "kismet/KismetMathLibrary.h"
#include "MusicBullet.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "MainVillian.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"


APlasmaRifle::APlasmaRifle(const class FObjectInitializer& PCIP)
{
	Start = GetActorLocation();
	Damage = 15;
	FireRate = 0.2;
	MaxAmmo = 30;
	Range = 10000;
	ReloadSpeed = 1.0f;
}

void APlasmaRifle::BeginPlay()
{
	Super::BeginPlay();

	Ammo = MaxAmmo;
	Upgrade1Level = 1;
	Upgrade1Cost = 500;
}


void APlasmaRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 APlasmaRifle::Upgrade1_Implementation(int32 Gold)
{
	//For weapon upgrades. Not complete.
	return Gold;

}

int32 APlasmaRifle::Upgrade2_Implementation(int32 Gold)
{
	return Gold;
}

void APlasmaRifle::Shoot_Implementation(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet)
{
	// Matches the parameters with local variables in this class.
	// Casts Villian to the player actor passed into the function
	MuzzleRot = MuzzleRotation;
	TheBullet = Bullet;
	MyPlayer = UGameplayStatics::GetPlayerPawn(Player, 0);
	Villian = Cast<AMainVillian>(MyPlayer);
	IsShooting = true;
	PlayerCam = Camera;
	// Creates a looping timer to spawn bullets repeatedly
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &APlasmaRifle::ShootFunction, FireRate, true);
}

void APlasmaRifle::EndShoot_Implementation()
{
	// Fires one bullet then stops firing.
	if (Ammo > 0)
	{
		SpawnBullet();
	}
	IsShooting = false;
}

void APlasmaRifle::ShootFunction()
{
	// Check to see if player is busy and makes them busy when shooting begins
	if (!Villian->IsBusy)
	{
		Villian->IsBusy = true;
		// Checks to see if they are already shooting and that they have enough ammo then calls the SpawnBullet Function
		if (IsShooting == true)
		{
			if (Ammo > 0)
			{
				SpawnBullet();
			}
			else
			{
				//If no ammo is left, clear the bullet spawn timer and start the reload animation/reload timer
				GetWorld()->GetTimerManager().ClearTimer(Timer);
				Villian->ReloadingAnimation = true;
				GetWorld()->GetTimerManager().SetTimer(Reloading, this, &APlasmaRifle::Reload, ReloadSpeed, false);
			}
		}
		else
		{
			//If we're no longer shooting clear the timer as well.
			GetWorld()->GetTimerManager().ClearTimer(Timer);
		}
	}
	// Shooting is done, player is no longer busy
	Villian->IsBusy = false;
}

void APlasmaRifle::SpawnBullet()
{
	// Set the spawn location/rotation etc. for the raycast
	GetActorEyesViewPoint(GunLoc, MuzzleRot);
	Start = PlayerCam->GetComponentLocation();
	ForwardVector = PlayerCam->GetForwardVector();
	End = (Start + (ForwardVector * Range));
	// Create a raycast from these parameters
	isHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	SpawnParams.Owner = this;
	if (isHit)
	{
		if (Hit.bBlockingHit)
		{
			// If the raycast exists and hits anything find where the player is looking.

			//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 4, 2, FColor::Red, false, -1, 0, 10);
			PlayerRot = UKismetMathLibrary::FindLookAtRotation(Gun->GetSocketLocation("b_gun_muzzleflash"), Hit.ImpactPoint);

			// Checks to see that the bullet being spawned is of Music Bullet class (This is useful because down the line there will
			// be a variety of different characters each with a unique type of bullet.

			if (TheBullet.Equals(TEXT("MusicBullet"), ESearchCase::CaseSensitive))
			{
				if (MusicBulletClass)
				{
					// If the bullet's class exists spawn it using the socket location and player's rotation we just calculated
					// then set its velocity, damage and give it a reference to the player who fired it.
					MusicBullet = GetWorld()->SpawnActor<AMusicBullet>(MusicBulletClass, Gun->GetSocketLocation("b_gun_muzzleflash"),
						PlayerRot, SpawnParams);
					Ammo -= 1;
					Velocity = MyPlayer->GetActorForwardVector();
					MusicBullet->Velocity = FVector(Velocity);
					MusicBullet->Damage = Damage;
					MusicBullet->BulletOwner = MyPlayer;
					MusicBullet->Type = TheBullet;
				}

			}
		}
	}
	else
	{
		// If the raycast doesn't hit anything just shoot the bullet forward.
		PlayerRot = MyPlayer->GetControlRotation();
		if (TheBullet.Equals(TEXT("MusicBullet"), ESearchCase::CaseSensitive))
		{
			if (MusicBulletClass)
			{
				MusicBullet = GetWorld()->SpawnActor<AMusicBullet>(MusicBulletClass, Gun->GetSocketLocation("b_gun_muzzleflash"), PlayerRot, SpawnParams);
				Velocity = MyPlayer->GetActorForwardVector();
				MusicBullet->Velocity = FVector(Velocity);
				MusicBullet->Damage = Damage;
				MusicBullet->BulletOwner = MyPlayer;
				MusicBullet->Type = TheBullet;
			}

		}
	}
}

void APlasmaRifle::Reload()
{
	// When called, stops the reloadin animation and sets ammo to max ammo.
	Ammo = MaxAmmo;
	Villian->ReloadingAnimation = false;
}



