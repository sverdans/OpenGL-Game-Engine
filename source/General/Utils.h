#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace utils
{
	const std::string FetchDataFromFile(const std::string& sFilePath);
	bool ParseJsonFile(const std::string& sFilePath, nlohmann::json& outJson);
}
