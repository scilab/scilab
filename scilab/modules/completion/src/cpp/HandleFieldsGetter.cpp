/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "FieldsManager.hxx"
#include "HandleFieldsGetter.hxx"


namespace org_modules_completion
{

const char ** HandleFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    return getFieldsName(mlist, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** HandleFieldsGetter::getFieldsName(int * handle, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    return const_cast<const char **>(completionOnHandleGraphicsProperties("", fieldsSize));
}
}
