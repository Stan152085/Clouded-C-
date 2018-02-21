#include "Utilities\Serialization.h"
#include "Utilities\FileWriter.h"
#include "Utilities\FileReader.h"

#include "Utilities\Json\json.h"

// serializable structs
#include "Shading/Color.h"
#include "Math\Vec2.h"
#include "Math\Vec3.h"

//-----------------------------------------------
// File IO

Serializer::Serializer(const std::string& a_rRoot)
	:
	m_root(a_rRoot)
{
	m_values.push(new Json::Value());
	StepIn(a_rRoot);
}

Serializer::~Serializer()
{
	while (m_values.size() > 1)
	{
		m_values.pop();
	}
	delete m_values.top();
	m_values.pop();
}

ErrorCodes Serializer::Serialize(const std::string& filePath)
{
	FileWriter writer;
	if (true != writer.OpenEmpty(filePath))
	{
		return ErrorCodes::FAILURE;
	}

	Json::StyledWriter JsonWiter;

	// Restore root
	if (m_values.empty() == true)
	{
		return ErrorCodes::FAILURE;
	}
	while (m_values.size() > 1)
	{
		m_values.pop();
	}

	// Open root
	std::string buf = JsonWiter.write(Root());
	size_t size = buf.size();

	writer.Write((void*)buf.c_str(), size, 0);
	writer.Close();

	// Step back into root
	StepIn(m_root.c_str());

	return ErrorCodes::SUCCESS;
}

ErrorCodes Serializer::Deserialize(const std::string& filePath)
{
	FileReader reader;
	FileHandle handle = reader.Open(filePath);

	if (handle.pHandle == nullptr)
	{
		return ErrorCodes::FAILURE;
	}

	Json::Reader Jsonreader;

	// Clear stack
	while (m_values.size() > 1)
	{
		m_values.pop();
	}

	// Write from root
	if (Jsonreader.parse(handle.pHandle.get(), handle.pHandle.get() + handle.size, Root()) == false)
	{
		return ErrorCodes::FAILURE;
	}

	// Step back into root
	StepIn(m_root.c_str());

	return ErrorCodes::SUCCESS;
}

//--------------------------------------------
// step functions

void Serializer::StepIn(const std::string& a_rName)
{
	Json::Value* pNewRoot = &(Root())[a_rName];
	m_values.push(pNewRoot);
}

void Serializer::StepOut()
{
	// Preserve absolute root
	if (m_values.size() == 1) return;

	m_values.pop();
}

//--------------------------------------------
// Containers

void Serializer::Add(const std::string& a_rName, float a_value)
{
	Root()[a_rName] = a_value;
}

void Serializer::Add(const std::string& a_rName, bool a_value)
{
	Root()[a_rName] = a_value;
}

void Serializer::Add(const std::string& a_rName, double a_value)
{
	Root()[a_rName] = a_value;
}

void Serializer::Add(const std::string& a_rName, int a_value)
{
	Root()[a_rName] = a_value;
}

void Serializer::Add(const std::string& a_rName, size_t a_value)
{
	Root()[a_rName] = a_value;
}

void Serializer::Add(const std::string& a_rName, const Vec2& a_rValue)
{
	StepIn(a_rName);

	Root()["x"] = a_rValue.x;
	Root()["y"] = a_rValue.y;

	StepOut();
}

void Serializer::Add(const std::string& a_rName, const Vec3& a_rValue)
{
	StepIn(a_rName);

	Root()["x"] = a_rValue.x;
	Root()["y"] = a_rValue.y;
	Root()["z"] = a_rValue.z;

	StepOut();
}

void Serializer::Add(const std::string& a_rName, const Color & a_rValue)
{
	StepIn(a_rName);

	Root()["r"] = a_rValue.r;
	Root()["g"] = a_rValue.g;
	Root()["b"] = a_rValue.b;

	StepOut();
}

void Serializer::Add(const std::string& a_rName, const std::string& a_rValue)
{
	Root()[a_rName] = a_rValue;
}


//---------------------------------------------------------
// Getters

void Serializer::Get(const std::string& a_rName, float& a_rValue)
{
	a_rValue = Root()[a_rName].asFloat();
}

void Serializer::Get(const std::string& a_rName, bool& a_rValue)
{
	a_rValue = Root()[a_rName].asBool();
}

void Serializer::Get(const std::string& a_rName, double& a_rValue)
{
	a_rValue = Root()[a_rName].asDouble();
}

void Serializer::Get(const std::string& a_rName, int& a_rValue)
{
	a_rValue = Root()[a_rName].asInt();
}

void Serializer::Get(const std::string& a_rName, size_t& a_rValue)
{
	a_rValue = (size_t)Root()[a_rName].asInt64();
}

void Serializer::Get(const std::string& a_rName, Vec2& a_rValue)
{
	StepIn(a_rName);

	a_rValue.x = Root()["x"].asFloat();
	a_rValue.y = Root()["y"].asFloat();

	StepOut();
}

void Serializer::Get(const std::string& a_rName, Vec3& a_rValue)
{
	StepIn(a_rName);

	a_rValue.x = Root()["x"].asFloat();
	a_rValue.y = Root()["y"].asFloat();
	a_rValue.z = Root()["z"].asFloat();

	StepOut();
}

void Serializer::Get(const std::string& a_rName, Color & a_rValue)
{
	StepIn(a_rName);

	a_rValue.r = Root()["r"].asFloat();
	a_rValue.g = Root()["g"].asFloat();
	a_rValue.b = Root()["b"].asFloat();

	StepOut();
}

void Serializer::Get(const std::string& a_rName, std::string& a_rValue)
{
	a_rValue = Root()[a_rName].asCString();
}
