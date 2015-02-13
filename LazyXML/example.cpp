//************************************
// An example of how to use LazyXML
//************************************

//Better to put these in a header file
#include "LazyXML.h"

//Extend from lazy::Base class
class ExampleXML : public lazyxml::Base < ExampleXML > {
	//List variables
	LAZY_XML_VAR(ExampleXML, bool, TestVar0)
	LAZY_XML_VAR(ExampleXML, int, TestVar1)
	LAZY_XML_VAR(ExampleXML, float, TestVar2)
	LAZY_XML_VAR(ExampleXML, double, TestVar3)
	LAZY_XML_VAR(ExampleXML, std::vector<int>, TestVar4)
};

#include <iostream>

int main(int argc, const char ** argv)
{
	//read from file
	ExampleXML::GetInstance()->ReadFromFile("example.xml");

	//getters and setters
	bool var_0 = ExampleXML::GetInstance()->TestVar0();
	ExampleXML::GetInstance()->set_TestVar1(1);

	//write to buffer
	std::string buffer;
	ExampleXML::GetInstance()->WriteToBuffer(buffer);
	std::cout << buffer << std::endl;
}