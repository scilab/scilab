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
#ifndef __SCILABKEYWORDS_H__
#define __SCILABKEYWORDS_H__

/**
* Get all variables name in scilab
* @return char array
*/
char **ScilabKeyword_GetVariablesName(void);

/**
* Get all commands name in scilab
* @return char array
*/
char **ScilabKeyword_GetCommandsName(void);

/**
* get primitives name in scilab
* @return char array
*/
char **ScilabKeyword_GetFunctionsName(void);

/**
* get macros name  in scilab
* @return char array
*/
char **ScilabKeyword_GetMacrosName(void);

#endif /* __SCILABKEYWORDS_H__ */
/*--------------------------------------------------------------------------*/  
