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
#ifndef __GW_XML_H__
#define __GW_XML_H__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_xml_scilab.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/ 
XML_SCILAB_IMPEXP int gw_xml(void);
/*--------------------------------------------------------------------------*/ 
XML_SCILAB_IMPEXP int sci_xmlRead(char *fname, unsigned long fname_len);
XML_SCILAB_IMPEXP int sci_xmlXPath(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_XML_H__ */
/*--------------------------------------------------------------------------*/
