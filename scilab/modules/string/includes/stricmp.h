
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*-----------------------------------------------------------------------*/
#ifndef __STRICMP_H__
#define __STRICMP_H__

#include <string.h>

/* compare two strings ignoring the case */

#if _MSC_VER
#define stricmp _stricmp
#define strnicmp _strnicmp
#else
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

#endif /* __STRICMP_H__ */
/*-----------------------------------------------------------------------*/
