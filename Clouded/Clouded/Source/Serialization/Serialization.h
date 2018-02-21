#pragma once
#include <string>
#include <stack>

namespace Json
{
	class Value;
}

enum ErrorCodes
{
	FAILURE = 0,
	SUCCESS = 1
};

// class for deserializing data.
// this class will serialize any values added with the Add function and deserialize a file
// a file and put the parameters in the paramters map. you can then fetch all deserialized 
// values using the GetParameter() functions
class Serializer
{
public:
	Serializer(const std::string& a_rRoot);
	~Serializer();

	ErrorCodes Serialize(const std::string& filePath);
	ErrorCodes Deserialize(const std::string& filePath);

	void StepIn(const std::string& a_rName);
	void StepOut();

	void Add(const std::string& a_rName, float a_value);
	void Add(const std::string& a_rName, bool a_value);
	void Add(const std::string& a_rName, double a_value);
	void Add(const std::string& a_rName, int a_value);
	void Add(const std::string& a_rName, size_t a_value);
	void Add(const std::string& a_rName, const std::string& a_rValue);

	void Get(const std::string& a_rName, float& a_value);
	void Get(const std::string& a_rName, bool& a_value);
	void Get(const std::string& a_rName, double& a_value);
	void Get(const std::string& a_rName, int& a_value);
	void Get(const std::string& a_rName, size_t& a_value);
	void Get(const std::string& a_rName, std::string& a_rValue);

private:
	inline Json::Value& Root()
	{
		return *m_values.top();
	}
	std::stack<Json::Value*> m_values;
	std::string m_root;
};