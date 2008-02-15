/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __CONVERTPATHUNIXTOWINDOWS_H__
#define __CONVERTPATHUNIXTOWINDOWS_H__

#include "machine.h"

/**
* convert a unix path to windows path
* replaces '/' by '\'
* @param [in]
* @param [out]
* @return
*/
BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);

#endif /* __CONVERTPATHUNIXTOWINDOWS_H__ */
/*--------------------------------------------------------------------------*/
