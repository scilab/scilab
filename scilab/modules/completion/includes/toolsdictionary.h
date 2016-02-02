/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
