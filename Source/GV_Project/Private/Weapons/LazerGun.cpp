// Fill out your copyright notice in the Description page of Project Settings.


#include "LazerGun.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "MusicBullet.h"
#include "MainVillian.h"
#include "Figaro.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"
#include "AI.h"

// See PlasmaRifle.cpp for more details on this class since a lot of it is very similar

ALazerGun::ALazerGun(const class FObjectInitializer& PCIP)
{
	Start = GetActorLocation();
	MaxDamage = 40;
	MinDamage = 10;
	DamageClimbRate = 4;
	FireRate = 0.2;
	MaxAmmo = 30;
	Range = 1000;
	ReloadSpeed = 1.0f;

	// Create a default lazer particle
	LazerParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle Beam"));


}

void ALazerGun::BeginPlay()
{
	Super::BeginPlay();
	LazerParticle->SetActive(true);
	Ammo = MaxAmmo;
	Damage = MinDamage;

}


int32 ALazerGun::Upgrade1_Implementation(int32 Gold)
{
	return Gold;
}


int32 ALazerGun::Upgrade2_Implementation(int32 Gold)
{
	return Gold;
}

void ALazerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsShooting)
	{
		ShootLazer();
	}

}

void ALazerGun::Shoot_Implementation(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet)
{
	MuzzleRot = MuzzleRotation;
	TheBullet = Bullet;
	PlayerCam = Camera;
	MyPlayer = UGameplayStatics::GetPlayerPawn(Player, 0);
	Villian = Cast<AMainVillian>(MyPlayer);
	IsShooting = true;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ALazerGun::ShootFunction, FireRate, true);
}

void ALazerGun::EndShoot_Implementation()
{
	LazerParticle->SetActive(false);
	if (Ammo > 0)
	{
		ShootFunction();
	}
	IsShooting = false;
}

void ALazerGun::ShootFunction()
{
	if (!Villian->IsBusy)
	{
		Villian->IsBusy = true;
		if (IsShooting == true)
		{
			if (Ammo > 0)
			{
				Ammo -= 1;
				ShootLazer();
				if (isHit)
				{
					if (Hit.bBlockingHit)
					{
						// Find the interface of the hit actor.
						IGameInterface* Interface = Cast<IGameInterface>(Hit.GetActor());
						if (Interface)
						{
							// Increase the damage of the lazer over time, but not higher than the max damage.
							if (Damage < MaxDamage)
							{
								Damage += 1;
								if (Damage > MaxDamage)
								{
									Damage = MaxDamage;
								}
							}
							// If this is Figaro firing this lazer, we need to trigger his passive as we deal damage. See Figaro.cpp
							if (TheBullet.Equals(TEXT("MusicBullet"), ESearchCase::CaseSensitive))
							{
								AFigaro* Figaro = Cast<AFigaro>(MyPlayer);

								// The character Figaro has a special ability where he gains movement speed as he does damage
								// So if his max speed has exceeded base speed + 300 then cap it out, else increase it by the damage this bullet dealt.
								if (Figaro->SpeedUp + Damage > 300)
								{
									Figaro->SpeedUp = 300;
								}
								else
								{
									Figaro->SpeedUp += Damage;
								}
								//Call a couple of functions that can be viewed in Figaro.cpp
								Figaro->EndLoop();
								Figaro->RunSpeedUp();
								// Deal damage to the actor hit based on this bullet's damage. This calls a function from the interface.
							}
							Interface->Execute_HurtActor(Hit.GetActor(), this, Damage);
						}
					}
				}
			}
			else
			{
				// Once we run out of ammo, reset the lazer's damage and start reloading
				Damage = MinDamage;
				GetWorld()->GetTimerManager().ClearTimer(Timer);
				EndShoot_Implementation();
				Villian->ReloadingAnimation = true;
				GetWorld()->GetTimerManager().SetTimer(Reloading, this, &ALazerGun::Reload_Implementation, ReloadSpeed, false);
			}
		}
		else
		{
			// If we're busy doing something else reset the damage and do nothing.
			EndShoot_Implementation();
			GetWorld()->GetTimerManager().ClearTimer(Timer);
			Damage = MinDamage;
		}
		Villian->IsBusy = false;
	}
}

void ALazerGun::ShootLazer()
{
	//Spawn settings
	Start = PlayerCam->GetComponentLocation();
	ForwardVector = PlayerCam->GetForwardVector();
	End = (Start + (ForwardVector * Range));
	isHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	SpawnParams.Owner = this;
	LazerStart = Gun->GetSocketLocation("b_gun_muzzlepoint");
	if (isHit)
	{
		// Set the lazers endpoint.
		LazerEnd = Hit.ImpactPoint;
	}
	// Set lazer endpoint to max range if it hits nothing
	else
	{
		LazerEnd = End;
	}
	// Set the beams source point and end point then set it to active.
	LazerParticle->SetBeamSourcePoint(0, LazerStart, 0);
	LazerParticle->SetBeamTargetPoint(0, LazerEnd, 0);
	LazerParticle->SetActive(true);



}

void ALazerGun::Reload_Implementation()
{
	Ammo = MaxAmmo;
	Villian->ReloadingAnimation = false;
	Villian->IsBusy = false;
}

