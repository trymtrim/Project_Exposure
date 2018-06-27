//Fill out your copyright notice in the Description page of Project Settings.

#include "Highscore.h"
#include <iostream>
#include <fstream>

Highscore::Highscore ()
{
	std::ofstream file;
	file.open ("D:/Game_Development/Git/NewProjectExposure/Project_Exposure/test/yo.csv");
	
	file << "This is the first cell in the first column.\n";
	file << "a;b;c;\n";
	file << "semi;colon";
	file.close ();

	/*const FString fileTemplate = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n<root>\n</root>";
	FXmlFile* xmlFile = new FXmlFile (fileTemplate, EConstructMethod::ConstructFromBuffer);

	FXmlFile* xmlFile = new FXmlFile ();
	if (xmlFile == nullptr)
		return;

	FXmlNode* xmlRoot = xmlFile->GetRootNode ();
	if (xmlRoot == nullptr)
		return;

	FXmlAttribute* xmlAttribute = new FXmlAttribute (TEXT ("Name"), TEXT ("MyGame"));
	if (xmlAttribute == nullptr)
		return;
 
	xmlRoot->SetContent (TEXT ("Game"));
 
	xmlFile->Save ("D:/Game_Development/Git/NewProjectExposure/Project_Exposure/yo.xml");
	xmlFile->Clear ();*/
}

void Highscore::AddScore (FString name, int score)
{

}

Highscore::~Highscore ()
{

}
