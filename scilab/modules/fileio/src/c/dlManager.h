/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - SE - Sylvestre LEDRU
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#ifndef __DLMANAGER_H__
#define __DLMANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include "BOOL.h"

#define DEFAULT_FILENAME "index.html"

/**
 * Download a file
 * @param[in] fileUrl the location of the file
 * @param[in] dest where to save the file
 * @return the file pointer
*/
char *downloadFile(char *url, char *dest, char *username, char *password, char **content);

#endif /* __DLMANAGER_H__ */
