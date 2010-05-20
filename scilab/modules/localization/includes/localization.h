/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*--------------------------------------------------------------------------*/ 
#ifdef __cplusplus
extern "C" {
#endif

#include "machine.h" /* HAVE_LIBINTL_H */

#ifdef HAVE_LIBINTL_H
/* Have been able to find the libintl.h file on the system */
#include <libintl.h>



#define _(String)  gettext(String)/** for console message*/
#define _t(String1,String2,n) ngettext (String1,String2,n)

#else
/* Restore the normal behaviour ... all the string will be in english */

#define _(String) ((const char *) (String))
#define gettext(String) ((const char *) (String))
#define _t(String1,String2,n) ngettext (String1,String2,n)
#define ngettext(String1, String2, n) ((n) == 1 ? (const char *) (String1) : (const char *) (String2))

#endif

#ifdef __cplusplus
}
#endif
#endif /* __LOCALIZATION_H__ */
/*--------------------------------------------------------------------------*/ 
