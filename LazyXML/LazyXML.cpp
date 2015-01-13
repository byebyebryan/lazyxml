#include "LazyXML.h"

using namespace tinyxml2;

std::map<std::string, std::function<void(XMLElement *)>> LazyXML::LazyXMLBase::readingFuncs;
std::map<std::string, std::function<void(XMLElement *)>> LazyXML::LazyXMLBase::writingFuncs;

bool LazyXML::LazyXMLBase::init()
{
#define LAZY_XML_VAR(varType, varName) \
	readingFuncs[#varName] = std::bind(&LazyVarType<varType>::reader, std::placeholders::_1, &varName); \
	writingFuncs[#varName] = std::bind(&LazyVarType<varType>::writer, std::placeholders::_1, #varName, std::cref(varName));

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
		auto it = readingFuncs.find(xmlNode->Name());
		if (it != readingFuncs.end())
		{
			it->second(xmlNode);
		}
		xmlNode = xmlNode->NextSiblingElement();
	}
	delete xmlDoc;
	return true;
}

bool LazyXML::LazyXMLBase::writeToBuffer(std::string & buffer)
{
	XMLDocument * xmlDoc = new XMLDocument();
	XMLDeclaration * xmlDecl = xmlDoc->NewDeclaration();
	xmlDoc->InsertFirstChild(xmlDecl);
	XMLElement * rootElement = xmlDoc->NewElement("root");
	xmlDoc->InsertEndChild(rootElement);

	for (auto & it : writingFuncs)
	{
		it.second(rootElement);
	}

	XMLPrinter xmlPrinter;
	xmlDoc->Print(&xmlPrinter);
	buffer = std::string(xmlPrinter.CStr());
	return true;
}

bool LazyXML::LazyXMLBase::writeToFile(const std::string & fileName)
{
	std::string buffer;
	writeToBuffer(buffer);
	std::ofstream fileStream(fileName);
	fileStream << buffer;
	return true;
}