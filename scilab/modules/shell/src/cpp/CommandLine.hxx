/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__
/*------------------------------------------------------------------------*/
extern "C"
{
#include "machine.h" /* BOOL */
};
#include <string>
/*------------------------------------------------------------------------*/
using namespace std ;
/*------------------------------------------------------------------------*/
class CommandLine
{
public:
	/**
	* Constructor CommandLine
	*/
	CommandLine(void);

	/**
	* Constructor CommandLine
	* @param line_ : a string 
	*/
	CommandLine(std::string line_);

	/**
	* Destructor CommandLine
	*/
	~CommandLine();

	/**
	* get command line
	* @return a string
	*/
	std::string get(void);

	/**
	* set command line
	* @param line_ : a string 
	* @return TRUE or FALSE
	*/
	BOOL set(std::string line_);
	
protected:

private:
	std::string Command;
};
/*------------------------------------------------------------------------*/
#endif /* __COMMANDLINE_H__ */
/*------------------------------------------------------------------------*/
