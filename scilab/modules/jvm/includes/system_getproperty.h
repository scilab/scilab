
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
#ifndef __SYSTEM_GETPROPERTY_H__
#define __SYSTEM_GETPROPERTY_H__

/**
* get system property
* @param property name
* @return property value
*/
char * system_getproperty(const char * property, const char * defaultproperty);

#endif /* __SYSTEM_GETPROPERTY_H__ */
/*--------------------------------------------------------------------------*/
