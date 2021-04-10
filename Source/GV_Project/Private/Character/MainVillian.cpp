// Fill out your copyright notice in the Description page of Project Settings.


#include "MainVillian.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlasmaRifle.h"
#include "LazerGun.h"
#include "RocketLauncher.h"
#include "ShrinkRay.h"
#include "Boots.h"
#include "WeaponInterface.h"
#include "GadgetInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

// This is the main villian class, which all played characters will derive from. A lot of the basic functionality for the game is in here.


// Sets default values
AMainVillian::AMainVillian()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up player mesh component
	//Set up capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create spring arm (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create the main camera
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the spring and let the spring arm adjust to match the controller orientation
	MainCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the camera that activates on zoom
	RightCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RightCamera"));
	RightCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the spring and let the spring arm adjust to match the controller orientation
	RightCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}



// Called when the game starts or when spawned
void AMainVillian::BeginPlay()
{
	Super::BeginPlay();

	// Here we will create the weapons for the player on spawn. This is just for testing purposes.
	// Set the parameters and weapon transform of the weapons we are about to create.
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform WeaponTransform;
	WeaponTransform.SetLocation(FVector::ZeroVector);
	WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	// Set the zoom camera off by default
	if (RightCamera)
	{
		RightCamera->SetActive(false);
	}
	// First we spawn the weapon using the weapon transform from above then attach it to our players hand socket.
	if (PlasmaRifle)
	{
		Rifle = GetWorld()->SpawnActor<APlasmaRifle>(PlasmaRifle, WeaponTransform, SpawnParams);
		if (Rifle)
		{
			Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunHand"));
		}
	}
	if (LazerGun)
	{
		// Transform muzzleoffset from camera space to world space
		MuzzleLocation = PlayerLocation + FTransform(PlayerRotation).TransformVector(MuzzleOffset);
		MuzzleRotation = PlayerRotation;


		// Spawn the projectile at the muzzle
		Lazer = GetWorld()->SpawnActor<ALazerGun>(LazerGun, WeaponTransform, SpawnParams);
		if (Lazer)
		{
			Lazer->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunHand"));
		}
	}
	// Same as aboves
	if (RocketLauncher)
	{
		Launcher = GetWorld()->SpawnActor<ARocketLauncher>(RocketLauncher, WeaponTransform, SpawnParams);
		if (Launcher)
		{
			Launcher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunHand"));
		}
	}
	if (BootsClass)
	{
		Boots = GetWorld()->SpawnActor<ABoots>(BootsClass, WeaponTransform, SpawnParams);
		if (Boots)
		{
			Boots->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("BootSocket"));
		}
	}

	if (ShrinkRay)
	{
		Shrink = GetWorld()->SpawnActor<AShrinkRay>(ShrinkRay, WeaponTransform, SpawnParams);
		if (Shrink)
		{
			Shrink->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GadgetSocket"));
		}
	}
	// Set up a reference to the weapons
	BootsReference = Cast<ABoots>(Boots);
	ShrinkReference = Cast<AShrinkRay>(ShrinkRay);
	RifleReference = Cast<APlasmaRifle>(Rifle);
	LazerReference = Cast<ALazerGun>(Lazer);
	LauncherReference = Cast<ARocketLauncher>(Launcher);
	// Call the equipweapon function which will equip us with a default weapon.
	EquipWeapon();
	Gold = 1000;
	// Dash CD equals length of animation
	DashCD = 0.675f;

}

void AMainVillian::MoveForward(float Amount)
{
	if ((Controller != NULL) && (Amount != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Amount);
	}
}

void AMainVillian::MoveRight(float Amount)
{
	if ((Controller != NULL) && (Amount != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Amount);
	}
}

