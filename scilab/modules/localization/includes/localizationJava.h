/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/**
 * Declaration of the basic localization methods
 */
#ifndef __LOCALIZATIONJAVA_H__
#define __LOCALIZATIONJAVA_H__
/*--------------------------------------------------------------------------*/ 
#include "machine.h" /* HAVE_LIBINTL_H & NAMELOCALIZATIONDOMAIN*/

#ifdef HAVE_LIBINTL_H
/* Have been able to find the libintl.h file on the system */
#include <libintl.h>
#include "stack-def.h"
/* This piece of code has been written because of the bug #4005
 * in version 5.0, we were using two localization system (native and 
 * Java).
 * Now, only the native one is used. 
 * However, for an unknown reason, gettext() is not working. Only 
 * dgettext is */
#define scigettext(String1) dgettext(NAMELOCALIZATIONDOMAIN,String1)

#else
/* Restore the normal behaviour ... all the string will be in english */

#define scigettext(String) ((const char *) (String))

#endif

#endif /* __LOCALIZATIONJAVA_H__ */
/*--------------------------------------------------------------------------*/ 
