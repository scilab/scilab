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
#include <string.h>
#include "GetXmlFileEncoding.h"
#include "libxml/xmlreader.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/ 
char *GetXmlFileEncoding(const char *filename)
{
#define DEFAULT_ENCODING "UTF-8"
	char *encoding=NULL;
	xmlDocPtr doc = NULL;

	/* default */
	encoding = strdup(DEFAULT_ENCODING);

	doc = xmlParseFile (filename);
	if (doc) 
	{
		if (doc->encoding)
		{
			if (encoding) {FREE(encoding);encoding=NULL;}
			encoding = strdup((char*)doc->encoding);
		}
	}

	xmlFreeDoc (doc);
	return encoding;
}
/*--------------------------------------------------------------------------*/ 
