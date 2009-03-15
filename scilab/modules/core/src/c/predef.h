
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

#ifndef __PREDEF_H__
#define __PREDEF_H__

/**
* get the number of protected variables
* @return number of protected variables
*/
int getNumberPredefVariablesProtected(void);

/**
* set the max(nb,7) last defined variables as protected
* @param[in] nb variables
*/
void setNumberPredefVariablesProtected(int nb);

/**
* sets all the variables protected
*/
void clearPredef(void);

/**
* unprotect all variables
*/
void predefAll(void);

#endif /* __PREDEF_H__ */
/*--------------------------------------------------------------------------*/
