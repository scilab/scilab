/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "completion_wrap_java.h"
#include "completion.h"
/*--------------------------------------------------------------------------*/
char **searchAllDictionaries(char *somechars)
{
	int nbrElements = 0;
	return completion(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchFunctionsDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnFunctions(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchCommandsDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnCommandWords(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchMacrosDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnMacros(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchVariablesDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnVariables(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchFilesDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnFiles(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchHandleGraphicsPropertiesDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnHandleGraphicsProperties(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
