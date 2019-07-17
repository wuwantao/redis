#ifndef CL_Config_H_
#define CL_Config_H_

#include <boost/property_tree/ptree.hpp>

class CL_Config
{
private:
	boost::property_tree::ptree m_Ptree;

public:
	CL_Config(void);
	~CL_Config();

	bool Load(const std::string& vPath);
	bool Save(const std::string& vPath);

	bool Get(const std::string& vKeyPath, std::string* vRetValue);
	bool Get(const std::string& vKeyPath, int* vRetValue);

	bool Set(const std::string& vKeyPath, const std::string& vValue);
	bool Set(const std::string& vKeyPath, int vValue);
};
#endif
