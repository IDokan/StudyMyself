/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LocalizeText.cpp
Purpose: Source file for assignment 5 localize
Project: sinil.gang CS225 Assignment5 Localize 
Author: Sinil Kang
Creation date: 11/26/2019
- End Header ----------------------------------------------------------------
*/ 
#include <fstream>
#include <string>
#include "LocalizeText.h"

LocalizedText::LocalizedText(std::string dataFile)
: idDisplayMap()
{
    std::ifstream dataStream(dataFile);
    
    if (dataStream.is_open() == false)
    {
    	return;
    }

    std::string inputLine;
    while (std::getline(dataStream, inputLine))
    {
        // Get a position of delimiter
        long long unsigned int delimiter = inputLine.find(DELIMITER);
        // Parse the line
        long stringId = strtol(inputLine.substr(0, delimiter).c_str(), nullptr, HEXADECIMAL);
        std::string displayText = inputLine.substr(delimiter+1);

        // Save in map
        idDisplayMap.emplace(stringId, displayText);
    }
}

std::string LocalizedText::ToString(long string_id_number)
{
    return idDisplayMap[string_id_number];
}