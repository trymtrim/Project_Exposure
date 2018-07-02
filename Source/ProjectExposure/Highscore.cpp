//Fill out your copyright notice in the Description page of Project Settings.

#include "Highscore.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

Highscore::Highscore (FString filePath) : _filePath (filePath)
{
	LoadScores ();
}

void Highscore::AddScore (FString name, int score)
{
	_scores.Add (score);
	_names.Add (name);

	TArray <FString> newNames;
	TArray <int> newScores;

	for (int i = 0; i < 10; i++)
	{
		int highestScoreIndex = 0;
		int tempHighestScore = 0;

		for (int j = 0; j < _scores.Num (); j++)
		{
			int currentScore = _scores [j];

			if (currentScore > tempHighestScore || currentScore == 0 && tempHighestScore == 0)
			{
				tempHighestScore = currentScore;
				highestScoreIndex = j;
			}
		}

		newNames.Add (_names [highestScoreIndex]);
		newScores.Add (_scores [highestScoreIndex]);
		
		_names.RemoveAt (highestScoreIndex);
		_scores.RemoveAt (highestScoreIndex);
	}

	_names = newNames;
	_scores = newScores;

	SaveScores ();
}

void Highscore::SaveScores ()
{
	std::ofstream file;
	file.open (std::string (TCHAR_TO_UTF8 (*_filePath)));
	
	file << "Name;Date;Time;Score;Year;Feedback\n";

	for (int i = 0; i < _names.Num (); i++)
	{
		std::string name = std::string (TCHAR_TO_UTF8 (*_names [i]));

		if (name == "--")
		{
			std::string date = "--";
			std::string time = "--";
			std::string score = "0";
			std::string year = "--";
			std::string feedback = "--";
			
			file << name << ";" << date << ";" << time << ";" << score << ";" << year << ";" << feedback << "\n";
		}
		else
		{
			//Get current date and time
			auto t = std::time (nullptr);
			auto tm = *std::localtime (&t);

			//Date
			std::ostringstream dateStream;
			dateStream << std::put_time (&tm, "%d.%m.%Y");
			std::string date = dateStream.str ();

			//Time
			std::ostringstream timeStream;
			timeStream << std::put_time (&tm, "%H:%M");
			std::string time = timeStream.str ();

			//Score
			std::string score = std::string (TCHAR_TO_UTF8 (*FString::FromInt (_scores [i])));

			//Year
			//Time
			std::ostringstream yearStream;
			yearStream << std::put_time (&tm, "%Y");
			std::string year = yearStream.str ();
			
			//Feedback
			std::string feedback = "Sure";
			
			file << name << ";" << date << ";" << time << ";" << score << ";" << year << ";" << feedback << "\n";
		}
	}

	file.close ();
}

TArray <int> Highscore::GetHighscores ()
{
	return _scores;
}

TArray <FString> Highscore::GetHighscoreNames ()
{
	return _names;
}

void Highscore::LoadScores ()
{
	//Load the file
	std::ifstream file (std::string (TCHAR_TO_UTF8 (*_filePath)).c_str ());

	//If there is no file yet, create a file
	if (!file)
	{
		CreateInitialFile ();
		LoadScores ();

		return;
	}

	TArray <FString> lines;

	std::string str;

	bool firstLineFinished = false;

	//Read the next line from File untill it reaches the end
	while (std::getline (file, str))
	{
		//If we're on the first line, skip to next line
		if (!firstLineFinished)
		{
			firstLineFinished = true;
			continue;
		}

		//Save each line to the array
		if (str.size () > 0)
			lines.Add (str.c_str ());
	}

	file.close ();

	for (int i = 0; i < lines.Num (); i++)
	{
		TArray <FString> splitArray;

		std::stringstream ss (std::string (TCHAR_TO_UTF8 (*lines [i])));
		std::string word;

		while (std::getline (ss, word, ';'))
			splitArray.Add (word.c_str ());

		_names.Add (splitArray [0]);
		_scores.Add (FCString::Atoi (*splitArray [3]));
	}
}

void Highscore::CreateInitialFile ()
{
	std::ofstream file;
	file.open (std::string (TCHAR_TO_UTF8 (*_filePath)));
	
	file << "Name;Date;Time;Score;Year;Feedback\n";

	for (int i = 0; i < 10; i++)
		file << "--;--;--;--;--;--\n";

	file.close ();
}

Highscore::~Highscore ()
{

}
