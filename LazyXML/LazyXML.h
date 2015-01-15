#ifndef _LAZY_XML_H
#define _LAZY_XML_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <assert.h>
#include "tinyxml2.h"

#define VAR_TABLE_FILE "LazyXML_VarTable.h"

namespace LazyXML
{
	class Base
	{

#define LAZY_XML_VAR(varType, varName) \
	private: varType varName; \
	public: const varType & get_##varName() const {return varName;} \
	void set_##varName(const varType & value) {varName = value;}
#include VAR_TABLE_FILE
#undef LAZY_XML_VAR

	private:
		static std::map<std::string, std::function<void(tinyxml2::XMLElement *)>> readingFuncs;
		static std::map<std::string, std::function<void(tinyxml2::XMLElement *)>> writingFuncs;
		
	public:
		void init();

		void readFromFile(const std::string & fileName);
		void readFromBuffer(const std::string & buffer);
		
		void writeToBuffer(std::string & buffer);
		void writeToFile(const std::string & fileName);
	};

	class Singleton : public Base
	{
		Singleton() {}
		static Singleton * instance;
	public:
		static Singleton * getInstance()
		{
			if (!instance)
			{
				instance = new Singleton();
				instance->init();
			}
			return instance;
		}
	};

	template<typename T>
	struct VarType
	{
		static void reader(tinyxml2::XMLElement * xmlElement, T & value)
		{
			std::cerr << "missing XML reading function for value type : " << typeid(T).name() << std::endl;
			assert(false);
		}

		static void writer(tinyxml2::XMLElement * xmlElement, const T & value)
		{
			std::cerr << "missing XML writing function for value type : " << typeid(T).name() << std::endl;
			assert(false);
		}
	};

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