void AMainVillian::Turn(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainVillian::LookUp(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainVillian::Zoom()
{
	// Sets the main camera to false and the zooming one to true.
	MainCamera->SetActive(false);
	RightCamera->SetActive(true);
}

void AMainVillian::ZoomOut()
{
	// Reverts previous function
	RightCamera->SetActive(false);
	MainCamera->SetActive(true);
}


void AMainVillian::EquipWeapon()
{
	// Depending on which weapon is equipped, make it visible and hide the others from view.
	// Also sets the name of the weapon for use in blueprints.
	if (EquippedWeapon == EEquippedWeapon::LAZER)
	{
		if (LazerReference)
		{
			LazerReference->Gun->SetHiddenInGame(false);
		}
		if (RifleReference)
		{
			RifleReference->Gun->SetHiddenInGame(true);
		}
		if (LauncherReference)
		{
			LauncherReference->Gun->SetHiddenInGame(true);
		}

		WeaponName = FString(TEXT("Lazer"));
	}

	else if (EquippedWeapon == EEquippedWeapon::RIFLE)
	{
		if (LazerReference)
		{
			LazerReference->Gun->SetHiddenInGame(true);
		}
		if (RifleReference)
		{
			RifleReference->Gun->SetHiddenInGame(false);
		}
		if (LauncherReference)
		{
			LauncherReference->Gun->SetHiddenInGame(true);
		}

		WeaponName = FString(TEXT("Rifle"));

	}

	else if (EquippedWeapon == EEquippedWeapon::LAUNCHER)
	{
		if (LazerReference)
		{
			LazerReference->Gun->SetHiddenInGame(true);
		}
		if (RifleReference)
		{
			RifleReference->Gun->SetHiddenInGame(true);
		}
		if (LauncherReference)
		{
			LauncherReference->Gun->SetHiddenInGame(false);
		}

		WeaponName = FString(TEXT("Rocket"));

	}


}

void AMainVillian::SwapWeapon()
{
	// Swap weapon swaps between the equipped one and a secondary one then calls equip weapon to handle the visuals
	ChosenWeapon = SecondaryWeapon;
	SecondaryWeapon = EquippedWeapon;
	EquippedWeapon = ChosenWeapon;
	EquipWeapon();
}

void AMainVillian::Upgrade1(EEquippedWeapon UpgradedWeapon)
{
	// Not yet implemented, but will take the player's gold and pass it into a function on the weapon.
	// Depending on what they choose to upgrade, that function will upgrade it and return the gold value after the exchange.

	if (UpgradedWeapon == EEquippedWeapon::LAZER)
	{
		IWeaponInterface* LazerInterface = Cast<IWeaponInterface>(Rifle);
		if (LazerInterface)
		{
			Gold = LazerInterface->Execute_Upgrade1(Lazer, Gold);
		}
	}

	else if (UpgradedWeapon == EEquippedWeapon::RIFLE)
	{
		IWeaponInterface* RifleInterface = Cast<IWeaponInterface>(Rifle);
		if (RifleInterface)
		{
			Gold = RifleInterface->Execute_Upgrade1(Rifle, Gold);
		}

	}
}

void AMainVillian::Upgrade2(EEquippedWeapon WeaponType)
{
}


void AMainVillian::EquipGadget()
{
	// Also not implemented fully, but operates like EquipWeapon.
	if (EquippedGadget == EEquippedGadget::BOOTS)
	{
		if (BootsReference)
		{
			BootsReference->Tech->SetHiddenInGame(false);
			BootsJumpHeight = BootsReference->JumpHeight;
		}
		if (ShrinkReference)
		{
			ShrinkReference->Tech->SetHiddenInGame(false);
		}
	}

	else if (EquippedGadget == EEquippedGadget::SHRINK)
	{
		if (ShrinkReference)
		{
			ShrinkReference->Tech->SetHiddenInGame(false);
			MyCharSize = ShrinkReference->CharSize;
		}
		if (BootsReference)
		{
			BootsReference->Tech->SetHiddenInGame(false);
		}
	}
}

void AMainVillian::SwapGadget(EEquippedGadget ChosenGadget)
{
	EquippedGadget = ChosenGadget;
	EquipGadget();
}



void AMainVillian::StopFire()
{
	// Calls a function that depending on the weapon equipped, calls a function on the weapons that ceases fire.
	if (EquippedWeapon == EEquippedWeapon::LAZER)
	{
		// Checks to see if the weapon has a weaponinterface
		// If it does, call the function.
		IWeaponInterface* LazerInterface = Cast<IWeaponInterface>(Rifle);
		if (LazerInterface)
		{
			LazerInterface->Execute_EndShoot(Lazer);
		}
	}

	else if (EquippedWeapon == EEquippedWeapon::RIFLE)
	{
		IWeaponInterface* RifleInterface = Cast<IWeaponInterface>(Rifle);
		if (RifleInterface)
		{
			RifleInterface->Execute_EndShoot(Rifle);
		}
	}

	else if (EquippedWeapon == EEquippedWeapon::LAUNCHER)
	{
		IWeaponInterface* LauncherInterface = Cast<IWeaponInterface>(Launcher);
		if (LauncherInterface)
		{
			LauncherInterface->Execute_EndShoot(Launcher);
		}

	}
}

void AMainVillian::UseGadget()
{
	// Calls the functions to use the gadgets but not yet fully implemented.
	if (EquippedGadget == EEquippedGadget::BOOTS)
	{
		IGadgetInterface* BootsInterface = Cast<IGadgetInterface>(Boots);
		if (BootsInterface)
		{
			BootsInterface->Execute_UseTech(Boots);
		}
	}

	else if (EquippedGadget == EEquippedGadget::SHRINK)
	{
		IGadgetInterface* ShrinkInterface = Cast<IGadgetInterface>(Shrink);
		if (ShrinkInterface)
		{
			SetActorScale3D(MyCharSize);
			ShrinkInterface->Execute_UseTech(Shrink);
		}
	}
}

void AMainVillian::StopGadget()
{

}

void AMainVillian::Jumping()
{
	// If the boots are equipped, the player will jump higher else they'll jump at base height.
	if (EquippedGadget == EEquippedGadget::BOOTS)
	{
		LaunchCharacter(BootsJumpHeight, false, false);
	}
	else if (EquippedGadget != EEquippedGadget::BOOTS)
	{
		LaunchCharacter(BaseJumpHeight, false, false);
	}
}


// Called every frame
void AMainVillian::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void AMainVillian::Dash()
{
	// Checks to see if player is already busy doing something
	MovementComp->BrakingFrictionFactor = 0.f;
	if (!IsBusy)
	{
		IsBusy = true;
		// If not if player controller exists, check to see which key was pressed.
		// The player will dash in the direction of the last directional key pressed.
		// If for example, W was pressed the player will dash forward
		if (PlayerController)
		{
			if (PlayerController->WasInputKeyJustPressed("W"))
			{
				IsDashingW = true;
				// Launches the character forward in the x and y without moving them vertically. Edited by dash distance.
				LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * DashDistance, true, true);
			}
			else if (PlayerController->WasInputKeyJustPressed("S"))
			{
				IsDashingS = true;
				LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * (-1 * DashDistance), true, true);
			}
			else if (PlayerController->WasInputKeyJustPressed("D"))
			{
				IsDashingD = true;
				LaunchCharacter(FVector(GetActorRightVector().X, GetActorRightVector().Y, 0).GetSafeNormal() * DashDistance, true, true);
			}
			else if (PlayerController->WasInputKeyJustPressed("A"))
			{
				IsDashingA = true;
				LaunchCharacter(FVector(GetActorRightVector().X, GetActorRightVector().Y, 0).GetSafeNormal() * (-1 * DashDistance), true, true);
			}
			GetWorld()->GetTimerManager().SetTimer(DashTimer, this, &AMainVillian::ResetCD, DashCD, false);
		}
	}
	
}

void AMainVillian::ResetCD()
{
	IsDashingW = false;
	IsDashingS = false;
	IsDashingD = false;
	IsDashingA = false;
	IsBusy = false;

}

// Called to bind functionality to input
void AMainVillian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainVillian::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMainVillian::Zoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMainVillian::ZoomOut);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainVillian::StopFire);
	PlayerInputComponent->BindAction("Dash", IE_DoubleClick, this, &AMainVillian::Dash);
	PlayerInputComponent->BindAction("SwapWeapon", IE_Pressed, this, &AMainVillian::SwapWeapon);
	//PlayerInputComponent->BindAction("SwapGadget", IE_Pressed, this, &AMainVillian::SwapGadget);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainVillian::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainVillian::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainVillian::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainVillian::LookUp);

}

void AMainVillian::OnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You Died!"));
}

void AMainVillian::StartFocus_Implementation()
{
}

void AMainVillian::EndFocus_Implementation()
{
}

void AMainVillian::HurtActor_Implementation(AActor* Caller, float Damage)
{
	UGameplayStatics::ApplyDamage(this, Damage, GetController(), Caller, nullptr);
}



