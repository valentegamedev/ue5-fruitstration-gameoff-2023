// Fill out your copyright notice in the Description page of Project Settings.


#include "VCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AVCameraPawn::AVCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	DummySceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy"));
	SetRootComponent(DummySceneComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AVCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

