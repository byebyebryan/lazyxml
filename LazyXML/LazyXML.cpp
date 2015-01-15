#include "LazyXML.h"

using namespace tinyxml2;

std::map<std::string, std::function<void(tinyxml2::XMLElement *)>> LazyXML::Base::readingFuncs;
std::map<std::string, std::function<void(tinyxml2::XMLElement *)>> LazyXML::Base::writingFuncs;

void LazyXML::Base::init()
{
#define LAZY_XML_VAR(varType, varName) \
	readingFuncs[#varName] = std::bind(&VarType<varType>::reader, std::placeholders::_1, &varName); \
	writingFuncs[#varName] = std::bind(&VarType<varType>::writer, std::placeholders::_1, #varName, std::cref(varName));
#include VAR_TABLE_FILE
#undef LAZY_XML_VAR
}

void LazyXML::Base::readFromFile(const std::string & fileName)
{
	std::ifstream fileStream(fileName);
	std::stringstream stringBuffer;
	stringBuffer << fileStream.rdbuf();
	readFromBuffer(stringBuffer.str());
}

void LazyXML::Base::readFromBuffer(const std::string & buffer)
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
}

void LazyXML::Base::writeToBuffer(std::string & buffer)
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
}

void LazyXML::Base::writeToFile(const std::string & fileName)
{
	std::string buffer;
	writeToBuffer(buffer);
	std::ofstream fileStream(fileName);
	fileStream << buffer;
}

LazyXML::Singleton * LazyXML::Singleton::instance = nullptr;