/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
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

#include "extraction.hxx"
#include "gw_xml.h"

using namespace org_modules_xml;

int sci_percent_XMLDoc_e(char *fname, void* pvApiCtx)
{
    return sci_extraction < org_modules_xml::XMLDocument > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_e(char *fname, void* pvApiCtx)
{
    return sci_extraction < XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLNs_e(char *fname, void* pvApiCtx)
{
    return sci_extraction < XMLNs > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
