#include <iostream>

#include "LazyXML.h"

int main(int argc, const char ** argv)
{
	std::cout << "Okay" << std::endl;

	auto lazyXML = new LazyXML::LazyXMLBase();
	lazyXML->init();
	lazyXML->readFromFile("test_config.xml");

}