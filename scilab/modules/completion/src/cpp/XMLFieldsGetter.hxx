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

#ifndef __XMLFIELDSGETTER_HXX__
#define __XMLFIELDSGETTER_HXX__

#include <string>
#include <cstring>
#include "FieldsGetter.hxx"

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
class XMLFieldsGetter : public FieldsGetter
{

public :

    XMLFieldsGetter() { }

    virtual ~XMLFieldsGetter() { }

    virtual const char ** getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
    {
        return NULL;
    }

    /**
     * Initialization
     */
    static void initializeXML();

protected :

    /**
     * Copy a char * array and prepend typeName
     * @param typeName the type name
     * @param arr the arry to copy
     * @return the copy
     */
    static inline const char ** copy(const char ** arr, const int size)
    {
        char ** cpy = (char **)MALLOC(sizeof(char *) * size);
        for (int i = 0; i < size; i++)
        {
            cpy[i] = strdup(arr[i]);
        }

        return const_cast<const char **>(cpy);
    }
};
}

#endif
