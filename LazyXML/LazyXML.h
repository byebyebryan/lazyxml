/*
Bryan B.
byebyebryan@gmail.com

simple & stupid c++ auto xml serialization based on tinyxml2
*/

#ifndef _LAZY_XML_H
#define _LAZY_XML_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <assert.h>
#include "tinyxml2.h"

//************************************
// Begin class declaration
//  -declare basic structure for singleton
//  -declare init()
//************************************
#define LAZY_XML_BEGIN_DECLARATION(className) \
class className : public LazyXML::Base { \
	private: className() {} \
	static className * instance; \
	public: static className * getInstance(); \
	static void destoryInstance(); \
	protected: void init() override;

//************************************
// End class declaration
//************************************
#define LAZY_XML_END_DECLARATION };

//************************************
// Begin class implementation
//	-implement basic function for singleton
//	-begin implementation of init()
//************************************
#define LAZY_XML_BEGIN_IMPLEMENTATION(className) \
className * className::instance = nullptr; \
className * className::getInstance() { \
	if (!instance) {instance = new className(); instance->init();} \
	return instance; \
} \
void className::destoryInstance() { \
	if (instance) {delete instance; instance = nullptr;} \
} \
void className::init() {

//************************************
// End class implementation
//************************************
#define LAZY_XML_END_IMPLEMENTATION }

namespace LazyXML
{
	class Base
	{
	protected:
		//************************************
		// map of var name to reading/writing functions
		//  -function should be already binded to storage of var
		//************************************
		std::unordered_map<std::string, std::function<void(tinyxml2::XMLElement *)>> readingFuncs;
		std::unordered_map<std::string, std::function<void(tinyxml2::XMLElement *)>> writingFuncs;

		//************************************
		// FullName:  LazyXML::Base::init
		// Access:    virtual protected 
		// Returns:   void
		//
		// Summary:   populate read/write function maps
		//************************************
		virtual void init() = 0;

	public:

		//************************************
		// FullName:  LazyXML::Base::readFromFile
		// Access:    public 
		// Returns:   void
		// Parameter: const std::string & fileName
		//
		// Summary:   read from file
		//************************************
		void readFromFile(const std::string & fileName);

		//************************************
		// FullName:  LazyXML::Base::readFromBuffer
		// Access:    public 
		// Returns:   void
		// Parameter: const std::string & buffer
		//
		// Summary:   read from buffer
		//************************************
		void readFromBuffer(const std::string & buffer);
		
		//************************************
		// FullName:  LazyXML::Base::writeToBuffer
		// Access:    public 
		// Returns:   void
		// Parameter: std::string & buffer
		//
		// Summary:   write to buffer
		//************************************
		void writeToBuffer(std::string & buffer);


		//************************************
		// FullName:  LazyXML::Base::writeToFile
		// Access:    public 
		// Returns:   void
		// Parameter: const std::string & fileName
		//
		// Summary:   write to file
		//************************************
		void writeToFile(const std::string & fileName);
	};

	//************************************
	// container struct to provide read/write function for generic type T
	//  -default implementation block execution
	//  -specialize T to override
	//************************************
	template<typename T>
	struct VarType
	{
		static void reader(tinyxml2::XMLElement * xmlElement, T & value)
		{
			std::cerr << "missing XML reading function for value type : " << typeid(T).name() << std::endl;
			assert(false);
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const T & value)
		{
			std::cerr << "missing XML writing function for value type : " << typeid(T).name() << std::endl;
			assert(false);
		}
	};

	//************************************
	// partially specialized read/write for container type std::vector<T>
	//  -calls fully specialized read/write for type T
	//************************************
	template<typename T>
	struct VarType < std::vector<T> >
	{
		static void reader(tinyxml2::XMLElement * xmlElement, std::vector<T> * value)
		{
			tinyxml2::XMLElement * childElement = xmlElement->FirstChildElement();
			while (childElement)
			{
				T childValue;
				VarType<T>::reader(childElement, &childValue);
				value->push_back(childValue);
				childElement = childElement->NextSiblingElement();
			}
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const std::vector<T> & value)
		{
			tinyxml2::XMLElement * newElement = xmlElement->GetDocument()->NewElement(name.c_str());
			for (auto & item : value)
			{
				VarType<T>::writer(newElement, "item", item);
			}
			xmlElement->InsertEndChild(newElement);
		}
	};

	//************************************
	// fully specialized read/write for bool
	//************************************
	template<>
	struct VarType < bool >
	{
		static void reader(tinyxml2::XMLElement * xmlElement, bool * value)
		{
			xmlElement->QueryBoolText(value);
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const bool & value)
		{
			tinyxml2::XMLElement * newElement = xmlElement->GetDocument()->NewElement(name.c_str());
			newElement->SetText(value);
			xmlElement->InsertEndChild(newElement);
		}
	};

	//************************************
	// fully specialized read/write for int
	//************************************
	template<>
	struct VarType < int >
	{
		static void reader(tinyxml2::XMLElement * xmlElement, int * value)
		{
			xmlElement->QueryIntText(value);
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const int & value)
		{
			tinyxml2::XMLElement * newElement = xmlElement->GetDocument()->NewElement(name.c_str());
			newElement->SetText(value);
			xmlElement->InsertEndChild(newElement);
		}
	};

	//************************************
	// fully specialized read/write for float
	//************************************
	template<>
	struct VarType < float >
	{
		static void reader(tinyxml2::XMLElement * xmlElement, float * value)
		{
			xmlElement->QueryFloatText(value);
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const float & value)
		{
			tinyxml2::XMLElement * newElement = xmlElement->GetDocument()->NewElement(name.c_str());
			newElement->SetText(value);
			xmlElement->InsertEndChild(newElement);
		}
	};

	//************************************
	// fully specialized read/write for double
	//************************************
	template<>
	struct VarType < double >
	{
		static void reader(tinyxml2::XMLElement * xmlElement, double * value)
		{
			xmlElement->QueryDoubleText(value);
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const double & value)
		{
			tinyxml2::XMLElement * newElement = xmlElement->GetDocument()->NewElement(name.c_str());
			newElement->SetText(value);
			xmlElement->InsertEndChild(newElement);
		}
	};

	//************************************
	// fully specialized read/write for std::string
	//************************************
	template<>
	struct VarType < std::string >
	{
		static void reader(tinyxml2::XMLElement * xmlElement, std::string * value)
		{
			*value = xmlElement->GetText();
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const std::string & name, const std::string & value)
		{
			tinyxml2::XMLElement * newElement = xmlElement->GetDocument()->NewElement(name.c_str());
			newElement->SetText(value.c_str());
			xmlElement->InsertEndChild(newElement);
		}
	};
	
}

#endif // _LAZY_XML_H