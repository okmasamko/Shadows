// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineIndicatorCode.generated.h"

UCLASS(Blueprintable)
class SHADOWGAME_API ALineIndicatorCode : public AActor
{
	GENERATED_BODY()


	public:
		UFUNCTION(BlueprintCallable) FVector MoveToMiddle(UPARAM(DisplayName="Start") FVector start, UPARAM(DisplayName="end") FVector end);
		UFUNCTION(BlueprintCallable) float CalculateSize(UPARAM(DisplayName="Start") FVector start, UPARAM(DisplayName="end") FVector end);
		UFUNCTION(BlueprintCallable) FRotator CalculateRotation(UPARAM(DisplayName = "Start") FVector start, UPARAM(DisplayName = "End") FVector end);

};
