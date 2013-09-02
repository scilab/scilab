/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GETLCIDSTRING_H__
#define __GETLCIDSTRING_H__

/**
* convert Locale name to LCID string
* used by CHM
* http://msdn.microsoft.com/en-us/library/0h88fahh(VS.85).aspx
* @param[in] locale name ex: en_US
* @return LCID string ex: 0x0409
*/
char *getLcidString(const char *pStrLocale);

#endif /* __GETLCIDSTRING_H__ */
/*--------------------------------------------------------------------------*/
