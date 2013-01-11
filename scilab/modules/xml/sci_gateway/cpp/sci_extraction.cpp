/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "extraction.hxx"

using namespace org_modules_xml;

int sci_percent_XMLDoc_e(char *fname, unsigned long fname_len)
{
    return sci_extraction < org_modules_xml::XMLDocument > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_e(char *fname, unsigned long fname_len)
{
    return sci_extraction < XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLNs_e(char *fname, unsigned long fname_len)
{
    return sci_extraction < XMLNs > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
