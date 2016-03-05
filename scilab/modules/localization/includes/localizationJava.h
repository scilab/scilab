/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
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
/* This piece of code has been written because of the bug #4005
 * in version 5.0, we were using two localization system (native and
 * Java).
 * Now, only the native one is used.
 * However, for an unknown reason, gettext() is not working. Only
 * dgettext is */
#define scigettext(String1) dgettext(NAMELOCALIZATIONDOMAIN,String1)
#define scidgettext(String1, String2) dgettext(String1, String2)

#else
/* Restore the normal behaviour ... all the string will be in english */

#define scigettext(String) ((const char *) (String))
#define scidgettext(String1, String2) ((const char *) (String2))

#endif

#endif /* __LOCALIZATIONJAVA_H__ */
/*--------------------------------------------------------------------------*/
