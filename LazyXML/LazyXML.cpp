#include "LazyXML.h"

using namespace tinyxml2;

std::map<std::string, std::function<void(XMLElement *)>> LazyXML::LazyXMLBase::readFunctions;

bool LazyXML::LazyXMLBase::init()
{
#define LAZY_XML_VAR(varType, varName) \
	readFunctions[#varName] = std::bind(LazyReader<varType>(), std::placeholders::_1, &varName);

#include "LazyXML_VarTable.h"

#undef LAZY_XML_VAR
	return true;
}

bool LazyXML::LazyXMLBase::readFromFile(const std::string & fileName)
{
	std::ifstream fileStream(fileName);
	std::stringstream stringBuffer;
	stringBuffer << fileStream.rdbuf();
	return readFromBuffer(stringBuffer.str());
}

bool LazyXML::LazyXMLBase::readFromBuffer(const std::string & buffer)
{
	XMLDocument * xmlDoc = new XMLDocument();
	xmlDoc->Parse(buffer.c_str());
	XMLElement * xmlNode = xmlDoc->RootElement()->FirstChildElement();
	while (xmlNode)
	{
		auto it = readFunctions.find(xmlNode->Name());
		if (it != readFunctions.end())
		{
			it->second(xmlNode);
		}
		xmlNode = xmlNode->NextSiblingElement();
	}
	delete xmlDoc;
	return true;
}

#include "tinyxml2.cpp"