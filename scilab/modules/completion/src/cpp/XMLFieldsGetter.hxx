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
    class XMLFieldsGetter : FieldsGetter
    {

    public :

        virtual const char ** getFieldsName(const std::string & typeName, int * mlist) const;

        /**
         * Initialization
         */
        static void initializeXML()
            {
                XMLFieldsGetter * getter = new XMLFieldsGetter();
                FieldsManager::addFieldsGetter(std::string("XMLDoc"), getter);
                FieldsManager::addFieldsGetter(std::string("XMLElem"), getter);
                FieldsManager::addFieldsGetter(std::string("XMLNs"), getter);
                FieldsManager::addFieldsGetter(std::string("XMLAttr"), getter);
                FieldsManager::addFieldsGetter(std::string("XMLList"), getter);
                FieldsManager::addFieldsGetter(std::string("XMLSet"), getter);
            }

    private :

        XMLFieldsGetter() { }

        ~XMLFieldsGetter() { }

        /**
         * Copy a char * array and prepend typeName
         * @param typeName the type name
         * @param arr the arry to copy
         * @return the copy
         */
        static inline const char ** copy(const char * typeName, const char ** arr)
            {
                int size = 0;
                for (; arr[size]; size++);
                char ** cpy = (char **)MALLOC(sizeof(char *) * (size + 2));
                for (int i = 0; i < size; i++)
                {
                    cpy[i + 1] = strdup(arr[i]);
                }
                cpy[0] = strdup(typeName);
                cpy[size + 1] = 0;

                return const_cast<const char **>(cpy);
            }
    };
}

#endif
