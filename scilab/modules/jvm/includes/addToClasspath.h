
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

/*--------------------------------------------------------------------------*/ 
#ifndef __ADDTOCLASSPATH_H__
#define __ADDTOCLASSPATH_H__

#include "BOOL.h" /* BOOL */

typedef enum { STARTUP = 0,
			   BACKGROUND = 1,
               ONUSE = 2
	
} typeOfLoad;


/**
* add a path to CLASSPATH
* @param a new path to add
* @return TRUE or FALSE
*/
BOOL addToClasspath(char *classpathstring, typeOfLoad load);

#endif /* __ADDTOCLASSPATH_H__ */
/*--------------------------------------------------------------------------*/ 
