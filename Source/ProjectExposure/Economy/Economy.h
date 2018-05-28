// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTEXPOSURE_API Economy
{
public:
	Economy();
	~Economy();

private:

	UPROPERTY()
	uint8 _pollution;

	UPROPERTY()
	uint8 _energy;
};
