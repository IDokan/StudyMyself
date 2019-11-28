/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Preprocess.cpp
Purpose: Converter that takes txt file and return dat file.
Project: sinil.gang CS225 Assignment5 Localize 
Author: Sinil Kang
Creation date: 11/26/2019
- End Header ----------------------------------------------------------------
*/ 
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

#define FAIL_TO_OPEN false
#define INIT_STRING_ID_NUMBER 0x10000

/* Helper function declarations */

void ConvertFiles(char const * filePath, const std::map<unsigned long, std::string>& map);
std::string ParseFileName(std::string filePath);
void MakeDatFile(std::ofstream& outStream, const std::map<unsigned long, std::string>& stringIdMap, const std::map<std::string, std::string>& idDisplayMap);
void FillIdMap(std::map<unsigned long, std::string>& stringIdMap, std::string filePath);

/* End of declarations */

void MakeDatFile(std::ofstream& outStream, const std::map<unsigned long, std::string>& stringIdMap, const std::map<std::string, std::string>& idDisplayMap)
{
    for (const auto& stringIdIterator : stringIdMap)
    {
        const auto& displayResult = idDisplayMap.find(stringIdIterator.second);
        
        // If fail to find,
        if (displayResult == end(idDisplayMap))
        {
            // result would be "stringIdNumber \t stringIdText"
            outStream << std::hex << std::uppercase << stringIdIterator.first << '\t' << stringIdIterator.second << std::endl;
        }
        // If success,
        else
        {
            // result would be "stringIdNumber \t displayText"
            outStream << std::hex << std::uppercase << stringIdIterator.first << '\t' << displayResult->second << std::endl;
        }
    }
}

void ConvertFiles(char const* filePath, const std::map<unsigned long, std::string>& map)
{
    std::ifstream txtFile(filePath);

    // Result Map STL that takes 
    std::map<std::string, std::string> numberTextMap;

    // Error check
    if (txtFile.is_open() == FAIL_TO_OPEN)
    {  
        return;
    }
    
    // Match text with given map
    std::string textLine;
    while (getline(txtFile, textLine))
    {
        // Find delimiter and divide a line with it.
        int delimiter = textLine.find('\t');
        std::string stringIdText = textLine.substr(0, delimiter);
        std::string displayText = textLine.substr(delimiter+1);
        
        // store in result map
        numberTextMap.emplace(stringIdText, displayText);
    }


    // After update result map, output result (Make file in here)
    std::ofstream datFile(ParseFileName(filePath) + ".dat");
    if(datFile.is_open() == true)
    {
        MakeDatFile(datFile, map, numberTextMap);
    }
}

std::string ParseFileName(std::string filePath)
{
    int fileNamePos = filePath.find('.');
    return filePath.substr(0, fileNamePos);
}

void FillIdMap(std::map<unsigned long, std::string>& stringIdMap, std::string filePath)
{
    std::ifstream txtFile(filePath);
    std::ofstream datFile(ParseFileName(filePath) + ".dat");

    std::ofstream stringsDefineFile("Strings.h");

    // Error check if they are not opened
    if (
        txtFile.is_open() == FAIL_TO_OPEN || 
        datFile.is_open() == FAIL_TO_OPEN || 
        stringsDefineFile.is_open() == FAIL_TO_OPEN
        )
    {
        std::cout << "Unable to open the file : " << filePath << std::endl;
    }

    unsigned long stringIdNumber = INIT_STRING_ID_NUMBER;
    // tmporary variable for take each line
    std::string textLine;
    while (getline(txtFile, textLine))
    {
        // Find delimiter and divide a line with it.
        int delimiter = textLine.find('\t');
        std::string stringIdText = textLine.substr(0, delimiter);
        std::string displayText = textLine.substr(delimiter+1);
    
        // Update map
        stringIdMap.insert(make_pair(stringIdNumber, stringIdText));

        // result in datFile would be "stringIdNumber \t displayText\n"
        datFile << std::hex << std::uppercase <<  stringIdNumber << '\t' << displayText << std::endl;
        // result in Strings.h would be "#define stringIdText 0xstringIdNumber\n"
        stringsDefineFile << std::hex << std::uppercase << "#define " << stringIdText << " 0x" << stringIdNumber << std::endl;

        // Update stringIdNumber
        ++stringIdNumber;
    }
}

int main(int argc, char const *argv[])
{
    int argumentCount = 6;
    const char * argumentValues[] = 
    {
        argv[0],
        "English.txt",
        "French.txt",
        "Italian.txt",
        "Spanish.txt",
        "Korean.txt",
    };

    // Open text file with given file path & Open dat file
    std::map<unsigned long, std::string> stringIds;
    std::string filePath = argumentValues[1];
    FillIdMap(stringIds, filePath);
    
    // Covert extra files into .dat file
    while (argumentCount > 2)
    {
        ConvertFiles(argumentValues[--argumentCount], stringIds);
    }
    
    
    
    return 0;
}
