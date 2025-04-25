// Fill out your copyright notice in the Description page of Project Settings.


#include "LitghtOrb.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
FVector ALitghtOrb::Orbit(float time, float speed, float radius, float height, AActor* Player)
{
	if (overallTime == 0)
	{
		overallTime = FMath::RandRange(0, 100);
	}
	overallTime += time;
	float OrbitAngle = overallTime * speed;

	FVector Offset;
	Offset.X = FMath::Cos(OrbitAngle) * radius;
	Offset.Y = FMath::Sin(OrbitAngle) * radius;
	Offset.Z = height;

	if (!Player) return GetActorLocation(); // Fail-safe

	FVector NewLocation = Player->GetActorLocation() + Offset;
	return NewLocation;
}

