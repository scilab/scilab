/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
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
/*--------------------------------------------------------------------------*/
#ifndef __SCILABKEYWORDS_H__
#define __SCILABKEYWORDS_H__

/**
* Get all variables name in scilab
* @return char array
*/
char **GetVariablesName(void);

/**
* Get all commands name in scilab
* @return char array
*/
char **GetCommandsName(void);

/**
* get primitives name in scilab
* @return char array
*/
char **GetFunctionsName(void);

/**
* get macros name  in scilab
* @return char array
*/
char **GetMacrosName(void);

#endif /* __SCILABKEYWORDS_H__ */
/*--------------------------------------------------------------------------*/
