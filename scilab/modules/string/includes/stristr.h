/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __STRISTR_H__
#define __STRISTR_H__

#include "dynlib_string.h"

/**
* strstr case insensitive
* @param[in] string where we search
* @param[in] string to search
* @return result
*/
STRING_IMPEXP char *stristr(const char *psz, const char *tofind);

#endif /* __STRISTR_H__ */
/*--------------------------------------------------------------------------*/
