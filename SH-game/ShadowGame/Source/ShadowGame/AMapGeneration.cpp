// Fill out your copyright notice in the Description page of Project Settings.


#include "AMapGeneration.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Noise/FastNoiseLite.h"
#include <stdexcept>  // for std::runtime_error
#include <exception>  // for std::exception



//FINISHED
void AAMapGeneration::InitializeGridCode() {

	for (int x = 0; x < gridResolutionCode; x++)
	{
		TArray<int> currentRow;
		for (int y = 0; y < gridResolutionCode; y++)
		{
			currentRow.Add(0);
		}
		gridCode.Add(currentRow);
	}

	UKismetSystemLibrary::PrintString(this, TEXT("Initialize gridCode Code"));
}

//FINISHED
void AAMapGeneration::GenerateMapCode() {
	int RandomValue = FMath::RandRange(3, 7);

	for (int i = 0; i < RandomValue; i++)
	{
		TArray<FVector2d> currentType = GetAllTypeCode(1);
		if (currentType.Num() > 0)
		{
			FVector2d start = currentType[FMath::RandRange(0, currentType.Num() - 1)];
			CreateCircle(start, FMath::RandRange(5, 10), 1);
		}
		else
		{
			CreateCircle(FVector2d(50, 50), FMath::RandRange(5, 10), 1);
		}
	}

	numbersUsedCode.Add(1);
}

//FINISHED
void AAMapGeneration::GenerateIslandsCode()
{
	//numbersUsedCode = new TArray<int>();
	numbersUsedCode.Add(1);

	int numberOfIslands = FMath::RandRange(3, 7);

	for (int i = 0; i < numberOfIslands; i++)
	{
		TArray<FVector2d> potentialStarts = GetAllTypeCode(numbersUsedCode[FMath::RandRange(0, numbersUsedCode.Num() - 1)]);

		FVector2d position = potentialStarts[FMath::RandRange(0, potentialStarts.Num() - 1)];
		int direction = FMath::RandRange(0, 3);
		int zerosEncountered = 0;
		TArray<FVector2d> path = TArray<FVector2d>();
		int islandDistance = FMath::RandRange(12, 16);

		while (true)
		{
			try{
				if (position.X < 0 || position.X >= gridResolutionCode || position.Y < 0 || position.Y >= gridResolutionCode)
					throw std::exception("Something went wrong");
				if (gridCode[position.X][position.Y] == 0)
				{
					zerosEncountered++;
					path.Add(FVector2d(position.X, position.Y));
				}
				switch (direction)
				{
					case 0:
						position.X--;
						break;
					case 1:
						position.X++;
						break;
					case 2:
						position.Y--;
						break;
					case 3:
						position.Y++;
						break;
				}
			}
			catch(...){
				zerosEncountered = 0;
				direction = FMath::RandRange(0, 3);
				path = TArray<FVector2d>();
				islandDistance = FMath::RandRange(12, 16);
				position = potentialStarts[FMath::RandRange(0, potentialStarts.Num() - 1)];
			}

			if (zerosEncountered >= islandDistance)
				break;
		}

		FVector2d islandStart = path[path.Num() - 1];
		int sizeRandomness = FMath::RandRange(4, 10);

		for (auto current : path)
		{
			gridCode[current.X][current.Y] = 2;
		}

		for (int e = 0; e < sizeRandomness; e++)
		{
			int radius = FMath::RandRange(3, 5);
			if (e == 0)
			{
				CreateCircle(islandStart, radius, i + 3);
			}
			else
			{
				TArray<FVector2d> possiblePlace = GetAllTypeCode(i + 3);
				CreateCircle(possiblePlace[FMath::RandRange(0, possiblePlace.Num() - 2)], radius, i + 3);
			}
		}

		numbersUsedCode.Add(i + 3);
	}
}

//FINISHED
void AAMapGeneration::DispalyCode()
{
	for (int x = 0; x < gridResolutionCode; x++)
	{
		for (int y = 0; y < gridResolutionCode; y++)
		{
			if (gridCode[x][y] == 2)
				DispalyBridge(FVector2d(x, y));
			else if (gridCode[x][y] != 0)
				DisplayDirt(FVector2d(x, y));
		}
	}
}

