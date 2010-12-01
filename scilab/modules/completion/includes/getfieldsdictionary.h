/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETFIELDSDICTIONARY_H__
#define __GETFIELDSDICTIONARY_H__

/**
* get fields dictionary
* @param[in] the line before the caret
* @param[in] the part
* @param[in] the returned size of the array
* @return array of strings
*/
char **getfieldsdictionary(char *, char*, int*);

#endif /* __GETFIELDSDICTIONARY_H__ */
/*--------------------------------------------------------------------------*/ 
