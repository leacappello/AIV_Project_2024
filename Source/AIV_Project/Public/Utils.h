// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Engine/Engine.h>
#include <Kismet/GameplayStatics.h>

#define SCREENLOG(Format,Color,...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOGWARN(Format,...) UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__)
#define LOGERROR(Format,...) UE_LOG(LogTemp, Error, TEXT(Format), ##__VA_ARGS__)
#define LOGINFO(Format,...) UE_LOG(LogTemp, Display, TEXT(Format), ##__VA_ARGS__)
