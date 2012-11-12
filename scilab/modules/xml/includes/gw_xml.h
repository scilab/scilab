/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_XML_H__
#define __GW_XML_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_xml_scilab.h"
/*--------------------------------------------------------------------------*/
XML_SCILAB_IMPEXP int gw_xml(void);

/*--------------------------------------------------------------------------*/
int sci_xmlRead(char *fname, void *pvApiCtx);
int sci_xmlDelete(char *fname, void *pvApiCtx);
int sci_xmlDump(char *fname, void *pvApiCtx);
int sci_xmlGetOpenDocs(char *fname, void *pvApiCtx);
int sci_xmlXPath(char *fname, void *pvApiCtx);
int sci_percent_XMLDoc_e(char *fname, void *pvApiCtx);
int sci_percent_XMLElem_e(char *fname, void *pvApiCtx);
int sci_percent_XMLNs_e(char *fname, void *pvApiCtx);
int sci_percent_XMLAttr_e(char *fname, void *pvApiCtx);
int sci_percent_XMLList_e(char *fname, void *pvApiCtx);
int sci_percent_XMLDoc_p(char *fname, void *pvApiCtx);
int sci_percent_XMLElem_p(char *fname, void *pvApiCtx);
int sci_percent_XMLNs_p(char *fname, void *pvApiCtx);
int sci_percent_XMLList_p(char *fname, void *pvApiCtx);
int sci_percent_XMLAttr_p(char *fname, void *pvApiCtx);
int sci_percent_XMLSet_p(char *fname, void *pvApiCtx);
int sci_percent_XMLList_size(char *fname, void *pvApiCtx);
int sci_percent_XMLSet_size(char *fname, void *pvApiCtx);
int sci_percent_XMLAttr_size(char *fname, void *pvApiCtx);
int sci_percent_c_i_XMLDoc(char *fname, void *pvApiCtx);
int sci_percent_XMLElem_i_XMLDoc(char *fname, void *pvApiCtx);
int sci_percent_c_i_XMLElem(char *fname, void *pvApiCtx);
int sci_percent_XMLElem_i_XMLElem(char *fname, void *pvApiCtx);
int sci_percent_XMLAttr_i_XMLElem(char *fname, void *pvApiCtx);
int sci_percent_XMLList_i_XMLElem(char *fname, void *pvApiCtx);
int sci_percent_XMLNs_i_XMLElem(char *fname, void *pvApiCtx);
int sci_percent_c_i_XMLAttr(char *fname, void *pvApiCtx);
int sci_percent_XMLElem_i_XMLList(char *fname, void *pvApiCtx);
int sci_percent_XMLDoc_i_XMLList(char *fname, void *pvApiCtx);
int sci_percent_c_i_XMLList(char *fname, void *pvApiCtx);
int sci_percent_s_i_XMLList(char *fname, void *pvApiCtx);
int sci_percent_foo_i_XMLList(char *fname, void *pvApiCtx);
int sci_percent_XMLList_i_XMLList(char *fname, void *pvApiCtx);
int sci_xmlReadStr(char *fname, void *pvApiCtx);
int sci_xmlFormat(char *fname, void *pvApiCtx);
int sci_xmlElement(char *fname, void *pvApiCtx);
int sci_xmlDocument(char *fname, void *pvApiCtx);
int sci_xmlNs(char *fname, void *pvApiCtx);
int sci_xmlAddNs(char *fname, void *pvApiCtx);
int sci_xmlGetNsByHref(char *fname, void *pvApiCtx);
int sci_xmlGetNsByPrefix(char *fname, void *pvApiCtx);
int sci_xmlWrite(char *fname, void *pvApiCtx);
int sci_percent_XMLList_length(char *fname, void *pvApiCtx);
int sci_percent_XMLAttr_length(char *fname, void *pvApiCtx);
int sci_xmlDTD(char *fname, void *pvApiCtx);
int sci_xmlSchema(char *fname, void *pvApiCtx);
int sci_xmlRelaxNG(char *fname, void *pvApiCtx);
int sci_xmlValidate(char *fname, void *pvApiCtx);
int sci_percent_XMLValid_p(char *fname, void *pvApiCtx);
int sci_xmlIsValidObject(char *fname, void *pvApiCtx);
int sci_xmlAsNumber(char *fname, void *pvApiCtx);
int sci_xmlAsText(char *fname, void *pvApiCtx);
int sci_xmlRemove(char *fname, void *pvApiCtx);
int sci_xmlSetAttributes(char *fname, void *pvApiCtx);
int sci_xmlName(char *fname, void *pvApiCtx);

/*--------------------------------------------------------------------------*/
#endif /* __GW_XML_H__ */
/*--------------------------------------------------------------------------*/
