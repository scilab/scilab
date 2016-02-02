/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
