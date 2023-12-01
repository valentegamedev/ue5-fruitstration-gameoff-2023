// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "VFruit.generated.h"

/**
 * 
 */
UCLASS()
class FRUITSTRATION_API AVFruit : public ADynamicMeshActor
{
	GENERATED_BODY()
	//TODO: TObjectPrt
	//TODO: Add blender sources
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess, ExposeOnSpawn))
	class UVFruitData* FruitData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool bAutoInitialize;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float CutImpulseMultiplier = 1;
public:
	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	AVFruit* Clone();

	UFUNCTION(BlueprintCallable)
	void Cut(FVector Direction, FVector Location, bool bFlipCutSide);

	UFUNCTION(BlueprintCallable)
	float GetVolume();

	FORCEINLINE UVFruitData* GetFruitData() const
	{
		return FruitData;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGizmos(FVector Location, FRotator Rotation);
};
