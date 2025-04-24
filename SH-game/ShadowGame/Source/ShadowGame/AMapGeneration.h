// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "AMapGeneration.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SHADOWGAME_API AAMapGeneration : public ALevelScriptActor
{
	GENERATED_BODY()


	TArray<TArray<int>> gridCode;
	TArray<int> numbersUsedCode;

	UPROPERTY(EditAnywhere, Category = "Blocks") TSubclassOf<AActor> blockGrass;
	UPROPERTY(EditAnywhere, Category = "Blocks") TSubclassOf<AActor> blockGrassCorner;
	UPROPERTY(EditAnywhere, Category = "Blocks") TSubclassOf<AActor> blockGrassSide;
	UPROPERTY(EditAnywhere, Category = "Blocks") TSubclassOf<AActor> blockBridge;
	UPROPERTY(EditAnywhere, Category = "Blocks") TSubclassOf<AActor> blockBridgeStart;
	UPROPERTY(EditAnywhere, Category = "Props") TSubclassOf<AActor> Lamp;
	UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<AActor> normalEnemy;
	UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<AActor> rangedEnemy;





	TArray<int> GetNeighbourCell(FVector2d coords, TArray<int> types);
	void DispalyBridge(FVector2d coords);
	void DisplayDirt(FVector2d coords);
	void CreateCircle(FVector2d coords, int radius, int type);
	TArray<FVector2d> GetAllTypeCode(int type);

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int gridResolutionCode = 100;

	UFUNCTION(BlueprintCallable) void InitializeGridCode();
	UFUNCTION(BlueprintCallable) void GenerateMapCode();
	UFUNCTION(BlueprintCallable) void GenerateIslandsCode();
	UFUNCTION(BlueprintCallable) void DispalyCode();
	UFUNCTION(BlueprintCallable) void Populate(UPARAM(DisplayName="Actor") TArray<TSubclassOf<AActor>> toSpawn, UPARAM(DisplayName="Threshold") TArray<float> threshold);
	UFUNCTION(BlueprintCallable) void PlaceLamps();
	UFUNCTION(BlueprintCallable) void PlaceEnemyCamps();

};
