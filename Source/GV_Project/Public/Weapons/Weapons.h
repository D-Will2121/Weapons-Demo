// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"
#include "Weapons.generated.h"

UCLASS(config = Game)
class GV_PROJECT_API AWeapons : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapons();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float ReloadSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MaxDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageClimbRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MinDamage;
	UPROPERTY(EditAnywhere, Category = "Bullets")
		FVector MuzzleOffset;

	bool IsShooting;
	int32 Upgrade1Level;
	int32 Upgrade2Level;
	int32 Upgrade1Cost;
	int32 Upgrade2Cost;
	FRotator MuzzleRot;
	FVector MuzzleLoc;
	FVector GunLoc;
	FRotator PlayerRot;
	APawn* MyPlayer;
	FVector Velocity;
	UCameraComponent* PlayerCam;
	FActorSpawnParameters SpawnParams;
	FVector LaunchDirection;
	FString TheBullet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullets")
		TSubclassOf<class AMusicBullet> MusicBulletClass;

	class AMusicBullet* MusicBullet;

	class AMainVillian* Villian;



	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* Gun;

	UPROPERTY(EditAnywhere, Category = "Fire Point")
		USceneComponent* FirePoint;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Upgrade")
		int32 Upgrade1(int32 Gold);
	virtual int32 Upgrade1_Implementation(int32 Gold);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Upgrade")
		int32 Upgrade2(int32 Gold);
	virtual int32 Upgrade2_Implementation(int32 Gold);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firing")
		void Shoot(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet);
		virtual void Shoot_Implementation(AActor* Player, UCameraComponent* Camera, FVector MuzzleLocation, FRotator MuzzleRotation, const FString& Bullet);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firing")
		void EndShoot();
		virtual void EndShoot_Implementation();
};
