/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "machine.h" /* HAVE_LIBINTL_H */
#include "charEncoding.h" /* mandatory for _W macro using to_wide_string */

#ifdef HAVE_LIBINTL_H
/* Have been able to find the libintl.h file on the system */
#include <libintl.h>


#define _(String)  gettext(String)/** for console message*/
//#define _W(String)  to_wide_string(gettext(String))/** for console message*/
#define _d(Domain, String)  dgettext(Domain, String)
#define _t(String1,String2,n) ngettext (String1,String2,n)

#else
/* Restore the normal behaviour ... all the string will be in english */

#define _(String) ((const char *) (String))
#define gettext(String) ((const char *) (String))
#define dgettext(Domain, String) ((const char *) (String))
#define _d(Domain, String)  ((const char *) (String))
#define _t(String1,String2,n) ngettext (String1,String2,n)
#define ngettext(String1, String2, n) ((n) == 1 ? (const char *) (String1) : (const char *) (String2))

#endif

#ifdef __cplusplus
}
#endif
#endif /* __LOCALIZATION_H__ */
/*--------------------------------------------------------------------------*/
