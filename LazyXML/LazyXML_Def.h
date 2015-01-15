//************************************
// Macros providing structure/function for individual var
//************************************

#ifdef LAZY_XML_DECLARATION		//switch between declaration/implementation
//************************************
// declare storage for a var and provide auto getter & setter
//************************************
#undef LAZY_XML_VAR
#define LAZY_XML_VAR(varType, varName) \
	private: varType varName; \
	public: const varType & get_##varName() const {return varName;} \
	void set_##varName(const varType & value) {varName = value;}
#undef LAZY_XML_BEGIN
#define LAZY_XML_BEGIN(className) LAZY_XML_BEGIN_DECLARATION(className)
#undef LAZY_XML_END
#define LAZY_XML_END LAZY_XML_END_DECLARATION
#else
//************************************
// bind a var to a pair of read/write functions and add to the function map
//************************************
#undef LAZY_XML_VAR
#define LAZY_XML_VAR(varType, varName) \
	varName = varType(); \
	readingFuncs[#varName] = std::bind(&LazyXML::VarType<varType>::reader, std::placeholders::_1, &varName); \
	writingFuncs[#varName] = std::bind(&LazyXML::VarType<varType>::writer, std::placeholders::_1, #varName, std::cref(varName));
#undef LAZY_XML_BEGIN
#define LAZY_XML_BEGIN(className) LAZY_XML_BEGIN_IMPLEMENTATION(className)
#undef LAZY_XML_END
#define LAZY_XML_END LAZY_XML_END_IMPLEMENTATION
#endif