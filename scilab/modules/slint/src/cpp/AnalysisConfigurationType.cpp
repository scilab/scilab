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

#include "config/cnes/AnalysisConfigurationType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

AnalysisConfigurationType AnalysisConfigurationType::createFromXmlNode(xmlNode * node)
{
    std::string id;
    std::string name;
    std::string toolConfigurationId;
    std::string projectDevLevel;

    slint::XMLtools::getString(node, "analysisConfigurationId", id);
    slint::XMLtools::getString(node, "analysisConfigurationName", name);
    slint::XMLtools::getString(node, "toolConfigurationId", toolConfigurationId);
    slint::XMLtools::getString(node, "projectDevLevel", projectDevLevel);

    AnalysisConfigurationType act(id, name, toolConfigurationId, projectDevLevel);
    for (xmlNode * child = node->children; child; child = child->next)
    {
        std::string name((const char *)child->name);
        if (name == "excludedProjectFile")
        {
            act.add(ExcludedProjectFileType::createFromXmlNode(child));
        }
        else if (name == "analysisRule")
        {
            act.add(AnalysisRuleType::createFromXmlNode(child));
        }
    }

    return act;
}

} // namespace CNES

} // namespace slint
