//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class PROJECTEXPOSURE_API Highscore
{
public:
	Highscore (FString filePath);
	~Highscore ();

	void AddScore (FString name, int score);
	TArray <int> GetHighscores ();
	TArray <FString> GetHighscoreNames ();

private:
	void SaveScores ();
	void LoadScores ();
	void CreateInitialFile ();

	TArray <FString> _names;
	TArray <int> _scores;

	FString _filePath;
};
