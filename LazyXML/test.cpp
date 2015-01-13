#include <iostream>

#include "LazyXML.h"

int main(int argc, const char ** argv)
{
	auto lazyXML = new LazyXML::LazyXMLBase();
	lazyXML->init();
	lazyXML->readFromFile("test_config.xml");

	std::string buffer;
	lazyXML->writeToFile("test_out.xml");

	std::cout << "Okay" << std::endl;
}