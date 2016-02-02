/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
