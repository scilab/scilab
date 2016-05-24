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

#include "config/cnes/StandardRuleType.hxx"
#include "config/cnes/StandardRuleParameterType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

StandardRuleType StandardRuleType::createFromXmlNode(xmlNode * node)
{
    std::string standardRuleId;
    std::string category;
    std::string entitled;
    std::string desc;
    std::string classification;
    int maintainability = 0;
    int reliability = 0;
    int portability = 0;
    std::string justification;
    std::string example;
    std::string counterExample;

    slint::XMLtools::getString(node, "standardRuleId", standardRuleId);
    slint::XMLtools::getString(node, "category", category);
    slint::XMLtools::getString(node, "entitled", entitled);
    slint::XMLtools::getString(node, "desc", desc);
    slint::XMLtools::getString(node, "classification", classification);
    slint::XMLtools::getInt(node, "maintainability", maintainability);
    slint::XMLtools::getInt(node, "reliability", reliability);
    slint::XMLtools::getInt(node, "portability", portability);
    slint::XMLtools::getString(node, "justification", justification);
    slint::XMLtools::getString(node, "example", example);
    slint::XMLtools::getString(node, "counterExample", counterExample);

    StandardRuleType srt(standardRuleId, category, entitled, desc, classification, maintainability, reliability, portability, justification, example, counterExample);
    for (xmlNode * child = node->children; child; child = child->next)
    {
        srt.add(StandardRuleParameterType::createFromXmlNode(child));
    }

    return srt;
}

} // namespace CNES

} // namespace slint
