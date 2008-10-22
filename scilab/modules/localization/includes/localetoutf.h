/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Yung-Jang Lee
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LOCALETOUTF_H__
#define __LOCALETOUTF_H__

/**
 * Convert from locale to UTF-8 
 *
 * @param buffer    
 */
void localeToUTF(char** buffer) ;

/**
 * @TODO add comment
 *
 * @param sysLocale 
 * @param lang  
 */
void openLocaleToUTFConverter(char *sysLocale,char *lang);

#endif 
