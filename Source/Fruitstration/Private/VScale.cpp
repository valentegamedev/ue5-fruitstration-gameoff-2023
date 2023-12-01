// Fill out your copyright notice in the Description page of Project Settings.


#include "VScale.h"

#include "Components/BoxComponent.h"

AVScale::AVScale()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
}

void AVScale::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentLerpWeight = FMath::Lerp(CurrentLerpWeight, TotalWeight, DeltaTime * ScaleSpeed);
	int32 IntWeight = FMath::RoundToInt32(CurrentLerpWeight);
	
	FFormatNamedArguments Args;
	Args.Add("Weight", IntWeight);
	FText FormattedText = FText::Format(NSLOCTEXT("Fruitstration", "Weight", "{Weight}g"), Args);

	UpdateDigits(FormattedText);
}

