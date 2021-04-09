// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicBullet.h"
#include "GameInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Figaro.h"
#include "Engine/Engine.h"

// Some of this functionality is on the class Bullet.cpp

void ABullet::CollisionHandlingMusicBullet(UPrimitiveComponent* HitComponent, AActor* OtherActor)
{
	IGameInterface* Interface = Cast<IGameInterface>(OtherActor);
	if (Interface)
	{
		// When we hit a target, check to see if it has our GameInterface Interface that would imply that it is
		// either an AI or a player that is taking damage.

		// Cast the owner of the bullet to the character class that creates this particular bullet, Figaro.
		AFigaro* Figaro = Cast<AFigaro>(BulletOwner);

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
		Interface->Execute_HurtActor(OtherActor, this, Damage);
	}

	// After colliding with something, destroy the bullet.
	Destroy();
}

