// Fill out your copyright notice in the Description page of Project Settings.


#include "VFruit.h"

#include "VFruitData.h"
#include "GeometryScript/CollisionFunctions.h"
#include "GeometryScript/MeshAssetFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/MeshDecompositionFunctions.h"
#include "GeometryScript/MeshMaterialFunctions.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshQueryFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AVFruit::Initialize()
{
	if(FruitData)
	{
		UDynamicMesh* DynamicMesh = DynamicMeshComponent->GetDynamicMesh();
		
		EGeometryScriptOutcomePins GeometryScriptOutcomePins;
		UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMesh(FruitData->Mesh, DynamicMesh, FGeometryScriptCopyMeshFromAssetOptions(), FGeometryScriptMeshReadLOD(), GeometryScriptOutcomePins);
		if(GeometryScriptOutcomePins == EGeometryScriptOutcomePins::Success)
		{
			FGeometryScriptCollisionFromMeshOptions GeometryScriptCollisionFromMeshOptions;
			GeometryScriptCollisionFromMeshOptions.Method = EGeometryScriptCollisionGenerationMethod::ConvexHulls;
			UGeometryScriptLibrary_CollisionFunctions::SetDynamicMeshCollisionFromMesh(DynamicMesh, DynamicMeshComponent, GeometryScriptCollisionFromMeshOptions);
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to copy mesh."));
		}
	}
}

void AVFruit::BeginPlay()
{
	Super::BeginPlay();
	
	if (FruitData) {
		DynamicMaterial = DynamicMeshComponent->CreateDynamicMaterialInstance(0, FruitData->Material);
		//DynamicMeshComponent->SetWorldScale3D(FVector(FruitData->Scale));
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("FruitData can't be null."));
	}
	
	if(bAutoInitialize)
	{
		Initialize();
	}
}

AVFruit* AVFruit::Clone()
{
	AVFruit* Clone = Cast<AVFruit>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), GetClass(), GetTransform(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if(Clone)
	{
		Clone->FruitData = FruitData;
		UGameplayStatics::FinishSpawningActor(Clone, GetTransform());
	}

	UDynamicMesh* CopyToMeshOut;
	UGeometryScriptLibrary_MeshDecompositionFunctions::CopyMeshToMesh(DynamicMeshComponent->GetDynamicMesh(), Clone->GetDynamicMeshComponent()->GetDynamicMesh(), CopyToMeshOut);
	
	return Clone;
}

void AVFruit::Cut(FVector Direction, FVector Location, bool bFlipCutSide)
{

	FVector NormalizedDirection = Direction.GetSafeNormal();
	//TODO: Fix GetDynamicMeshComponent to call the getter
	UDynamicMesh* ComputeMesh = AllocateComputeMesh();
	FTransform ComputeMeshTransform;

	FVector ComputeMeshLocalLocation = GetActorTransform().InverseTransformPosition(Location);
	ComputeMeshTransform.SetLocation(ComputeMeshLocalLocation);

	FRotator SideCuteRotation(0.0f, 0.0, (bFlipCutSide?-90:90));
	NormalizedDirection.Z = 0.0f;
	FVector ComputeMeshRightVector = FVector::CrossProduct(FVector::UpVector, NormalizedDirection);
	FRotator CutWorldRotation = UKismetMathLibrary::MakeRotationFromAxes(NormalizedDirection, ComputeMeshRightVector, FVector::UpVector);
	FRotator CutLocalRotation = GetActorTransform().InverseTransformRotation(CutWorldRotation.Quaternion()).Rotator();
	FRotator CutCombinedRotation = UKismetMathLibrary::ComposeRotators(SideCuteRotation, CutLocalRotation);
	ComputeMeshTransform.SetRotation(CutCombinedRotation.Quaternion());
	
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(ComputeMesh, FGeometryScriptPrimitiveOptions(), ComputeMeshTransform, 200, 200, 200);
	UGeometryScriptLibrary_MeshMaterialFunctions::RemapMaterialIDs(ComputeMesh, 0, 1);
	
	UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean(DynamicMeshComponent->GetDynamicMesh(), FTransform(), ComputeMesh, FTransform(), EGeometryScriptBooleanOperation::Subtract, FGeometryScriptMeshBooleanOptions());

	TArray<UMaterialInterface*> NewMaterialSet;
	NewMaterialSet.Add(GetFruitData()->Material);
	NewMaterialSet.Add(GetFruitData()->InnerMaterial);
	DynamicMeshComponent->ConfigureMaterialSet(NewMaterialSet);
	
	FGeometryScriptCollisionFromMeshOptions GeometryScriptCollisionFromMeshOptions;
	GeometryScriptCollisionFromMeshOptions.Method = EGeometryScriptCollisionGenerationMethod::ConvexHulls;

	UpdateGizmos(Location, CutWorldRotation);
	UGeometryScriptLibrary_CollisionFunctions::SetDynamicMeshCollisionFromMesh(DynamicMeshComponent->GetDynamicMesh(), DynamicMeshComponent, GeometryScriptCollisionFromMeshOptions);

	ReleaseAllComputeMeshes();

	DynamicMeshComponent->AddImpulse(((ComputeMeshRightVector * (bFlipCutSide?CutImpulseMultiplier:-CutImpulseMultiplier)) + FVector::UpVector) * Direction.Length());
}

float AVFruit::GetVolume()
{
	//TODO: Set as const and Pure
	float SurfaceArea;
	float Volume;
	UGeometryScriptLibrary_MeshQueryFunctions::GetMeshVolumeArea(DynamicMeshComponent->GetDynamicMesh(), SurfaceArea, Volume);

	return Volume * GetActorScale3D().X * GetFruitData()->WeightScale;
}
