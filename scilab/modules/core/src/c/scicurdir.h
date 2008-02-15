/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __SCICURDIR_H__
#define __SCICURDIR_H__
#include "machine.h"

/**
 * Changes scilab current directory 
 * @param path : the path where Scilab should go
 * @param[out] err : the error code
 */
int C2F(scichdir)(char *path,int *err);

/**
 * Get scilab current directory 
 * @param[out] path : the current path
 * @param[out] lpath : the length of the char * path
 * @param[out] err : the error code
 */
int C2F(scigetcwd)(char **path,int *lpath,int *err);

#endif /* __SCICURDIR_H__ */
/*--------------------------------------------------------------------------*/
