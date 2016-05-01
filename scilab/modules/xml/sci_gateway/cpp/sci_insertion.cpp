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

#include "insertion.hxx"
#include "XMLList_insertion.hxx"

int sci_percent_c_i_XMLDoc(char *fname, void* pvApiCtx)
{
    return sci_insertion < org_modules_xml::XMLDocument, std::string > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_i_XMLDoc(char *fname, void* pvApiCtx)
{
    return sci_insertion < org_modules_xml::XMLDocument, XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_c_i_XMLElem(char *fname, void* pvApiCtx)
{
    return sci_insertion < XMLElement, std::string > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_i_XMLElem(char *fname, void* pvApiCtx)
{
    return sci_insertion < XMLElement, XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLAttr_i_XMLElem(char *fname, void* pvApiCtx)
{
    return sci_insertion < XMLElement, XMLAttr > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLList_i_XMLElem(char *fname, void* pvApiCtx)
{
    return sci_insertion < XMLElement, XMLNodeList > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLNs_i_XMLElem(char *fname, void* pvApiCtx)
{
    return sci_insertion < XMLElement, XMLNs > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_i_XMLList(char *fname, void* pvApiCtx)
{
    return sci_XMLList_insertion < XMLElement > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLDoc_i_XMLList(char *fname, void* pvApiCtx)
{
    return sci_XMLList_insertion < org_modules_xml::XMLDocument > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_c_i_XMLList(char *fname, void* pvApiCtx)
{
    return sci_XMLList_insertion < std::string > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLList_i_XMLList(char *fname, void* pvApiCtx)
{
    return sci_XMLList_insertion < XMLNodeList > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
