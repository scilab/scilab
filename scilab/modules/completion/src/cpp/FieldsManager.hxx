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
    static void clearFieldsGetter();

    /**
     * Retrieve the fields name for a given type and a given mlist
     * @param typeName the type name
     * @param mlist the mlist address
     * @return the array of fields name
     */
    static const char ** getFieldsForType(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize);

    static char ** getFieldPath(const char * _str, int * len);

    static const char ** getFields(int * addr, char ** fieldPath, const int fieldPathLen, int * fieldsSize);
};
}



#endif
