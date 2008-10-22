/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __FINDFILES_H__
#define __FINDFILES_H__

/** 
* Finding all files in a directory with a given filespec
* @param path example : "c:\"
* @param filespec example : "*.sce"
* @param[out] sizeListReturned the size of the files list
* @return all the files except '.' & '..'
*/

char **findfiles(char *path,char *filespec,int *sizeListReturned);

#endif /* __FINDFILES_H__ */
/*--------------------------------------------------------------------------*/ 
