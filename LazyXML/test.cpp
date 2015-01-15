#include <iostream>

#include "LazyXML.h"

int main(int argc, const char ** argv)
{
	LazyXML::Singleton::getInstance()->readFromFile("test_config.xml");

	std::string buffer;
	LazyXML::Singleton::getInstance()->writeToBuffer(buffer);
	std::cout << buffer << std::endl;
}