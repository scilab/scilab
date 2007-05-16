#ifndef __COMPLETION_H__
#define __COMPLETION_H__
/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET - INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
/**
 * completion function 
 * @param somechars first characters of a word
 * @param sizeArrayReturned
 * @return a array of chars 
 */
char **completion(char *somechars, int *sizeArrayReturned);
/*-----------------------------------------------------------------------------------*/
#endif /* __COMPLETION_H__ */
