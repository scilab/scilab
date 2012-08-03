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

#ifndef __FIELDSMANAGER_HXX__
#define __FIELDSMANAGER_HXX__

#include <map>
#include <string>
#include "FieldsGetter.hxx"

namespace org_modules_completion
{

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Class to handle the mapping between XMLObjects and their id
     */
    class FieldsManager
    {

        static std::map<const std::string, FieldsGetter *> typeToFieldsGetter;

    public :

        /**
         * Add a FieldGetter to the map
         * @param typeName the type name
         * @param getter a pointer to a FieldGetter
         */
        static void addFieldsGetter(const std::string & typeName, FieldsGetter * getter);

        /**
         * Retrieve the fields name for a given type and a given mlist
         * @param typeName the type name
         * @param mlist the mlist address
         * @return the array of fields name
         */
        static const char ** getFieldsForType(const std::string & typeName, int * mlist);
    };
}

#endif
