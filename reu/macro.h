#ifndef reu__macro_h
#define reu__macro_h

#include "_header.h"

/*** Class ***/

#define REU__PROPERTY(type, var, fnName)							\
protected:	type var;												\
public:		virtual type get##fnName() const {return var;}			\
public:		virtual void set##fnName(type var) {this->var = var;}

#define REU__PROPERTY_READONLY(type, var, fnName)					\
protected:	type var;												\
public:		virtual type get##fnName() const {return var;}

#define REU__PROPERTY_PASS_REF(type, var, fnName)					\
protected:	type var;												\
public:		virtual const type& get##fnName() const {return var;}	\
public:		virtual void set##fnName(type var) {this->var = var;}

#define REU__PROPERTY_READONLY_PASS_REF(type, var, fnName)			\
protected:	type var;												\
public:		virtual const type& get##fnName() const {return var;}

/*** Print ***/

#define REU__PRINT(str) \
	std::cout << str

#define REU__PRINTLN(str) \
	std::cout << str << std::endl

#define REU__NEWLINE \
	std::cout << std::endl

/* DEBUG */

#define REU_DEBUG__SENTENCE(type, var, sentence)	\
	type var = sentence; \
	std::cout << "DEBUG::" << __func__ << "::" << #sentence << "::" << var << std::endl

#define REU_DEBUG__PRINT(str) \
	std::cout << "DEBUG::" << __func__ << "::" << str << std::endl

#endif // reu__macro_h
