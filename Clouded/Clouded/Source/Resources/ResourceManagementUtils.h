#pragma once
#include <string>

inline std::string GetFilePathExtension(const std::string &FileName) {
	if (FileName.find_last_of(".") != std::string::npos)
		return FileName.substr(FileName.find_last_of(".") + 1);
	return "";
}