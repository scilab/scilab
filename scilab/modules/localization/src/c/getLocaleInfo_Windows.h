/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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

#ifndef __GETLOCALINFO_WINDOWS_H__
#define __GETLOCALINFO_WINDOWS_H__

/**
* returns user locale string
* @return Locale user example fr_FR or en_US
*/
wchar_t* getLocaleUserInfo(void);

/**
* returns system locale string
* @return Locale system example fr_FR or en_US
*/
char* getLocaleSystemInfo(void);

#endif /* __GETLOCALINFO_WINDOWS_H__ */

/*--------------------------------------------------------------------------*/
