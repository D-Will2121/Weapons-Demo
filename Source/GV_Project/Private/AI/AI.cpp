// I am not in charge of the AI for this project so I will not include any of it here.

#include "AI.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Controller.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAI::AAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


}

// Called when the game starts or when spawned
void AAI::BeginPlay()
{
	Alive = true;
	Super::BeginPlay();

	
}

// Called every frame
void AAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void  AAI::OnDeath_Implementation()
{
	Alive = false;
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (LavaPuddle)
	{
		Puddle = GetWorld()->SpawnActor<ABullet>(LavaPuddle, GetActorTransform(), SpawnParams);
	}
	Destroy();
}

void  AAI::StartFocus_Implementation()
{
}

void  AAI::EndFocus_Implementation()
{
}

void AAI::HurtActor_Implementation(AActor* Caller, float Damage)
{
	if (Alive)
	{
		UGameplayStatics::ApplyDamage(this, Damage, GetController(), Caller, nullptr);
	}
}
