#include <iostream>
#include <fstream>
#include <sstream>

/*
#include <System/Parser.h>

void Parser::setDirectoryPath(const std::string& path) 
{
	directoryPath = path;
	for (int i = 0; i < 3; ++i)
	{
		size_t found = directoryPath.find_last_of("/\\");
		directoryPath = directoryPath.substr(0, found);
	}
	directoryPath += "/";
}

const std::string Parser::getDirectoryPath() 
{ 
	return directoryPath; 
}

const std::string Parser::getFileString(const std::string& filePath)
{
	std::ifstream fin(filePath, std::ios::in);

	if (!fin.is_open())
		return std::string("");

	std::stringstream buffer;
	buffer << fin.rdbuf();
	return std::string(buffer.str());
}

void Parser::parseJsonFile(const std::string& filePath, nlohmann::json& outJson)
{
	const std::string JSONString = getFileString(filePath);

	if (JSONString.empty())
	{
		std::cout << "Error! No JSON objects in file. File path: " << filePath << "." << std::endl;
		exit(-1);
	}

	outJson = nlohmann::json::parse(JSONString);
	
	if (!outJson.is_object())
	{
		std::cout << "JSON parse error." << std::endl;
		std::cout << "File path: " << filePath << std::endl;
		exit(-1);
	}
}

std::string Parser::directoryPath;

*/
