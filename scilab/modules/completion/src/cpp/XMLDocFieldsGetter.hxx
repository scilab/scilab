/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __XMLDOCFIELDSGETTER_HXX__
#define __XMLDOCFIELDSGETTER_HXX__

#include <string>
#include <cstring>

#include "FieldsGetter.hxx"
#include "XMLFieldsGetter.hxx"
#include "XMLDocument.hxx"
#include "xmlObjects.h"

extern "C"
{
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}

namespace org_modules_completion
{
/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to manage the retrievment of the fields name of a XML object
 */
class XMLDocFieldsGetter : public XMLFieldsGetter
{

public :

    XMLDocFieldsGetter() { }

    virtual ~XMLDocFieldsGetter() { }

    virtual const char ** getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const;

    static const char ** getFieldsName(const org_modules_xml::XMLDocument * doc, char ** fieldPath, const int fieldPathLen, int * fieldsSize);
};
}

#endif
