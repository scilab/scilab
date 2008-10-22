/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __LOADONUSECLASSPATH_H__
#define __LOADONUSECLASSPATH_H__

#include "machine.h" /* BOOL */

/**
* Load a serie of jar(s) matching to the tag declared in etc/classpath.xml
* @param a new path to add
* @return TRUE or FALSE
*/
BOOL loadOnUseClassPath(char *tag);

#endif /* __LOADONUSECLASSPATH_H__ */
/*--------------------------------------------------------------------------*/ 
