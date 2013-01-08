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
int sci_xmlRead(char *fname, unsigned long fname_len);
int sci_xmlDelete(char *fname, unsigned long fname_len);
int sci_xmlDump(char *fname, unsigned long fname_len);
int sci_xmlGetOpenDocs(char *fname, unsigned long fname_len);
int sci_xmlXPath(char *fname, unsigned long fname_len);
int sci_percent_XMLDoc_e(char *fname, unsigned long fname_len);
int sci_percent_XMLElem_e(char *fname, unsigned long fname_len);
int sci_percent_XMLNs_e(char *fname, unsigned long fname_len);
int sci_percent_XMLAttr_e(char *fname, unsigned long fname_len);
int sci_percent_XMLList_e(char *fname, unsigned long fname_len);
int sci_percent_XMLDoc_p(char *fname, unsigned long fname_len);
int sci_percent_XMLElem_p(char *fname, unsigned long fname_len);
int sci_percent_XMLNs_p(char *fname, unsigned long fname_len);
int sci_percent_XMLList_p(char *fname, unsigned long fname_len);
int sci_percent_XMLAttr_p(char *fname, unsigned long fname_len);
int sci_percent_XMLSet_p(char *fname, unsigned long fname_len);
int sci_percent_XMLList_size(char *fname, unsigned long fname_len);
int sci_percent_XMLSet_size(char *fname, unsigned long fname_len);
int sci_percent_XMLAttr_size(char *fname, unsigned long fname_len);
int sci_percent_c_i_XMLDoc(char *fname, unsigned long fname_len);
int sci_percent_XMLElem_i_XMLDoc(char *fname, unsigned long fname_len);
int sci_percent_c_i_XMLElem(char *fname, unsigned long fname_len);
int sci_percent_XMLElem_i_XMLElem(char *fname, unsigned long fname_len);
int sci_percent_XMLAttr_i_XMLElem(char *fname, unsigned long fname_len);
int sci_percent_XMLList_i_XMLElem(char *fname, unsigned long fname_len);
int sci_percent_XMLNs_i_XMLElem(char *fname, unsigned long fname_len);
int sci_percent_c_i_XMLAttr(char *fname, unsigned long fname_len);
int sci_percent_XMLElem_i_XMLList(char *fname, unsigned long fname_len);
int sci_percent_XMLDoc_i_XMLList(char *fname, unsigned long fname_len);
int sci_percent_c_i_XMLList(char *fname, unsigned long fname_len);
int sci_percent_s_i_XMLList(char *fname, unsigned long fname_len);
int sci_percent_foo_i_XMLList(char *fname, unsigned long fname_len);
int sci_percent_XMLList_i_XMLList(char *fname, unsigned long fname_len);
int sci_xmlReadStr(char *fname, unsigned long fname_len);
int sci_xmlFormat(char *fname, unsigned long fname_len);
int sci_xmlElement(char *fname, unsigned long fname_len);
int sci_xmlDocument(char *fname, unsigned long fname_len);
int sci_xmlNs(char *fname, unsigned long fname_len);
int sci_xmlAddNs(char *fname, unsigned long fname_len);
int sci_xmlGetNsByHref(char *fname, unsigned long fname_len);
int sci_xmlGetNsByPrefix(char *fname, unsigned long fname_len);
int sci_xmlWrite(char *fname, unsigned long fname_len);
int sci_percent_XMLList_length(char *fname, unsigned long fname_len);
int sci_percent_XMLAttr_length(char *fname, unsigned long fname_len);
int sci_xmlDTD(char *fname, unsigned long fname_len);
int sci_xmlSchema(char *fname, unsigned long fname_len);
int sci_xmlRelaxNG(char *fname, unsigned long fname_len);
int sci_xmlValidate(char *fname, unsigned long fname_len);
int sci_percent_XMLValid_p(char *fname, unsigned long fname_len);
int sci_xmlIsValidObject(char *fname, unsigned long fname_len);
int sci_xmlAsNumber(char *fname, unsigned long fname_len);
int sci_xmlAsText(char *fname, unsigned long fname_len);
int sci_xmlRemove(char *fname, unsigned long fname_len);
int sci_xmlSetAttributes(char *fname, unsigned long fname_len);
int sci_xmlName(char *fname, unsigned long fname_len);
int sci_xmlAppend(char *fname, unsigned long fname_len);

/*--------------------------------------------------------------------------*/
#endif /* __GW_XML_H__ */
/*--------------------------------------------------------------------------*/
