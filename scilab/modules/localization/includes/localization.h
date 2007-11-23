/**
 * INRIA 2007 
 * @author Sylvestre Ledru <sylvestre.ledru@inria.fr>
 * Declaration of the basic localization methods
 */
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*--------------------------------------------------------------------------*/ 
#include "machine.h" /* HAVE_LIBINTL_H */

#ifdef HAVE_LIBINTL_H
/* Have been able to find the libintl.h file on the system */
#include <libintl.h>

#define _(String) gettext (String)
#define _t(String1,String2,n) ngettext (String1,String2,n)

#else
/* Restore the normal behaviour ... all the string will be in english */

#define _(String) ((const char *) (String))
#define gettext(String) ((const char *) (String))
#define _t(String1,String2,n) ngettext (String1,String2,n)
#define ngettext(String1, String2, n) ((n) == 1 ? (const char *) (String1) : (const char *) (String2))

#endif

#endif /* __LOCALIZATION_H__ */
/*--------------------------------------------------------------------------*/ 
