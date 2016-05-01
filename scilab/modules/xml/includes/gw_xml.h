/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
/*--------------------------------------------------------------------------*/
#ifndef __GW_XML_H__
#define __GW_XML_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_xml_scilab.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlRead);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlDelete);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlDump);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlGetOpenDocs);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlXPath);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLDoc_e);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLElem_e);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLNs_e);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLAttr_e);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLList_e);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLDoc_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLElem_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLNs_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLList_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLAttr_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLSet_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLList_size);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLSet_size);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLAttr_size);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_c_i_XMLDoc);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLElem_i_XMLDoc);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_c_i_XMLElem);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLElem_i_XMLElem);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLAttr_i_XMLElem);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLList_i_XMLElem);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLNs_i_XMLElem);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_c_i_XMLAttr);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLElem_i_XMLList);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLDoc_i_XMLList);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_c_i_XMLList);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_s_i_XMLList);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_foo_i_XMLList);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLList_i_XMLList);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlReadStr);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlFormat);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlElement);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlDocument);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlNs);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlAddNs);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlGetNsByHref);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlGetNsByPrefix);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlWrite);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLList_length);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLAttr_length);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlDTD);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlSchema);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlRelaxNG);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlValidate);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_percent_XMLValid_p);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlIsValidObject);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlAsNumber);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlAsText);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlRemove);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlSetAttributes);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlName);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_xmlAppend);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_htmlRead);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_htmlReadStr);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_htmlWrite);
XML_SCILAB_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_htmlDump);

/*--------------------------------------------------------------------------*/
#endif /* __GW_XML_H__ */
/*--------------------------------------------------------------------------*/
