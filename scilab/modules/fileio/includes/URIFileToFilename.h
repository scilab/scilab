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
#ifndef __URIFILETOFILENAME_H__
#define __URIFILETOFILENAME_H__

#include "BOOL.h"
/**
* convert URI File (file://path/filename.ext) to standard filename
* @param[in] uri : a string URI file
* @return a filename
*/
char *URIFileToFilename(char *uri);

/**
* check if it is a URIFile
* @param [in] uri : a string URI file
* @return TRUE or FALSE;
*/
BOOL isURIFile(char *uri);

#endif /* __URIFILETOFILENAME_H__ */
/*--------------------------------------------------------------------------*/
