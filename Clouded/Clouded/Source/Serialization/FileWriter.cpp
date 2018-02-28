#include "stdafx.h"
#include "FileWriter.h"

FileWriter::FileWriter(const std::string& a_rFileName)
{
    Open(a_rFileName);
}

FileWriter::~FileWriter()
{
	Close();
}

const bool FileWriter::Open(const std::string& a_rFileName)
{
    m_pFile = fopen(a_rFileName.c_str(), "ab+");

    if (m_pFile == nullptr)
    {
		assert(false && "could not open file");
        return false;
    }

    fseek(m_pFile, 0, SEEK_END);
    m_FileSize = ftell(m_pFile);
    fseek(m_pFile, 0, SEEK_SET);
    return true;
}

const bool FileWriter::Close()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = nullptr;
        return true;
    }
    return false;
}

const bool FileWriter::Write(void * a_pBuf, size_t a_size, size_t a_pos)
{
    if (a_pos > m_FileSize)
    {
		assert(false && "tried to write past the end of the file");
        return false;
    }

    fseek(m_pFile, (long)a_pos, SEEK_SET);
    fwrite(a_pBuf, a_size, 1, m_pFile);
    m_FileSize += a_pos + a_size - m_FileSize;
    return true;
}

const bool FileWriter::Append(void * a_pBuf, size_t a_size)
{
    fwrite(a_pBuf, a_size, 1, m_pFile);
    m_FileSize += a_size;
    return true;
}

const bool FileWriter::OpenEmpty(const std::string& a_rFileName)
{
    m_pFile = fopen(a_rFileName.c_str(), "w");

    if (m_pFile == nullptr)
    {
		assert(false && "could not open file");
        return false;
    }

    fseek(m_pFile, 0, SEEK_END);
    m_FileSize = ftell(m_pFile);
    fseek(m_pFile, 0, SEEK_SET);
    return true;
}
