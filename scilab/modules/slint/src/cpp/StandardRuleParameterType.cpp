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

#include "config/cnes/StandardRuleParameterType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

StandardRuleParameterType StandardRuleParameterType::createFromXmlNode(xmlNode * node)
{
    std::string name;
    slint::XMLtools::getString(node, "standardRuleParameterName", name);

    StandardRuleParameterType srpt(name);
    for (xmlNode * child = node->children; child; child = child->next)
    {
        srpt.add(StandardRuleParameterValueType::createFromXmlNode(child));
    }

    return srpt;
}

} // namespace CNES

} // namespace slint
