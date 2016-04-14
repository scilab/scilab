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

#include "config/cnes/RuleLinkType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

RuleLinkType RuleLinkType::createFromXmlNode(xmlNode * node)
{
    std::string standardRuleId;
    std::string analysisRuleId;
    std::string ruleCoverage;
    std::string standardId;

    slint::XMLtools::getString(node, "standardRuleId", standardRuleId);
    slint::XMLtools::getString(node, "analysisRuleId", analysisRuleId);
    slint::XMLtools::getString(node, "ruleCoverage", ruleCoverage);
    slint::XMLtools::getString(node, "standardId", standardId);

    return RuleLinkType(standardRuleId, analysisRuleId, ruleCoverage, standardId);
}

} // namespace CNES

} // namespace slint
