/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XMLREMOVABLE_HXX__
#define __XMLREMOVABLE_HXX__

namespace org_modules_xml
{
    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Pure virtual class for XMLElement and XMLList
     */
    class XMLRemovable
    {

public:

        /**
         * Remove this object or this list of objects from the document
         */
        virtual void remove() const = 0;
    };
}

#endif
