#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace utils
{
	template<class T>
	std::string ClassName()
	{
		#if __GNUC__
			return std::string(typeid(T).name()).substr(2);
		#else
			return std::string(typeid(T).name());
		#endif
	}

	const std::string FetchDataFromFile(const std::string& sFilePath);
	bool ParseJsonFile(const std::string& sFilePath, json& outJson);
}
