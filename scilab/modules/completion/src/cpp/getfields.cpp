/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

#include <string>
#include "FieldsManager.hxx"
#include "XMLFieldsGetter.hxx"
#include "EOFieldsGetter.hxx"
#include "StructFieldsGetter.hxx"

extern "C"
{
#include "getfields.h"
}

using namespace org_modules_completion;

const char ** getFieldsForType(const char * typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    return FieldsManager::getFieldsForType(std::string(typeName), mlist, fieldPath, fieldPathLen, fieldsSize);
}

const char ** getFields(int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    return FieldsManager::getFields(mlist, fieldPath, fieldPathLen, fieldsSize);
}

char ** getFieldPath(const char * _str, int * len)
{
    return FieldsManager::getFieldPath(_str, len);
}

void initializeFieldsGetter()
{
    XMLFieldsGetter::initializeXML();
    EOFieldsGetter::initializeEO();
    FieldsManager::addFieldsGetter(std::string("st"), new StructFieldsGetter());
}
