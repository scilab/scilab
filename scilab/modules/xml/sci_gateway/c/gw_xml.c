/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_xml.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_xmlRead, "xmlRead"},
    {sci_xmlClose, "xmlClose"},
    {sci_xmlDump, "xmlDump"},
    {sci_xmlGetOpenStreams, "xmlGetOpenStreams"},
    {sci_xmlXPath, "xmlXPath"},
    {sci_percent_XMLDoc_e, "%XMLDoc_e"},
    {sci_percent_XMLElem_e, "%XMLElem_e"},
    {sci_percent_XMLNs_e, "%XMLNs_e"},
    {sci_percent_XMLAttr_e, "%XMLAttr_e"},
    {sci_percent_XMLList_e, "%XMLList_e"},
    {sci_percent_XMLDoc_p, "%XMLDoc_p"},
    {sci_percent_XMLElem_p, "%XMLElem_p"},
    {sci_percent_XMLNs_p, "%XMLNs_p"},
    {sci_percent_XMLList_p, "%XMLList_p"},
    {sci_percent_XMLAttr_p, "%XMLAttr_p"},
    {sci_percent_XMLList_size, "%XMLList_size"}
};
/*--------------------------------------------------------------------------*/
int gw_xml(void)
{
    Rhs = Max(0, Rhs);

    if(pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/

