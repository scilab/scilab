
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
#ifndef __LOADCLASSPATH_H__
#define __LOADCLASSPATH_H__

#include <libxml/xmlreader.h>

#include "BOOL.h" /* BOOL */

/**
* add paths to CLASSPATH
* @param a filename SCI/etc/classpath.xml
* @return TRUE or FALSE
*/
BOOL LoadClasspath(char *xmlfilename);


/**
* returns xmlDocPtr of classpath.xml
* @return xmlDocPtr
*/
xmlDocPtr getClassPathxmlDocPtr(void);

/**
* free xmlDocPtr of classpath.xml
*/
void freeClassPathxmlDocPtr(void);

#endif /* __LOADCLASSPATH_H__ */
/*--------------------------------------------------------------------------*/
