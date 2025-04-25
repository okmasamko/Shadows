// Fill out your copyright notice in the Description page of Project Settings.


#include "LineIndicatorCode.h"
#include "Kismet/KismetMathLibrary.h"


FVector ALineIndicatorCode::MoveToMiddle(FVector start, FVector end)
{
	return (start + end) * 0.5f;
}

float ALineIndicatorCode::CalculateSize(FVector start, FVector end)
{
	return FVector::Distance(start, end) / 100;
}

FRotator ALineIndicatorCode::CalculateRotation(FVector start, FVector end)
{
	return UKismetMathLibrary::FindLookAtRotation(start, end);
}


