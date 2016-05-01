/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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
#ifndef __LOADONUSECLASSPATH_H__
#define __LOADONUSECLASSPATH_H__

#include "BOOL.h" /* BOOL */

/**
* Load a serie of jar(s) matching to the tag declared in etc/classpath.xml
* @param a new path to add
* @return TRUE or FALSE
*/
BOOL loadOnUseClassPath(char const* tag);

#endif /* __LOADONUSECLASSPATH_H__ */
/*--------------------------------------------------------------------------*/
