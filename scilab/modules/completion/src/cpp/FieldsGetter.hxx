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
        virtual const char ** getFieldsName(const std::string & typeName, int * mlist) const = 0;
    };
}

#endif
