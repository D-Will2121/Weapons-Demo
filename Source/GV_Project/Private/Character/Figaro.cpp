// Fill out your copyright notice in the Description page of Project Settings.


#include "Figaro.h"
#include "MusicBullet.h"
#include "Camera/CameraComponent.h"
#include "WeaponInterface.h"
#include "PlasmaRifle.h"
#include "TimerManager.h"
#include "LazerGun.h"
#include "RocketLauncher.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// This is a character derived from MainVillain.cpp

AFigaro::AFigaro(const class FObjectInitializer& PCIP)
{

}

void AFigaro::BeginPlay()
{
	Super::BeginPlay();

	// Sets some base values that will be used later
	MusicBullet = FString(TEXT("MusicBullet"));
	MovementComp = GetCharacterMovement();
	BaseSpeed = MovementComp->MaxWalkSpeed;
}

void AFigaro::Fire()
{
	// Takes the muzzle rotation and camera and puts them in a function for shooting.

	// Get the camera transform
	GetActorEyesViewPoint(PlayerLocation, PlayerRotation);

	// Transform muzzleoffset from camera space to world space
	MuzzleLocation = PlayerLocation + FTransform(PlayerRotation).TransformVector(MuzzleOffset);
	MuzzleRotation = PlayerRotation;
	LaunchDirection = MuzzleRotation.Vector();


	if (EquippedWeapon == EEquippedWeapon::LAZER)
	{
		// Checks to see if the lazer (derived from AMainVillian) has a weapon interface.
		// If it does, call the shoot function and pass in a set of parameters.
		// The MusicBullet parameter is used to tell the system what type of bullet to fire.
		IWeaponInterface* LazerInterface = Cast<IWeaponInterface>(Lazer);
		if (LazerInterface)
		{
			LazerInterface->Execute_Shoot(Lazer, this, MainCamera, MuzzleLocation, MuzzleRotation, MusicBullet);

		}
	}

	else if (EquippedWeapon == EEquippedWeapon::RIFLE)
	{
		IWeaponInterface* RifleInterface = Cast<IWeaponInterface>(Rifle);
		if (RifleInterface)
		{
			RifleInterface->Execute_Shoot(Rifle, this, MainCamera, MuzzleLocation, MuzzleRotation, MusicBullet);
		}
	}

	else if (EquippedWeapon == EEquippedWeapon::LAUNCHER)
	{
		IWeaponInterface* LauncherInterface = Cast<IWeaponInterface>(Launcher);
		if (LauncherInterface)
		{
			LauncherInterface->Execute_Shoot(Launcher, this, MainCamera, MuzzleLocation, MuzzleRotation, MusicBullet);
		}
	}
}

void AFigaro::RunSpeedUp()
{
	// Figaro's passive that increases his movement speed as he deals damage.
	// Set his runspeed is equal to his base speed, store his old speedup and make his walk speed = his run speed.
	// Then call a timer to decrease this over time
	RunSpeed = BaseSpeed + SpeedUp;
	OldSpeedUp = SpeedUp;
	MovementComp->MaxWalkSpeed = RunSpeed;
	GetWorld()->GetTimerManager().SetTimer(SpeedTimer, this, &AFigaro::RunSpeedDown, 0.5f, true);
}

void AFigaro::RunSpeedDown()
{
	// If we don't have any current speed boost, just end the loop.
	if (SpeedUp <= 0)
	{
		EndLoop();
	}

	// If we have less than 10 speed boost, just reset it. No negatives.
	// Else decrease the speed boost by 10.

	else if (SpeedUp < 10)
	{
		SpeedUp = 0;
	}

	else
	{
		SpeedUp -= 10;
	}

	// If we've slowed down we need to show that so if our oldspeed is greater than our current speed up
	// Our runspeed is subtracted by the difference between old speed and speedup.
	// This will make our run speed equal to our current speedup value. Also set Oldspeed to this new updated speed.
	// Change our walk speed to this runspeed again.
	if (OldSpeedUp > SpeedUp)
	{
		RunSpeed -= OldSpeedUp - SpeedUp;
		OldSpeedUp = SpeedUp;
		MovementComp->MaxWalkSpeed = RunSpeed;
	}

}

void AFigaro::EndLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(SpeedTimer);

}

void AFigaro::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFigaro::Fire);

}