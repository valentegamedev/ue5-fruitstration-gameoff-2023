// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "VBlade.generated.h"

UENUM(BlueprintType)
enum class EBladeState : uint8 {
	Previewing,
	Cutting,
	Done
};

UCLASS()
class FRUITSTRATION_API AVBlade : public AActor
{
	GENERATED_BODY()

	//TODO: Remove capsule, not needed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	EBladeState State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector StartCutLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector EndCutLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector Direction;
public:
	
	AVBlade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void Cut();

	UFUNCTION(BlueprintCallable)
	void StartCut(FVector Location);

	UFUNCTION(BlueprintCallable)
	void StopCut();

	UFUNCTION(BlueprintCallable)
	void UpdateLocation(FVector NewLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEffects(UVFruitData* FruitData, FVector Location, FRotator Rotation, FVector RawDirection);
};
