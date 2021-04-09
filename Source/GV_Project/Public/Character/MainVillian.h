// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameInterface.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainVillian.generated.h"


UENUM()
enum class EEquippedWeapon : uint8
{
	LAZER,
	RIFLE,
	LAUNCHER
};

UENUM()
enum class EEquippedGadget : uint8
{
	BOOTS,
	SHRINK
};

/*UENUM()
enum class States : uint8
{
	Freeze,
	Burn
};

void  SetValue(States Buff, float amount)
{
	Buff += amount;
	applyeffects(Buff);
	{
		check to see what the value of the buff, and if it is beyond a threshold call the timer function.

	}
}*/
/*USTRUCT(BlueprintType)
struct FPlayerWeapons
{
	GENERATED_BODY()

	class ADartGun* Pistol;
	class APlasmaRifle* Rifle;
	class ABeeSwarmLauncher* Launcher;
};*/



class USkeletalMeshComponent;

UCLASS(config = Game)
class GV_PROJECT_API AMainVillian : public ACharacter, public IGameInterface
{
	GENERATED_BODY()

	


public:
	// Sets default values for this character's properties
	AMainVillian();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	//Main Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* MainCamera;

	//Camera for Zooming in
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* RightCamera;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float LookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float Sensitivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FVector BaseJumpHeight;

	// Gun muzzle's offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FVector MuzzleOffset;

	FVector RayStart;

	FRotator PlayerRotation;
	FRotator MuzzleRotation;
	FVector PlayerLocation;
	FVector MuzzleLocation;
	FVector LaunchDirection;

	AActor* HitActor;

	USceneComponent* BulletSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		EEquippedWeapon EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
		EEquippedWeapon SecondaryWeapon;

	EEquippedWeapon ChosenWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gadgets")
		EEquippedGadget EquippedGadget;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
		//EEquippedWeapon WeaponType;

	UFUNCTION(BlueprintCallable)
		void Upgrade1(EEquippedWeapon UpgradedWeapon);

	UFUNCTION(BlueprintCallable)
		void Upgrade2(EEquippedWeapon UpgradedWeapon);

	//	FPlayerWeapons PlayerWeapons;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//set up character mesh
	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* PistolMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		FTransform GunPoint;

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* RifleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		TSubclassOf<class APlasmaRifle> PlasmaRifle;

	class APlasmaRifle* Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		TSubclassOf<class ALazerGun> LazerGun;

	class ALazerGun* Lazer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		TSubclassOf<class ARocketLauncher> RocketLauncher;

	class ARocketLauncher* Launcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gadget")
		TSubclassOf<class ABoots> BootsClass;

	UPROPERTY(EditAnywhere, Category = "Gadgets")
		FTransform BootsTransform;

	class ABoots* Boots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gadget")
		TSubclassOf<class AShrinkRay> ShrinkRay;

	UPROPERTY(EditAnywhere, Category = "Gadgets")
		FTransform ShrinkTransform;	

	UPROPERTY(EditAnywhere, Category = "Stats")
		float DashDistance;

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
		bool IsDashingW;

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
		bool IsDashingS;

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
		bool IsDashingA;

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
		bool IsDashingD;

	UPROPERTY(BlueprintReadOnly)
		FString WeaponName;


	class AShrinkRay* Shrink;

	APlasmaRifle* RifleReference;

	ALazerGun* LazerReference;

	ARocketLauncher* LauncherReference;

	ABoots* BootsReference;

	AShrinkRay* ShrinkReference;

	FVector BootsJumpHeight;
	
	FVector MyCharSize;

	UCharacterMovementComponent* MovementComp;

	APlayerController* PlayerController;

	FTimerHandle DashTimer;

	float DashCD;
	//class ADartGun* Pistol;

	//UProperty()

	//Moving forward/backwards function and turning functions

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SwapWeapon();

	UFUNCTION(BlueprintCallable, Category = "Gadget")
		void SwapGadget(EEquippedGadget ChosenGadget);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void Dash();


	void MoveForward(float Amount);

	void MoveRight(float Amount);

	void Turn(float Rate);

	void LookUp(float Rate);

	void Zoom();

	void ZoomOut();

	void EquipWeapon();

	void EquipGadget();

	void StopFire();

	void UseGadget();

	void StopGadget();

	void Jumping();


	void ResetCD();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
		bool ReloadingAnimation;

	bool IsBusy;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnDeath();
		virtual void OnDeath_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndFocus();
	virtual void EndFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void HurtActor(AActor* Caller, float Damage);
	virtual void HurtActor_Implementation(AActor* Caller, float Damage);




};
