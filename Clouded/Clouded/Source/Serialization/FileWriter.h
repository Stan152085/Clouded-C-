#pragma once
#include <string>

class FileWriter
{
public:
	FileWriter() {};
	FileWriter(const std::string& a_rFileName);
	~FileWriter();

	const bool Open(const std::string& a_rFileName);
	const bool Close();
	const bool Write(void* a_pBuf, size_t a_size, size_t a_pos);
	const bool Append(void* a_pBuf, size_t a_size);
	const bool OpenEmpty(const std::string& a_rFileName);

private:
	FILE * m_pFile;
	size_t m_FileSize;
};