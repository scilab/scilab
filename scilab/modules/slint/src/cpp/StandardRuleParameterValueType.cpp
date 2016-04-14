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

#include "config/cnes/StandardRuleParameterValueType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

StandardRuleParameterValueType StandardRuleParameterValueType::createFromXmlNode(xmlNode * node)
{
    double numericalValue = 0;
    std::string textValue;
    double valueMin = 0;
    double valueMax = 0;
    std::string devLevel;

    slint::XMLtools::getDouble(node, "standardRuleNumericalValue", numericalValue);
    slint::XMLtools::getString(node, "standardRuleTextValue", textValue);
    slint::XMLtools::getDouble(node, "standardRuleValueMin", valueMin);
    slint::XMLtools::getDouble(node, "standardRuleValueMax", valueMax);
    slint::XMLtools::getString(node, "standardRuleDevLevel", devLevel);

    return StandardRuleParameterValueType(numericalValue, textValue, valueMin, valueMax, devLevel);
}

} // namespace CNES

} // namespace slint

