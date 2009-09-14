/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETGETSCIPATH_H__
#define __SETGETSCIPATH_H__

#include <wchar.h>

/** 
 * save SCI path
 * @param path a char * in order to set the Scilab Path
*/
void setSCIpath(char *path);

/** 
* get SCI path
* @return the path to Scilab
*/
char *getSCIpath(void);

/** 
* get SCI path (wide string)
* @return the path to Scilab
*/
wchar_t *getSCIpathW(void);

#endif /* __SETGETSCIPATH_H__ */
/*--------------------------------------------------------------------------*/ 
