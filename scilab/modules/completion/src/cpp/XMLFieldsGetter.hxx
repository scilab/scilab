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

#ifndef __XMLFIELDSGETTER_HXX__
#define __XMLFIELDSGETTER_HXX__

#include <string>
#include <cstring>
#include "FieldsGetter.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
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
            cpy[i] = os_strdup(arr[i]);
        }

        return const_cast<const char **>(cpy);
    }
};
}

#endif
