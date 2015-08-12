/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __XMLNOTHANDLEDELEMENT_HXX__
#define __XMLNOTHANDLEDELEMENT_HXX__

#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLObject;

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Class for not handled nodes.
     */
    class XMLNotHandledElement:public XMLObject
    {
        const XMLObject & parent;
        xmlNode *node;

public:
        /**
         * Default constructor
         * @param parent the parent
         * @param node the not handled node
         */
          XMLNotHandledElement(const XMLObject & parent, xmlNode * node);

         ~XMLNotHandledElement();

        void *getRealXMLPointer() const;

        const XMLObject *getXMLObjectParent() const;
        const std::string toString() const;
    };
}

#endif
