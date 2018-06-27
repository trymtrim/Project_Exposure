//Fill out your copyright notice in the Description page of Project Settings.

#include "Highscore.h"

Highscore::Highscore ()
{
	const FString fileTemplate = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n<root>\n</root>";
	FXmlFile* xmlFile = new FXmlFile (fileTemplate, EConstructMethod::ConstructFromBuffer);

	/*FXmlFile* xmlFile = new FXmlFile ();
	if (xmlFile == nullptr)
		return;*/

	FXmlNode* xmlRoot = xmlFile->GetRootNode ();
	if (xmlRoot == nullptr)
		return;

	FXmlAttribute* xmlAttribute = new FXmlAttribute (TEXT ("Name"), TEXT ("MyGame"));
	if (xmlAttribute == nullptr)
		return;
 
	xmlRoot->SetContent (TEXT ("Game"));
 
	xmlFile->Save ("D:/Game_Development/Git/NewProjectExposure/Project_Exposure/yo.xml");
	xmlFile->Clear ();
}

void Highscore::AddScore (FString name, int score)
{

}

Highscore::~Highscore ()
{

}
