/*----------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#ifndef __STRCATFUNCTION_H__
#define __STRCATFUNCTION_H__
#include "machine.h"
#define STAR '*'
#define COL 'c'
#define ROW 'r'
/*------------------------------------------------------------------------*/
/**
* strcatfunction
* description To connect the two strings
* @param[in] Input_String_One
* @param[in] Input_String_Two
* @param[in] Num_of_Input
* @param[out] Output_StringMatrix
* @return bool 
*/
/*------------------------------------------------------------------------*/
BOOL strcatfunction(char **Input_String_One,char **Input_String_Two,int Num_of_Input,char **Output_StringMatrix);

#endif /* __STRCATFUNCTION_H__ */
/*----------------------------------------------------------------------------*/
