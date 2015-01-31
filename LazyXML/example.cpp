//************************************
// An example of how to use LazyXML
//************************************

#include <iostream>
#include "LazyXML.h"


//************************************
// this block should be put into a header file
#define LAZY_XML_DECLARATION	//set the flag for header
#include "LazyXML_Def.h"	//include the macro helper file (could copy the content here too)

LAZY_XML_BEGIN(TestXML)		//begin class declaration

//declare all the vars
LAZY_XML_VAR(bool, TestVar0)
LAZY_XML_VAR(int, TestVar1)
LAZY_XML_VAR(float, TestVar2)
LAZY_XML_VAR(double, TestVar3)
LAZY_XML_VAR(std::vector<int>, TestVar4)

LAZY_XML_END	//end class declaration
//
//************************************


//************************************
// this block should be put into a source file
#undef LAZY_XML_DECLARATION		//unset the flag for header
#include "LazyXML_Def.h"	//include the macro helper file (could copy the content here too)

//copied over from the header file
LAZY_XML_BEGIN(TestXML)
LAZY_XML_VAR(bool, TestVar0)
LAZY_XML_VAR(int, TestVar1)
LAZY_XML_VAR(float, TestVar2)
LAZY_XML_VAR(double, TestVar3)
LAZY_XML_VAR(std::vector<int>, TestVar4)
LAZY_XML_END
//
//************************************


int main(int argc, const char ** argv)
{
	TestXML::getInstance()->readFromFile("example.xml");	//read from file
	//TestXML::getInstance()->get_TestVar0();	//get a var
	//TestXML::getInstance()->set_TestVar2(0);	//set a var
	std::string buffer;
	TestXML::getInstance()->writeToBuffer(buffer);		//write to buffer
	std::cout << buffer << std::endl;
}