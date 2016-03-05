/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
class XMLNotHandledElement: public XMLObject
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
