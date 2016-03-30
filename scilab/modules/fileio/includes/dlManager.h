/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - SE - Sylvestre LEDRU
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
#ifndef __DLMANAGER_H__
#define __DLMANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include "BOOL.h"
#include "dynlib_fileio.h"

#define DEFAULT_FILENAME "index.html"

/**
 * Download a file
 * @param[in] fileUrl the location of the file
 * @param[in] dest where to save the file
 * @return the file pointer
*/
FILEIO_IMPEXP char *downloadFile(char *url, char *dest, char *username, char *password, char **content);

#endif /* __DLMANAGER_H__ */
