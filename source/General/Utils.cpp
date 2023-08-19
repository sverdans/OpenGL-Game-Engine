#include <fstream>
#include <sstream>
#include <iostream>

#include <General/Utils.h>
#include <spdlog/spdlog.h>

namespace utils
{

const std::string FetchDataFromFile(const std::string& sFilePath)
{
	std::ifstream fin(sFilePath, std::ios::in);

	if (!fin.is_open())
	{
		spdlog::get("main")->warn("Can not read '{}' file", sFilePath);
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
		spdlog::get("main")->warn("No JSON data in file '{}'", sFilePath);
		return false;
	}

	try
	{
		outJson = json::parse(sJsonString);
	}
	catch (const std::exception& e)
	{
		spdlog::get("main")->warn("json parse error from file '{}' : {}", sFilePath, e.what());
		return false;
	}

	return true;
}

} // utils
