/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __LANGUAGEPREF_H__
#define __LANGUAGEPREF_H__

/**
* returns full filename (with path) of preference about LC_MESSAGES
* @return a string
* "" if not find or not valid
*/

char *getFilenameLanguagePref(void);

#endif /* __LANGUAGEPREF_H__ */
/*--------------------------------------------------------------------------*/ 