//FINISHED
void AAMapGeneration::Populate(TArray<TSubclassOf<AActor>> toSpawn, TArray<float> threshold)
{
	TArray<FastNoiseLite> noises;
	for (int i = 0; i < toSpawn.Num(); i++)
	{
		FastNoiseLite Noise;
		Noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
		Noise.SetFrequency(0.1f);
		Noise.SetSeed(FMath::RandRange(0, 1000));
		noises.Add(Noise);

	}

	for (int x = 0; x < gridResolutionCode; x++)
	{
		for (int y = 0; y < gridResolutionCode; y++)
		{
			if (gridCode[x][y] == 1 || gridCode[x][y] > 2)
			{
				for (int e = 0; e < toSpawn.Num(); e++)
				{
					float currentThreshold = threshold[e];
					TSubclassOf<AActor> currentSpawn = toSpawn[e];


					float noiseValue = (noises[e].GetNoise((float) x, (float) y) + 1) / 2;

					if (noiseValue < currentThreshold)
					{
						FVector SpawnLocation = FVector((x - 50) * 190, (y - 50) * 190, 0.f);
						FActorSpawnParameters SpawnParams;
						FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);


						GetWorld()->SpawnActor<AActor>(currentSpawn, SpawnLocation, SpawnRotation, SpawnParams);
					}
				}
			}

		}
	}

}

//FINISHED
void AAMapGeneration::PlaceLamps()
{
    for (int value : numbersUsedCode)
    {
        TArray<FVector2d> allNumbers = GetAllTypeCode(value);
        FVector2d position = allNumbers[FMath::RandRange(0, allNumbers.Num() - 1)];

        FVector SpawnLocation = FVector((position.X - 50) * 190, (position.Y - 50) * 190, 0.f);
        FActorSpawnParameters SpawnParams;
        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
        TSubclassOf<AActor> currentSpawn = Lamp;


        GetWorld()->SpawnActor<AActor>(currentSpawn, SpawnLocation, SpawnRotation, SpawnParams);

    }
}

//UNFINISHED
void AAMapGeneration::PlaceEnemyCamps()
{
    for (int value : numbersUsedCode)
    {
        TArray<FVector2d> allNumbers = GetAllTypeCode(value);

        int numOf = (allNumbers.Num() / 100);// * FMath::RandRange(1, 3);
        for (int i = 0; i < numOf; i++)
        {

            FVector2d position = allNumbers[FMath::RandRange(0, allNumbers.Num() - 1)];
            FVector SpawnLocation = FVector((position.X - 50) * 190, (position.Y - 50) * 190, 89.f);
            FActorSpawnParameters SpawnParams;
            FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

            TSubclassOf<AActor> currentSpawn = normalEnemy;
            if (FMath::RandRange(0, 100) > 60) {
                currentSpawn = rangedEnemy;
            }

            GetWorld()->SpawnActor<AActor>(currentSpawn, SpawnLocation, SpawnRotation, SpawnParams);
        }

    }
}


