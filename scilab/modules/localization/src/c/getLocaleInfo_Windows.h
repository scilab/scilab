/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#ifndef __GETLOCALINFO_WINDOWS_H__
#define __GETLOCALINFO_WINDOWS_H__

/**
* returns user locale string
* @return Locale user example fr_FR or en_US
*/
char* getLocaleUserInfo(void);

/**
* returns system locale string
* @return Locale system example fr_FR or en_US
*/
char* getLocaleSystemInfo(void);

#endif /* __GETLOCALINFO_WINDOWS_H__ */

/*--------------------------------------------------------------------------*/
