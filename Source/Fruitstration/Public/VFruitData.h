// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VFruitData.generated.h"

UCLASS(BlueprintType)
class FRUITSTRATION_API UVFruitData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> InnerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WeightScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor SplashColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Scale;
};
