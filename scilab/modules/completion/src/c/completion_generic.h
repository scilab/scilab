/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __COMPLETION_GENERIC_H__
#define __COMPLETION_GENERIC_H__

/**
* completion
* @param[in] dictionary 
* @param[in] size of dictionary 
* @param[in] some chars of a symbol
* @param[out] size of returned strings
* @return strings found in dictionary
*/
char **completion_generic(char **dictionary,int sizedictionary,
						  char *somechars, int *sizeArrayReturned);

#endif /* __COMPLETION_GENERIC_H__ */
/*-----------------------------------------------------------------------------------*/
