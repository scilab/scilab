/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "config/cnes/Standard.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

Standard Standard::createFromXml(const std::wstring & path)
{
    xmlDoc * doc = slint::XMLtools::readXML(path);
    xmlNode * root = xmlDocGetRootElement(doc);

    Standard standard(StandardType::createFromXmlNode(root));
    xmlFreeDoc(doc);

    return standard;
}

} // namespace CNES

} // namespace slint
