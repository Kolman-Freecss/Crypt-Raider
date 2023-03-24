// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FRotator MyRotation = GetComponentRotation();
	// FString MyRotationString = MyRotation.ToString();
	// UE_LOG(LogTemp, Display, TEXT("MyRotation: %s"), *MyRotationString);
	//
	// float Time = GetWorld()->TimeSeconds;
	// UE_LOG(LogTemp, Display, TEXT("Time: %f"), Time);

	///Step 90
	// float Damage;
	// if (HasDamage(Damage))
	// {
	// 	PrintDamage(Damage);
	// }
	// float& DamageRef = Damage;
	// DamageRef = 10;
	//UE_LOG(LogTemp, Display, TEXT("Original Damage: %f"), Damage);

	
}

void UGrabber::PrintDamage(const float& Damage)
{
	//Damage = 2;
	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
}

bool UGrabber::HasDamage(float& OutDamage)
{
	OutDamage = 10;
	return true;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grabbed"));

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.f, 0, 10.f);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()-> SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
		);

	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hit: %s"), *HitActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Hit"));
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released"));
}