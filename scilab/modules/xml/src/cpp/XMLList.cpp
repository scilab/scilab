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

#include "XMLObject.hxx"
#include "XMLList.hxx"

namespace org_modules_xml
{
XMLList::XMLList(): XMLObject(), size(0)
{
    scilabType = XMLLIST;
}

const std::string XMLList::toString() const
{
    std::ostringstream oss;

    oss << "XML List" << std::endl;
    oss << "size: " << getSize();

    return oss.str();
}
}
