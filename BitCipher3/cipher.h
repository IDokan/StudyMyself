/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Cipher.h
Purpose: This is the interface file to be used for the bit cipher assignment
Project: CS225 BitCipher
Author: Kevin Wright
Creation date: 8/20/2019
- End Header ----------------------------------------------------------------
*/

std::vector<char> encode(std::string uncompressed );
std::string decode(std::vector<char>);
