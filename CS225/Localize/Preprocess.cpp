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
typedef  std::string STRING_ID_TEXT;
typedef unsigned long STRING_ID_NUMBER;
typedef std::string DISPLAY_STRING;

/* Helper function declarations */

void ConvertLine(char const * filePath, const std::map<STRING_ID_TEXT, STRING_ID_NUMBER>& map, STRING_ID_NUMBER maximumStringId);
std::string ParseFileName(std::string filePath);
void MakeDatFile(std::ofstream& outStream, const std::map<STRING_ID_TEXT, STRING_ID_NUMBER>& map, const std::map<STRING_ID_NUMBER, DISPLAY_STRING>& resultMap, STRING_ID_NUMBER maximumStringId);

/* End of declarations */

void MakeDatFile(std::ofstream& outStream, const std::map<STRING_ID_TEXT, STRING_ID_NUMBER>& map, const std::map<STRING_ID_NUMBER, DISPLAY_STRING>& resultMap, STRING_ID_NUMBER maximumStringId)
{
    for (STRING_ID_NUMBER i = INIT_STRING_ID_NUMBER; i < maximumStringId; i++)
    {
        /* code */
    }
    
    

    // Order is incorrect
    for (const auto& it : map)
    {
        const auto& tmp = resultMap.find(it.second);
        
        // If fail to find,
        if (tmp == end(resultMap))
        {
            // result would be "stringIdNumber \t stringIdText"
            outStream << std::hex << std::uppercase << it.second << '\t' << it.first << std::endl;
        }
        // If success,
        else
        {
            // result would be "stringIdNumber \t displayText"
            outStream << std::hex << std::uppercase << it.second << '\t' << tmp->second << std::endl;
        }
    }
}

void ConvertLine(char const* filePath, const std::map<STRING_ID_TEXT, STRING_ID_NUMBER>& map, STRING_ID_NUMBER maximumStringId)
{
    std::ifstream txtFile(filePath);

    // Result Map STL that takes 
    std::map<STRING_ID_NUMBER, DISPLAY_STRING> resultMap;

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
        STRING_ID_TEXT stringId = textLine.substr(0, delimiter);
        DISPLAY_STRING displayText = textLine.substr(delimiter+1);
        
        try
        {
            // store in result map
            resultMap.emplace(map.at(stringId), displayText);
        }
        // If map::at() throw exception,
        catch(const std::exception& e)
        {
            // Do nothing.
        }
    }


    // After update result map, output result (Make file in here)
    std::ofstream datFile(ParseFileName(filePath) + ".dat");
    if(datFile.is_open() == true)
    {
        MakeDatFile(datFile, map, resultMap, maximumStringId);
    }
}

std::string ParseFileName(std::string filePath)
{
    int fileNamePos = filePath.find('.');
    return filePath.substr(0, fileNamePos);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage : <%s> <txt file name> <Extra Files>\n", argv[0]);
        return 0;
    }

    std::string filePath = argv[1];

    // Open text file with given file path & Open dat file
    std::ifstream txtFile(filePath);
    std::ofstream datFile(ParseFileName(filePath) + ".dat");

    std::ofstream stringsDefineFile("String.h");

    // Error check if they are not opened
    if (
        txtFile.is_open() == FAIL_TO_OPEN || 
        datFile.is_open() == FAIL_TO_OPEN || 
        stringsDefineFile.is_open() == FAIL_TO_OPEN
        )
    {
        std::cout << "Unable to open the file : " << filePath << std::endl;
    }

    std::map<STRING_ID_TEXT, STRING_ID_NUMBER> stringIds;

    STRING_ID_NUMBER stringIdNumber = INIT_STRING_ID_NUMBER;
    // tmporary variable for take each line
    std::string textLine;
    while (getline(txtFile, textLine))
    {
        // Find delimiter and divide a line with it.
        int delimiter = textLine.find('\t');
        std::string stringId = textLine.substr(0, delimiter);
        std::string displayText = textLine.substr(delimiter+1);
    
        // Update map
        stringIds.insert(make_pair(stringId, stringIdNumber));

        datFile << std::hex << std::uppercase <<  stringIdNumber << '\t' << displayText << std::endl;
        stringsDefineFile << std::hex << std::uppercase << "#define " << stringId << " " << stringIdNumber << std::endl;

        // Update stringIdNumber
        ++stringIdNumber;
    }
    

    while (argc > 2)
    {
        ConvertLine(argv[argc - 1], stringIds, stringIdNumber - 1);
        --argc;
    }
    
    
    
    return 0;
}
