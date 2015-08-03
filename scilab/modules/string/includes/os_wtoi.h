/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OS_WTOI_H__
#define __OS_WTOI_H__

#include <string.h>
#include "dynlib_string.h"

STRING_IMPEXP int os_wtoi(const wchar_t* pwcsStr);

#endif /* !__OS_WTOI_H__ */
