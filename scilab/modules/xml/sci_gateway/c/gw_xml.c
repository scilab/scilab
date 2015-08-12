/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_xml.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_xmlRead, "xmlRead"},
    {sci_xmlDelete, "xmlDelete"},
    {sci_xmlDump, "xmlDump"},
    {sci_xmlGetOpenDocs, "xmlGetOpenDocs"},
    {sci_xmlXPath, "xmlXPath"},
    {sci_percent_XMLDoc_e, "%XMLDoc_e"},
    {sci_percent_XMLElem_e, "%XMLElem_e"},
    {sci_percent_XMLNs_e, "%XMLNs_e"},
    {sci_percent_XMLAttr_e, "%XMLAttr_e"},
    {sci_percent_XMLList_e, "%XMLList_e"},
    {sci_percent_XMLList_e, "%XMLSet_e"},
    {sci_percent_XMLDoc_p, "%XMLDoc_p"},
    {sci_percent_XMLElem_p, "%XMLElem_p"},
    {sci_percent_XMLNs_p, "%XMLNs_p"},
    {sci_percent_XMLList_p, "%XMLList_p"},
    {sci_percent_XMLAttr_p, "%XMLAttr_p"},
    {sci_percent_XMLSet_p, "%XMLSet_p"},
    {sci_percent_XMLList_size, "%XMLList_size"},
    {sci_percent_XMLList_size, "%XMLSet_size"},
    {sci_percent_XMLAttr_size, "%XMLAttr_size"},
    {sci_percent_c_i_XMLDoc, "%c_i_XMLDoc"},
    {sci_percent_XMLElem_i_XMLDoc, "%XMLElem_i_XMLDoc"},
    {sci_percent_c_i_XMLElem, "%c_i_XMLElem"},
    {sci_percent_XMLElem_i_XMLElem, "%XMLElem_i_XMLElem"},
    {sci_percent_XMLAttr_i_XMLElem, "%XMLAttr_i_XMLElem"},
    {sci_percent_XMLList_i_XMLElem, "%XMLList_i_XMLElem"},
    {sci_percent_XMLNs_i_XMLElem, "%XMLNs_i_XMLElem"},
    {sci_percent_c_i_XMLAttr, "%c_i_XMLAttr"},
    {sci_percent_XMLElem_i_XMLList, "%XMLElem_i_XMLList"},
    {sci_percent_XMLDoc_i_XMLList, "%XMLDoc_i_XMLList"},
    {sci_percent_c_i_XMLList, "%c_i_XMLList"},
    {sci_percent_s_i_XMLList, "%s_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%p_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%b_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%sp_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%spb_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%msp_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%i_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%h_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%fptr_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%mc_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%l_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%hm_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%ptr_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%ce_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%st_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%r_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%lss_i_XMLList"},
    {sci_percent_foo_i_XMLList, "%ip_i_XMLList"},
    {sci_percent_XMLList_i_XMLList, "%XMLList_i_XMLList"},
    {sci_xmlReadStr, "xmlReadStr"},
    {sci_xmlFormat, "xmlFormat"},
    {sci_xmlElement, "xmlElement"},
    {sci_xmlDocument, "xmlDocument"},
    {sci_xmlNs, "xmlNs"},
    {sci_xmlAddNs, "xmlAddNs"},
    {sci_xmlGetNsByHref, "xmlGetNsByHref"},
    {sci_xmlGetNsByPrefix, "xmlGetNsByPrefix"},
    {sci_percent_XMLElem_e, "%XMLElem_6"},
    {sci_percent_XMLDoc_e, "%XMLDoc_6"},
    {sci_percent_XMLNs_e, "%XMLNs_6"},
    {sci_percent_XMLAttr_e, "%XMLAttr_6"},
    {sci_percent_XMLList_e, "%XMLList_6"},
    {sci_percent_XMLList_e, "%XMLSet_6"},
    {sci_xmlWrite, "xmlWrite"},
    {sci_percent_XMLList_length, "%XMLList_length"},
    {sci_percent_XMLList_length, "%XMLSet_length"},
    {sci_percent_XMLAttr_length, "%XMLAttr_length"},
    {sci_xmlDTD, "xmlDTD"},
    {sci_xmlSchema, "xmlSchema"},
    {sci_xmlRelaxNG, "xmlRelaxNG"},
    {sci_xmlValidate, "xmlValidate"},
    {sci_percent_XMLValid_p, "%XMLValid_p"},
    {sci_xmlIsValidObject, "xmlIsValidObject"},
    {sci_xmlAsNumber, "xmlAsNumber"},
    {sci_xmlAsText, "xmlAsText"},
    {sci_xmlRemove, "xmlRemove"},
    {sci_xmlSetAttributes, "xmlSetAttributes"},
    {sci_xmlName, "xmlName"},
    {sci_xmlAppend, "xmlAppend"},
    {sci_htmlRead, "htmlRead"},
    {sci_htmlReadStr, "htmlReadStr"},
    {sci_htmlWrite, "htmlWrite"},
    {sci_htmlDump, "htmlDump"}
};

/*--------------------------------------------------------------------------*/
int gw_xml(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx *) MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char *)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}

/*--------------------------------------------------------------------------*/