// HELPERS ---------------------------------------------------------------------------------------
//FINISHED
void AAMapGeneration::DispalyBridge(FVector2d coords)
{
	int neighboursCount = 0;
	TArray<int> types;
	types.Add(2);
	TArray<int> neighbourCells = GetNeighbourCell(coords, types);

	for (int current : neighbourCells)
	{
		if (current == 1) neighboursCount ++;
	}

	TSubclassOf<AActor> blockToSpawn;
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

	switch (neighboursCount)
	{
		case 2:
			if (neighbourCells[0] == 1)
				SpawnRotation = FRotator(0.f, 90.f, 0.f);
			else
				SpawnRotation = FRotator(0.f, 0.f, 0.f);
			blockToSpawn = blockBridge;
			break;
		default:
			if (neighbourCells[0] == 1)
				SpawnRotation = FRotator(0.f, 90.f, 0.f);
			else if (neighbourCells[1] == 1)
				SpawnRotation = FRotator(0.f, 0.f, 0.f);
			else if (neighbourCells[2] == 1)
				SpawnRotation = FRotator(0.f, 270.f, 0.f);
			else
				SpawnRotation = FRotator(0.f, 180.f, 0.f);
			blockToSpawn = blockBridgeStart;
			break;
	}

	FVector SpawnLocation = FVector((coords.X - 50) * 190, (coords.Y - 50) * 190, 0.f);

	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AActor>(blockToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
}

//FINISHED
void AAMapGeneration::DisplayDirt(FVector2d coords)
{
	// - 50) * 190;
	int neighboursCount = 0;
	TArray<int> neighbourCells = GetNeighbourCell(coords, numbersUsedCode);

	for (int current : neighbourCells)
	{
		if (current == 1) neighboursCount ++;
	}

	TSubclassOf<AActor> blockToSpawn;
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	FVector SpawnLocation = FVector((coords.X - 50) * 190, (coords.Y - 50) * 190, 0.f);

	switch (neighboursCount)
	{
		case 3:
			blockToSpawn = blockGrassSide;
			if (neighbourCells[0] == 0)
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 90.f, 0.f);
			}
			else if (neighbourCells[1] == 0)
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 0.f, 0.f);
			}
			else if (neighbourCells[2] == 0)
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 270.f, 0.f);
			}
			else
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 180.f, 0.f);
			}
			break;
		case 2:
			blockToSpawn = blockGrassCorner;

			if (neighbourCells[0] == 0 && neighbourCells[1] == 0)
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 90.f, 0.f);
			}
			else if (neighbourCells[1] == 0 && neighbourCells[2] == 0)
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 0.f, 0.f);
			}
			else if (neighbourCells[2] == 0 && neighbourCells[3] == 0)
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 270.f, 0.f);
			}
			else
			{
				//SpawnLocation.Z += 100;
				SpawnRotation = FRotator(0.f, 180.f, 0.f);
			}
			break;
		default:
			blockToSpawn = blockGrass;
			break;
	}

	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AActor>(blockToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
}

//FINISHED
TArray<FVector2d> AAMapGeneration::GetAllTypeCode(int _type)
{
	TArray<FVector2d> result;
	for (int x = 0; x < gridResolutionCode; x++)
	{
		for (int y = 0; y < gridResolutionCode; y++)
		{
			if (gridCode[x][y] == _type)
				result.Add(FVector2d(x, y));
		}
	}

	return result;
}

//FINISHED
void AAMapGeneration::CreateCircle(FVector2d coords, int radius, int type)
{
	for (int x = coords.X - radius; x < coords.X + radius; x++)
	{
		for (int y = coords.Y - radius; y < coords.Y + radius; y++)
		{
			float distance = FMath::Sqrt(FMath::Square(coords.X - x) + FMath::Square(coords.Y - y));
			if (distance < radius)
			{
				if (x >= 0 && x < gridResolutionCode && y >= 0 && y < gridResolutionCode)
					gridCode[x][y] = type;
			}
		}
	}
}

//FINISHED
TArray<int> AAMapGeneration::GetNeighbourCell(FVector2d coords, TArray<int> types)
{
	TArray<int> neighbourCells;

	if (coords.X + 1 < gridResolutionCode)
	{
		if (types.Contains(gridCode[coords.X + 1][coords.Y])){
			neighbourCells.Add(1);
		}
		else {
			neighbourCells.Add(0);
		}
	}
	else
	{
		neighbourCells.Add(0);
	}

	if (coords.Y - 1 >= 0)
	{

		if (types.Contains(gridCode[coords.X][coords.Y - 1])){
			neighbourCells.Add(1);
		}
		else {
			neighbourCells.Add(0);
		}
	}
	else
	{
		neighbourCells.Add(0);
	}

	if (coords.X - 1 >= 0)
	{

		if (types.Contains(gridCode[coords.X - 1][coords.Y])){
			neighbourCells.Add(1);
		}
		else {
			neighbourCells.Add(0);
		}
	}
	else
	{
		neighbourCells.Add(0);
	}

	if (coords.Y + 1 < gridResolutionCode)
	{

		if (types.Contains(gridCode[coords.X][coords.Y + 1])){
			neighbourCells.Add(1);
		}
		else {
			neighbourCells.Add(0);
		}
	}
	else
	{
		neighbourCells.Add(0);
	}

	return neighbourCells;
}