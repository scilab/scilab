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

#include <string>
#include "FieldsManager.hxx"
#include "XMLFieldsGetter.hxx"

extern "C"
{
#include "getfields.h"
}

using namespace org_modules_completion;

const char ** getFieldsForType(const char * typeName, int * mlist)
{
    return FieldsManager::getFieldsForType(std::string(typeName), mlist);
}

void initializeFieldsGetter()
{
    XMLFieldsGetter::initializeXML();
}
