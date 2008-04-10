/*--------------------------------------------------------------------------*/

/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __HTTPDOWNLOADFILE_H__
#define __HTTPDOWNLOADFILE_H__

/**
* download a file by http (uses wininet library)
* http://msdn2.microsoft.com/en-us/library/aa385473(VS.85).aspx
* example httpDownloadFile("http://www.scilab.org/download/4.1.2/scilab-4.1.2.exe","d:/scilab-4.1.2.exe");
* @param[in] szURL string url file to download
* @param[in] szSaveFilePath string filename destination
*/
BOOL httpDownloadFile(char * szURL,char * szSaveFilePath);

#endif /* __HTTPDOWNLOADFILE_H__ */
/*--------------------------------------------------------------------------*/

