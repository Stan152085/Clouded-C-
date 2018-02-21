#include "stdafx.h"
#include "FileReader.h"
#include <fstream>

FileReader::~FileReader()
{
}


FileHandle FileReader::Open(const std::string& a_rFileName)
{
	FileHandle ret; 
	FILE* pFile = fopen(a_rFileName.c_str(), "rb");
	if (pFile == nullptr)
	{
		assert(false && "could not open the file");
        return ret;
	}
	// Check if we've already loaded it
	bool cached = false;
	if (cached == true)
	{
		// Handle cached files
		return ret;
	}
	fseek(pFile, 0, SEEK_END);
	ret.size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	ret.pHandle = std::shared_ptr<char>(new char[ret.size],
		std::default_delete<char[]>());
	fread(ret.pHandle.get(), sizeof(char), ret.size, pFile);
	fclose(pFile);
	return ret;
}
