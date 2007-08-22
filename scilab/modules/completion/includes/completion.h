/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/

#ifndef __COMPLETION_H__
#define __COMPLETION_H__

/**
 * completion function on all types for scilab
 * @param[in] somechars first characters of a word
 * @param[out] sizeArrayReturned
 * @return a array of chars 
 */
char **completion(char *somechars, int *sizeArrayReturned);

/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return a array of chars 
*/
char **completionOnFunctions(char *somechars, int *sizeArrayReturned);

/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return a array of chars 
*/
char **completionOnCommandWords(char *somechars, int *sizeArrayReturned);

/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return a array of chars 
*/
char **completionOnMacros(char *somechars, int *sizeArrayReturned);

/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return a array of chars 
*/
char **completionOnVariables(char *somechars, int *sizeArrayReturned);

/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return a array of chars 
*/
char **completionOnFiles(char *somechars, int *sizeArrayReturned);


/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return a array of chars 
*/
char **completionOnHandleGraphicsProperties(char *somechars, int *sizeArrayReturned);
/*-----------------------------------------------------------------------------------*/
#endif /* __COMPLETION_H__ */
