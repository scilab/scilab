
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
