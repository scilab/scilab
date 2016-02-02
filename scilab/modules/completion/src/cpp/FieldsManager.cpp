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
    //#include "code2str.h"
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
    int type = 0;
    const char ** fields = 0;
#if 0
    SciErr sciErr = getVarType(NULL, addr, &type);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (type == sci_mlist || type == sci_tlist)
    {
        int * strs = 0;
        const int nbItem = addr[1];
        sciErr = getListItemAddress(NULL, addr, 1, &strs);
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
#endif
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
            ret[i] = os_strdup(v.at(*len - i - 1).c_str());
        }
    }

    return ret;
}

void FieldsManager::clearFieldsGetter()
{
    for (auto field : typeToFieldsGetter)
    {
        delete field.second;
    }
}
}
