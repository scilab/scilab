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

#include "config/cnes/AnalysisRuleParameterType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

AnalysisRuleParameterType AnalysisRuleParameterType::createFromXmlNode(xmlNode * node)
{
    std::string parameterName;
    double numericalValue = 0;
    std::string textValue;
    double valueMin = 0;
    double valueMax = 0;

    slint::XMLtools::getString(node, "analysisRuleParameterName", parameterName);
    slint::XMLtools::getDouble(node, "analysisRuleNumericalValue", numericalValue);
    slint::XMLtools::getString(node, "analysisRuleTextValue", textValue);
    slint::XMLtools::getDouble(node, "analysisRuleValueMin", valueMin);
    slint::XMLtools::getDouble(node, "analysisRuleValueMax", valueMax);

    return AnalysisRuleParameterType(parameterName, numericalValue, textValue, valueMin, valueMax);
}

} // namespace CNES

} // namespace slint
