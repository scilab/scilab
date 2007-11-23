/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __TOOLSDICTIONARY_H__
#define __TOOLSDICTIONARY_H__

#include "machine.h"

/**
* Append Datas to a dictionary(string matrix)
* @param dictionary 
* @param i
* @param datas
* @param sizedatas
* @return TRUE or FALSE
*/
BOOL appendDictionary(char ***dictionary,int *i,char ***datas,int *sizedatas);

/**
* sort dictionary
* @param strings wto sort
* @param  size of new dictionnary
* @return result
*/
char **SortDictionary(char **Strings,int SizeStrings);

/**
* Remove duplicate words in a dictionary
* @param strings where to search
* @param[out] size of new dictionnary
* @return result
*/
char ** RemoveDuplicateDictionary(char **Strings,int *SizeStrings);

/**
* free pointer
* @param ptrD
* @param sizeptrD
*/
#define freePointerDictionary(ptrD,sizeptrD) if (ptrD)\
{\
	int ifree =0;\
	for (ifree = 0;ifree<(int)sizeptrD;ifree++)\
	{\
		if (ptrD[ifree])\
		{\
			FREE(ptrD[ifree]);\
			ptrD[ifree] = NULL;\
		}\
	}\
	FREE(ptrD);\
	ptrD = NULL;\
}

#endif /* __TOOLSDICTIONARY_H__ */
/*--------------------------------------------------------------------------*/ 
