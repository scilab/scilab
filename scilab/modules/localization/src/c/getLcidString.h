/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
