// Fill out your copyright notice in the Description page of Project Settings.


#include "VBlade.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FCTween.h"
#include "VFruit.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AVBlade::AVBlade()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	State = EBladeState::Previewing;
}

// Called when the game starts or when spawned
void AVBlade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVBlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AVBlade::Cut()
{
	State = EBladeState::Cutting;
	
	CapsuleComponent->SetWorldLocation(StartCutLocation, false, nullptr, ETeleportType::ResetPhysics);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NiagaraComponent->SetVisibility(true);
	NiagaraComponent->ReinitializeSystem();

	
	FCTween::Play(
	GetActorLocation(),
	EndCutLocation,
	[&](FVector t)
	{
		SetActorLocation(t);
	},
	.1f,
	EFCEase::Linear)->SetOnComplete([&]() { 
		State = EBladeState::Done;
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	});

	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	bool Success = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), StartCutLocation, EndCutLocation, .1f,
		44.0f, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);

	//TODO: Research reason for the bug
	TArray<AActor*> CuttedFruits;
	
	if (Success)
	{
		for(FHitResult Hit:OutHits)
		{
			if(CuttedFruits.Contains(Hit.GetActor()))
			{
				continue;
			} else
			{
				CuttedFruits.Add(Hit.GetActor());
			}
			
			if(State == EBladeState::Cutting)
			{
				AVFruit* Fruit = Cast<AVFruit>(Hit.GetActor());
				if(Fruit)
				{
					AVFruit* NewFruit = Fruit->Clone();
					NewFruit->Cut(Direction, Hit.Location, true);
					Fruit->Cut(Direction, Hit.Location, false);
					
					FRotator DirectionRotation = UKismetMathLibrary::MakeRotFromX(Direction);
					FRotator EffectsRotation(.0f, DirectionRotation.Yaw, .0f);
					PlayEffects(Fruit->GetFruitData(), Hit.Location, EffectsRotation, Direction);
					
				}
			}
		}
	}
	
}

void AVBlade::StartCut(FVector Location)
{
	if (State==EBladeState::Previewing || State == EBladeState::Done)
	{
		StaticMeshComponent->SetVisibility(true);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NiagaraComponent->SetVisibility(false);

		StartCutLocation = Location;
		State = EBladeState::Previewing;
	}
}

void AVBlade::StopCut()
{
	if (State==EBladeState::Previewing)
	{
		StaticMeshComponent->SetVisibility(false);
		NiagaraComponent->SetVisibility(true);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EndCutLocation = GetActorLocation();

		Direction = EndCutLocation - StartCutLocation;

		Cut();
	}
}

void AVBlade::UpdateLocation(FVector NewLocation)
{
	if(State == EBladeState::Previewing)
	{
		SetActorLocation(NewLocation);

		FRotator NewPreviewRotation = UKismetMathLibrary::FindLookAtRotation(StartCutLocation, NewLocation);
		StaticMeshComponent->SetWorldRotation(FRotator(.0f, NewPreviewRotation.Yaw, .0f));
		StaticMeshComponent->SetWorldLocation(StartCutLocation);
		StaticMeshComponent->SetWorldScale3D(FVector((NewLocation-StartCutLocation).Length(), 1.0f, 1.2f));
		
	}
}

