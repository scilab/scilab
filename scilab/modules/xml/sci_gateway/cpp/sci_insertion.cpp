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

#include "insertion.hpp"
#include "XMLList_insertion.hpp"

int sci_percent_c_i_XMLDoc(char *fname, unsigned long fname_len)
{
    return sci_insertion < org_modules_xml::XMLDocument, std::string > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_i_XMLDoc(char *fname, unsigned long fname_len)
{
    return sci_insertion < org_modules_xml::XMLDocument, XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_c_i_XMLElem(char *fname, unsigned long fname_len)
{
    return sci_insertion < XMLElement, std::string > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_i_XMLElem(char *fname, unsigned long fname_len)
{
    return sci_insertion < XMLElement, XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLAttr_i_XMLElem(char *fname, unsigned long fname_len)
{
    return sci_insertion < XMLElement, XMLAttr > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLList_i_XMLElem(char *fname, unsigned long fname_len)
{
    return sci_insertion < XMLElement, XMLNodeList > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLNs_i_XMLElem(char *fname, unsigned long fname_len)
{
    return sci_insertion < XMLElement, XMLNs > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_i_XMLList(char *fname, unsigned long fname_len)
{
    return sci_XMLList_insertion < XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLDoc_i_XMLList(char *fname, unsigned long fname_len)
{
    return sci_XMLList_insertion < org_modules_xml::XMLDocument > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_c_i_XMLList(char *fname, unsigned long fname_len)
{
    return sci_XMLList_insertion < std::string > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLList_i_XMLList(char *fname, unsigned long fname_len)
{
    return sci_XMLList_insertion < XMLNodeList > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
