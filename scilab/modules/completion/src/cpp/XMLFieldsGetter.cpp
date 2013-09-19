/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "api_scilab.h"
}

#include "FieldsManager.hxx"
#include "XMLFieldsGetter.hxx"
#include "XMLDocFieldsGetter.hxx"
#include "XMLElemFieldsGetter.hxx"
#include "XMLAttrFieldsGetter.hxx"
#include "XMLNsFieldsGetter.hxx"
#include "XMLListFieldsGetter.hxx"
#include "XMLSetFieldsGetter.hxx"


using namespace org_modules_xml;

namespace org_modules_completion
{

void XMLFieldsGetter::initializeXML()
{
    FieldsManager::addFieldsGetter(std::string("XMLDoc"), new XMLDocFieldsGetter());
    FieldsManager::addFieldsGetter(std::string("XMLElem"), new XMLElemFieldsGetter());
    FieldsManager::addFieldsGetter(std::string("XMLNs"), new XMLNsFieldsGetter());
    FieldsManager::addFieldsGetter(std::string("XMLAttr"), new XMLAttrFieldsGetter());
    FieldsManager::addFieldsGetter(std::string("XMLList"), new XMLListFieldsGetter());
    FieldsManager::addFieldsGetter(std::string("XMLSet"), new XMLSetFieldsGetter());
}
}
