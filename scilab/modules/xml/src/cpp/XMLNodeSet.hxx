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

#ifndef __XMLNODESET_HXX__
#define __XMLNODESET_HXX__

#include "XMLList.hxx"
#include "xml.h"

namespace org_modules_xml
{
    class XMLDocument;
    class XMLElement;

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Class to wrap the list of the elements returned by a XPath query
     */
    class XMLNodeSet : public XMLList
    {

        const XMLDocument & doc;
        xmlNodeSet * nodeSet;

    public :

        /**
         * Default constructor
         * @param doc the document where the node set is existing
         * @param nodeSet a xmlNodeSet
         */
        XMLNodeSet(const XMLDocument & doc, xmlNodeSet * nodeSet);

        ~XMLNodeSet();

        void * getRealXMLPointer() const;

        const char ** getContentFromList() const;

        const XMLObject * getXMLObjectParent() const;
        const XMLObject * getListElement(int index);
    };
}

#endif
