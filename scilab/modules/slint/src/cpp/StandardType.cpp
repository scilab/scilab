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

#include "config/cnes/StandardType.hxx"
#include "config/cnes/StandardRuleType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

StandardType StandardType::createFromXmlNode(xmlNode * node)
{
    std::string reference;
    std::string type;
    std::string version;
    std::string date;

    slint::XMLtools::getString(node, "standardReference", reference);
    slint::XMLtools::getString(node, "standardType", type);
    slint::XMLtools::getString(node, "standardVersion", version);
    slint::XMLtools::getString(node, "standardDate", date);

    StandardType st(reference, type, version, date);
    for (xmlNode * child = node->children; child; child = child->next)
    {
        st.add(StandardRuleType::createFromXmlNode(child));
    }

    return st;
}

} // namespace CNES

} // namespace slint
