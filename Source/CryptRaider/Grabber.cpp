// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	// UPhysicsHandleComponent* PhysicsHandlee = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// PhysicsHandlee = nullptr;
	// if (PhysicsHandlee == nullptr)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("PhysicsHandlee is null"));
	// }
	// else
	// {
	// 	PhysicsHandlee->GetName();
	// 	UE_LOG(LogTemp, Display, TEXT("PhysicsHandlee is not null"));
	// }
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandlee = GetPhysicsHandle();

	if (PhysicsHandlee && PhysicsHandlee->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandlee->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}



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
	UPhysicsHandleComponent* PhysicsHandlee = GetPhysicsHandle();
	if (PhysicsHandlee == nullptr)
	{
		return;
	}
	
	
	UE_LOG(LogTemp, Display, TEXT("Grabbed"));

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if (HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		PhysicsHandlee->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
		
		DrawDebugSphere(GetWorld(), HitResult.Location, GrabRadius, 12, FColor::Green, false, 0.f, 0, 10.f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, GrabRadius, 12, FColor::Yellow, false, 0.f, 0, 10.f);
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
	UPhysicsHandleComponent* PhysicsHandlee = GetPhysicsHandle();
	if (PhysicsHandlee == nullptr)
	{
		return;
	}
	if (PhysicsHandlee && PhysicsHandlee->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandlee->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandlee->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandlee is null"));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.f, 0, 10.f);
	DrawDebugSphere(GetWorld(), End, GrabRadius, 12, FColor::Blue, false, 0.f, 0, 10.f);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	return GetWorld()-> SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
		);
}