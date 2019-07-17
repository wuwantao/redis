#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include "CL_Config.h"

CL_Config::CL_Config()
{
}

CL_Config::~CL_Config()
{
}

bool CL_Config::Load(const std::string& vPath)
{
	if(false == boost::filesystem::exists(vPath))
		return false;

	read_xml(vPath, m_Ptree);
	return true;
}

bool CL_Config::Save(const std::string& vPath)
{
	write_xml(vPath, m_Ptree);
	return true;
}

bool CL_Config::Get(const std::string& vKeyPath, std::string* vRetValue)
{
	*vRetValue = m_Ptree.get<std::string>(vKeyPath);
	return true;
}

bool CL_Config::Get(const std::string& vKeyPath, int* vRetValue)
{
	std::string tValue = m_Ptree.get<std::string>(vKeyPath);
	*vRetValue = atoi(tValue.c_str());
	return true;
}

bool CL_Config::Set(const std::string& vKeyPath, const std::string& vValue)
{
	return false;
}

bool CL_Config::Set(const std::string& vKeyPath, int vValue)
{
	return false;
}
