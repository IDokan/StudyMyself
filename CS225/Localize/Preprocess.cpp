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

/* Helper function declarations */
void ConvertLine(char const * filePath, const std::map<std::string, unsigned long>& map);
std::string ParseFileName(std::string filePath);
/* End of declarations */

void ConvertLine(char const* filePath, const std::map<std::string, unsigned long>& map)
{
    std::ifstream txtFile(filePath);
    std::ofstream datFile(ParseFileName(filePath) + ".dat");

    // Result Map STL that takes 
    std::map<unsigned long, std::string> resultMap;

    // Error check
    if (txtFile.is_open() == FAIL_TO_OPEN || datFile.is_open() == FAIL_TO_OPEN)
    {  
        return;
    }
    
    // Match text with given map
    std::string textLine;
    while (getline(txtFile, textLine))
    {
        // Find delimiter and divide a line with it.
        int delimiter = textLine.find('\t');
        std::string stringId = textLine.substr(0, delimiter);
        std::string displayText = textLine.substr(delimiter+1);
        
        // store in result map
        resultMap.emplace(map.at(stringId), displayText);
    }

    // After update result map, output result (Make file in here)
    for (const auto& it : map)
    {
        const auto& tmp = resultMap.find(it.second);
        
        // If fail to find,
        if (tmp == end(resultMap))
        {
            // result would be "stringIdNumber \t stringIdText"
            datFile << std::hex << it.second << '\t' << it.first << std::endl;
        }
        // If success,
        else
        {
            // result would be "stringIdNumber \t displayText"
            datFile << std::hex << it.second << '\t' << tmp->second << std::endl;
        }
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

    std::map<std::string, unsigned long> stringIds;

    unsigned long stringIdNumber = 0x10000;
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

        datFile << std::hex <<  stringIdNumber << '\t' << displayText << std::endl;
        stringsDefineFile << std::hex << "#define " << stringId << " " << stringIdNumber++ << std::endl;
    }

    for (const auto& it : stringIds)
    {
        std::cout << it.first << '\t' << it.second << std::endl;
    }
    

    while (argc > 2)
    {
        ConvertLine(argv[argc - 1], stringIds);
        --argc;
    }
    
    
    
    return 0;
}
