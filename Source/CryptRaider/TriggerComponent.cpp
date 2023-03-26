// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Display, TEXT("TriggerComponent TickComponent"));

	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this
			, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetShouldMove(true);
		UE_LOG(LogTemp, Display, TEXT("TriggerComponent TickComponent: Actor is not null"));
	}
	else
	{
		Mover->SetShouldMove(false);
		UE_LOG(LogTemp, Display, TEXT("TriggerComponent TickComponent: Actor is null"));
	}
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool isGrabbed = Actor->ActorHasTag("Grabbed");
		if (HasAcceptableTag && !isGrabbed)
		{
			return Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}
