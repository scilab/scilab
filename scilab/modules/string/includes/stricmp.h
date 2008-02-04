/*-----------------------------------------------------------------------*/
/* INRIA 2008
/*-----------------------------------------------------------------------*/
#ifndef __STRICMP_H__
#define __STRICMP_H__

#include <string.h>

/* compare  two strings ignoring case */

#if _MSC_VER
	#define stricmp _stricmp 
 #else
    #define stricmp strcasecmp
#endif

#endif /* __STRICMP_H__ */
/*-----------------------------------------------------------------------*/
