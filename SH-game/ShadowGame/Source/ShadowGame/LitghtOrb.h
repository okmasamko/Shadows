// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LitghtOrb.generated.h"

UCLASS(Blueprintable)
class SHADOWGAME_API ALitghtOrb : public AActor
{
	GENERATED_BODY()
	float overallTime = 0;

public:
	UFUNCTION(BlueprintCallable)
	FVector Orbit(float time, float speed, float radius, float height, AActor* Player);
};
