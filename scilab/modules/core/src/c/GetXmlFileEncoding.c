/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <string.h>
#include "GetXmlFileEncoding.h"
#include "libxml/xmlreader.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "BOOL.h"
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/
char *GetXmlFileEncoding(const char *filename)
{
#define DEFAULT_ENCODING "UTF-8"
    char *encoding = NULL;
    xmlDocPtr doc = NULL;
    BOOL bConvert = FALSE;
    char *shortfilename = getshortpathname(filename, &bConvert);

    /* default */
    encoding = os_strdup(DEFAULT_ENCODING);

    if (shortfilename)
    {
        doc = xmlParseFile (filename);
        FREE(shortfilename);
        shortfilename = NULL;
        if (doc)
        {
            if (doc->encoding)
            {
                if (encoding)
                {
                    FREE(encoding);
                    encoding = NULL;
                }
                encoding = os_strdup(doc->encoding);
            }
        }
        xmlFreeDoc (doc);
    }
    return encoding;
}
/*--------------------------------------------------------------------------*/
