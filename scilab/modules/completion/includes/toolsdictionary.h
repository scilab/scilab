/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __TOOLSDICTIONARY_H__
#define __TOOLSDICTIONARY_H__

#include "BOOL.h"

/**
* Append Data to a dictionary(string matrix)
* @param dictionary
* @param i
* @param data
* @param sizedata
* @return TRUE or FALSE
*/
BOOL appendDictionary(char ***dictionary, int *i, char ***data, int *sizedata);

/**
* sort dictionary
* @param strings wto sort
* @param  size of new dictionary
* @return result
*/
char **SortDictionary(char **Strings, int SizeStrings);

/**
* Remove duplicate words in a dictionary
* @param strings where to search
* @param[out] size of new dictionary
* @return result
*/
char **RemoveDuplicateDictionary(char **Strings, int *SizeStrings);

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
