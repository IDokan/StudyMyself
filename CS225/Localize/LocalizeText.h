/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LocalizeText.h
Purpose: Header file for assignment 5 localize
Project: sinil.gang CS225 Assignment5 Localize 
Author: Sinil Kang
Creation date: 11/26/2019
- End Header ----------------------------------------------------------------
*/ 
#pragma once
#include <string>
#include <map>

class LocalizedText
{
private:
    #define HEXADECIMAL 16
    #define DELIMITER '\t'
public:
    explicit LocalizedText(std::string dataFile);

    std::string ToString(long string_id_number);

private:
    std::map<long, std::string> idDisplayMap;
};