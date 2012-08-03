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

#include <cstdio>

#include "FieldsManager.hxx"

namespace org_modules_completion
{

    std::map<const std::string, FieldsGetter *> FieldsManager::typeToFieldsGetter = std::map<const std::string, FieldsGetter *>();

    void FieldsManager::addFieldsGetter(const std::string & typeName, FieldsGetter * getter)
    {
        typeToFieldsGetter[typeName] = getter;
    }

    const char ** FieldsManager::getFieldsForType(const std::string & typeName, int * mlist)
    {
        std::map<const std::string, FieldsGetter *>::const_iterator it = typeToFieldsGetter.find(typeName);
        if (it == typeToFieldsGetter.end())
        {
            return 0;
        }
        return it->second->getFieldsName(typeName, mlist);
    }
}
