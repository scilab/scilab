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

#ifndef __FIELDSGETTER_HXX__
#define __FIELDSGETTER_HXX__

namespace org_modules_completion
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to retrieve the fields name of an uncomplete mlist
 */
class FieldsGetter
{

public :

    /**
     * Default constructor
     */
    FieldsGetter() { }

    /**
     * Destructor
     */
    virtual ~FieldsGetter() { }

    /**
     * @param typeName the type name
     * @param mlist the mlist addr
     */
    virtual const char ** getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const = 0;
};
}

#endif
