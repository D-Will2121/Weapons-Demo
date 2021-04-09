#include "AI_Controls.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"
#include "MainVillian.h"
#include "UObject/ConstructorHelpers.h"


AAI_Controls::AAI_Controls(FObjectInitializer const& object_initializer)
	: Super(object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;
	memory = false;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	MaxAged = AISightAge;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controls::OnTargetPerceptionUpdate);

	CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
}


void AAI_Controls::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (BehaviorTree != nullptr) {
		RunBehaviorTree(BehaviorTree);
	}
}

void AAI_Controls::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MyCharacter = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	PlayerPosition = MyCharacter;


}

FRotator AAI_Controls::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAI_Controls::OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{

	if (Actor->IsA(AMainVillian::StaticClass()))
	{
		Player = Actor;
		PlayerPosition = Actor->GetActorLocation();
		LastKnownPlayerPosition = Stimulus.StimulusLocation;
		bCanSeePlayer = Stimulus.WasSuccessfullySensed();
		if (bCanSeePlayer) {
			UE_LOG(LogTemp, Warning, TEXT("Target: I See u"));
			AISightAge = MaxAged;
			memory = true;
		}
		else {
			CoolDown();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Target: I See u"));

	}
	/*if (Cast<AMainVillian>(Actor)) {
		LastKnownPlayerPosition = Stimulus.StimulusLocation;
		bCanSeePlayer = Stimulus.WasSuccessfullySensed();
		UE_LOG(LogTemp, Warning, TEXT("Target: I See u"));
	}*/
}

void AAI_Controls::CoolDown()
{
	if (AISightAge <= 0) {
		memory = false;
	}
	else
	{
		--AISightAge;
		GetWorld()->GetTimerManager().SetTimer(ticker, this, &AAI_Controls::CoolDown, 1.0f, true);
		//CoolDown();
	}
}

