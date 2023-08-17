#include <fstream>
#include <sstream>
#include <iostream>

#include <General/Utils.h>

namespace utils
{

const std::string FetchDataFromFile(const std::string& sFilePath)
{
	std::ifstream fin(sFilePath, std::ios::in);

	if (!fin.is_open())
	{
		// warning
		return std::string("");
	}

	std::stringstream buffer;
	buffer << fin.rdbuf();
	fin.close();
	
	return std::string(buffer.str());
}

bool ParseJsonFile(const std::string& sFilePath, json& outJson)
{
	const std::string sJsonString = FetchDataFromFile(sFilePath);

	if (sJsonString.empty())
	{
		std::cout << "Error! No JSON objects in file. File path: " << sFilePath << "." << std::endl;
		return false;
	}

	outJson = json::parse(sJsonString);
	
	if (!outJson.is_object())
	{
		std::cout << "JSON parse error." << std::endl;
		std::cout << "File path: " << sFilePath << std::endl;
		return false;
	}

	return true;
}

} // utils
