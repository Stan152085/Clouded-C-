#pragma once
#include <memory>
#include <string>

struct  FileHandle
{
	std::shared_ptr<char> pHandle;
	size_t size;
};

class FileReader
{
public:
	FileReader() {};
	~FileReader();
	FileHandle Open(const std::string& file_name);
};