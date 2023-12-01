// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFruit.h"
#include "GameFramework/Actor.h"
#include "VScale.generated.h"

UCLASS()
class FRUITSTRATION_API AVScale : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float TotalWeight;

	UPROPERTY()
	float CurrentLerpWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float ScaleSpeed;

public:	
	AVScale();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDigits(const FText& NewValue);
};
