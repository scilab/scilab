/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __TYPENAME_H__
#define __TYPENAME_H__

#include "machine.h" /* C2F */

/* see FORTRAN COMMON in typ2code.f */

#define MAX_SCILAB_DEFINED_TYPE  50
#define MAX_LENGTH_NAME_DEFINED_TYPE  200

typedef struct {
	int tp[MAX_SCILAB_DEFINED_TYPE];
	int ptr[MAX_SCILAB_DEFINED_TYPE];
	int ln[MAX_SCILAB_DEFINED_TYPE];
	int namrec[MAX_LENGTH_NAME_DEFINED_TYPE];
	int  ptmax;
} TYPE_struct;


/**
* get all names of types
* @param[out] size of returned array
* @return names of defined types
*/
char **getAllTypesName(int *sizeArray);

/**
* get all values of types
* @param[out] size of returned array
* @return values of defined types
*/
int *getAllTypesNumber(int *sizeArray);

/**
* Get numbers of defined type
* @return numbers of defined type (18 at startup)
*/
int getNumberOfTypes(void);

/**
* add a named type defined by a value
* @param[in] named type
* @param[in] value of type
*/
int addNamedType(char *name,int val);

#endif /* __TYPENAME_H__ */
/*--------------------------------------------------------------------------*/
