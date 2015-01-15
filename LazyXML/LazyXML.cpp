#include "LazyXML.h"

using namespace tinyxml2;

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