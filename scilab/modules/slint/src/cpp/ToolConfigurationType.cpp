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

#include "config/cnes/ToolConfigurationType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

ToolConfigurationType ToolConfigurationType::createFromXmlNode(xmlNode * node)
{
    std::string id;
    std::string name;
    std::string version;

    slint::XMLtools::getString(node, "toolConfigurationId", id);
    slint::XMLtools::getString(node, "toolName", name);
    slint::XMLtools::getString(node, "toolVersion", version);

    ToolConfigurationType tct(id, name, version);
    for (xmlNode * child = node->children; child; child = child->next)
    {
        tct.add(RuleLinkType::createFromXmlNode(child));
    }

    return tct;
}

} // namespace CNES

} // namespace slint
