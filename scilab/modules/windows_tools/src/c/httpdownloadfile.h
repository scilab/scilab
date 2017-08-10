/*--------------------------------------------------------------------------*/

/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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

#ifndef __HTTPDOWNLOADFILE_H__
#define __HTTPDOWNLOADFILE_H__

#include "dynlib_windows_tools.h"

typedef enum
{
    HTTP_DOWNLOAD_ERROR_OK = 0,
    HTTP_DOWNLOAD_ERROR_INVALID_URL = -1,
    HTTP_DOWNLOAD_ERROR_INTERNET_OPEN = -2,
    HTTP_DOWNLOAD_ERROR_OPEN_URL = -3,
    HTTP_DOWNLOAD_ERROR_CREATEFILE = -4,
    HTTP_DOWNLOAD_ERROR_INVALID_FILE_SIZE = -5,
    HTTP_DOWNLOAD_ERROR_INTERNET_READFILE = -6,
    HTTP_DOWNLOAD_ERROR_WRITEFILE = -7,
    HTTP_DOWNLOAD_FAILURE = -8,
    HTTP_DOWNLOAD_OUTOFMEMORY = -9
} httpdownloadfile_error_code;

/**
* download a file by http (uses wininet library)
* http://msdn2.microsoft.com/en-us/library/aa385473(VS.85).aspx
* example httpDownloadFile("http://www.scilab.org/download/4.1.2/scilab-4.1.2.exe","d:/scilab-4.1.2.exe");
* @param[in] szURL string url file to download
* @param[in] szSaveFilePath string filename destination
*/
httpdownloadfile_error_code httpDownloadFile(char * szURL, char * szSaveFilePath);

/* simplified version for 'call' from scilab */
WINDOWS_TOOLS_IMPEXP void httpdownload(char * szURL, char * szSaveFilePath, double *ierr);

#endif /* __HTTPDOWNLOADFILE_H__ */
/*--------------------------------------------------------------------------*/

