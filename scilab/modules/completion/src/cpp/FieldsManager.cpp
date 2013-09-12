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

#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstring>

#include "FieldsManager.hxx"
#include "UnknownMlistFieldsGetter.hxx"
#include "HandleFieldsGetter.hxx"

extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "code2str.h"
}

#include <iostream>

namespace org_modules_completion
{

std::map<const std::string, FieldsGetter *> FieldsManager::typeToFieldsGetter = std::map<const std::string, FieldsGetter *>();

void FieldsManager::addFieldsGetter(const std::string & typeName, FieldsGetter * getter)
{
    typeToFieldsGetter[typeName] = getter;
}

const char ** FieldsManager::getFieldsForType(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    std::map<const std::string, FieldsGetter *>::const_iterator it = typeToFieldsGetter.find(typeName);
    *fieldsSize = 0;
    if (it == typeToFieldsGetter.end())
    {
        UnknownMlistFieldsGetter getter;
        return getter.getFieldsName(typeName, mlist, fieldPath, fieldPathLen, fieldsSize);
    }
    return it->second->getFieldsName(typeName, mlist, fieldPath, fieldPathLen, fieldsSize);
}

const char ** FieldsManager::getFields(int * addr, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    int type;
    const char ** fields = 0;
    SciErr sciErr = getVarType(pvApiCtx, addr, &type);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (type == sci_mlist || type == sci_tlist)
    {
        int * strs = 0;
        const int nbItem = addr[1];
        sciErr = getListItemAddress(pvApiCtx, addr, 1, &strs);
        if (sciErr.iErr)
        {
            return 0;
        }

        const int r = strs[1];
        const int c = strs[2];
        int typeLen = strs[5] - 1;
        char * str = new char[typeLen + 1];
        code2str(&str, strs + 5 + r * c, typeLen);
        str[typeLen] = 0;
        fields = getFieldsForType(str, addr, fieldPath, fieldPathLen, fieldsSize);
        delete[] str;
    }
    else if (type == sci_handles)
    {
        HandleFieldsGetter getter;
        fields = getter.getFieldsName("", addr, fieldPath, fieldPathLen, fieldsSize);
    }

    return fields;
}

char ** FieldsManager::getFieldPath(const char * _str, int * len)
{
    std::vector<std::string> v;
    std::string str(_str);
    int pos = (int)str.length();
    const char symbs[27] = "+-*/\\([ ^,;={&|])}:\"\'><~@\t";
    char ** ret = 0;

    if (str.at(pos - 1) == '.')
    {
        pos--;
    }

    for (int i = pos - 1; i >= 0; i--)
    {
        const char c = str.at(i);
        if (c == '.')
        {
            v.push_back(str.substr(i + 1, pos - i - 1));
            pos = i;
        }
        else
        {
            for (int j = 0; j < 27; j++)
            {
                if (c == symbs[j])
                {
                    v.push_back(str.substr(i + 1, pos - i - 1));
                    goto finish;
                }
            }
        }
    }

    v.push_back(str.substr(0, pos));

finish :

    *len = (int)v.size();
    if (*len != 0)
    {
        ret = (char **) malloc(sizeof(char *) **len);
        for (int i = 0; i < *len; i++)
        {
            ret[i] = strdup(v.at(*len - i - 1).c_str());
        }
    }

    return ret;
}
}
