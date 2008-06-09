/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __CONVERTSLASH_H__
#define __CONVERTSLASH_H__

#include "machine.h"


/* C:\Windows\system and C:/Windows/system */
/* since Windows 2000 SP2 are supported */

/**
* convert a unix path to windows path 'format'
* replaces '/' by '\'
* @param [in] path in with '/'
* @param [out] path out with '\'
* @return TRUE or FALSE
*/
BOOL slashToAntislash(char *pathunix,char *pathwindows);

/**
* convert a windows path to unix path 'format'
* replaces '\' by '/'
* @param [in] path in with '\'
* @param [out] path out with '/'
* @return TRUE or FALSE
*/
BOOL AntislashToSlash(char *pathwindows,char *pathunix);

#endif /* __CONVERTSLASH_H__ */
/*--------------------------------------------------------------------------*/
