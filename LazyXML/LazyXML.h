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

namespace LazyXML
{

	class LazyXMLBase
	{
#define LAZY_XML_VAR(varType, varName) \
	private: varType varName; \
	public: const varType & get_##varName() const {return varName;} \
	void set_##varName(const varType & value) {varName = value;}

#include "LazyXML_VarTable.h"

#undef LAZY_XML_VAR

	private:
		static std::map<std::string, std::function<void(tinyxml2::XMLElement *)>> readFunctions;

		
	public:
		bool init();
		bool readFromFile(const std::string & fileName);
		bool readFromBuffer(const std::string & buffer);
		
	};

	/*template<typename T>
	void lazyQueryValue(tinyxml2::XMLElement * xmlElement, T * value)
	{
		std::cerr << "missing XML reading function for value type : " << typeid(T).name() << std::endl;
		assert(false);
	}*/

	/*template<typename T>
	struct is_vector
	{
	bool operator()()
	{
	return false;
	}
	};

	template<typename T, typename A>
	struct is_vector<std::vector<T, A>>
	{
	bool operator()()
	{
	return true;
	}
	};*/

	template<typename T>
	struct LazyReader
	{
		void operator()(tinyxml2::XMLElement * xmlElement, T & value)
		{
			std::cerr << "missing XML reading function for value type : " << typeid(T).name() << std::endl;
			assert(false);
		}
	};

	template<typename T>
	struct LazyReader < std::vector<T> >
	{
		void operator()(tinyxml2::XMLElement * xmlElement, std::vector<T> * value)
		{
			tinyxml2::XMLElement * childElement = xmlElement->FirstChildElement();
			while (childElement)
			{
				T childValue;
				LazyReader<T>()(childElement, &childValue);
				value->push_back(childValue);
				childElement = childElement->NextSiblingElement();
			}
		}
	};

	template<>
	struct LazyReader < bool >
	{
		void operator()(tinyxml2::XMLElement * xmlElement, bool * value)
		{
			xmlElement->QueryBoolText(value);
		}
	};

	template<>
	struct LazyReader < int >
	{
		void operator()(tinyxml2::XMLElement * xmlElement, int * value)
		{
			xmlElement->QueryIntText(value);
		}
	};

	template<>
	struct LazyReader < float >
	{
		void operator()(tinyxml2::XMLElement * xmlElement, float * value)
		{
			xmlElement->QueryFloatText(value);
		}
	};

	template<>
	struct LazyReader < double >
	{
		void operator()(tinyxml2::XMLElement * xmlElement, double * value)
		{
			xmlElement->QueryDoubleText(value);
		}
	};

	template<>
	struct LazyReader < std::string >
	{
		void operator()(tinyxml2::XMLElement * xmlElement, std::string * value)
		{
			*value = xmlElement->GetText();
		}
	};
}

#endif // _LAZY_XML_H