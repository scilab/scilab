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

#include "checkers/AllCheckers.hxx"
#include "config/cnes/AnalysisRuleType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

AnalysisRuleType AnalysisRuleType::createFromXmlNode(xmlNode * node)
{
    std::string id;
    bool activation = false;

    slint::XMLtools::getString(node, "analysisRuleId", id);
    slint::XMLtools::getBool(node, "analysisRuleActivation", activation);

    AnalysisRuleType art(id, activation);
    for (xmlNode * child = node->children; child; child = child->next)
    {
        art.add(AnalysisRuleParameterType::createFromXmlNode(child));
    }

    return art;
}

} // namespace CNES

} // namespace slint
