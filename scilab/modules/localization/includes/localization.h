/**
 * INRIA 2007 
 * @author Sylvestre Ledru <sylvestre.ledru@inria.fr> - Rewrote using gettext
 * Declaration of the basic localization methods
 */
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*-----------------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
#include <libintl.h>
#include <locale.h>

#define _(String) gettext (String)
#define _t(String1,String2,n) ngettext (String1,String2,n)
#endif
#endif /* __LOCALIZATION_H__ */
/*-----------------------------------------------------------------------------------*/ 
