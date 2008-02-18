
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
#ifndef __SYSTEM_SETPROPERTY_H__
#define __SYSTEM_SETPROPERTY_H__

#include "machine.h" /* BOOL */

/**
* set a system property
* @param property name
* @param value
* @return previous value or NULL
*/
char * system_setproperty(char *property,char *value);

#endif /* __SYSTEM_SETPROPERTY_H__ */
/*--------------------------------------------------------------------------*/ 
