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

#include "XMLObject.hxx"
#include "XMLList.hxx"

namespace org_modules_xml
{
    XMLList::XMLList()
        : XMLObject()
    {
	scilabType = XMLLIST;
    }

    std::string * XMLList::toString()
    {
	std::string str = "XML List\n";
        str += "size: " + intToStr(getSize());
	
        return new std::string(str);
    }
}
