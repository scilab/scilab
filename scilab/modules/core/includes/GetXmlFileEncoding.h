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
#ifndef __GETXMLFILEENCODING_H__
#define __GETXMLFILEENCODING_H__
#include <string.h>

/**
* Get xml file encoding
* @param filename
* @return a string as UTF-8
*/
char *GetXmlFileEncoding(const char *filename);

#endif /* __GETXMLFILEENCODING_H__ */
/*--------------------------------------------------------------------------*/
