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

#ifndef __SLINT_CNES_ANALYSIS_RULE_PARAMETER_TYPE_HXX__
#define __SLINT_CNES_ANALYSIS_RULE_PARAMETER_TYPE_HXX__

#include <string>
#include <libxml/tree.h>

namespace slint
{

namespace CNES
{

class AnalysisRuleParameterType
{

    const std::string parameterName;
    const double numericalValue;
    const std::string textValue;
    const double valueMin;
    const double valueMax;

public:

    AnalysisRuleParameterType(const std::string & _parameterName, const double _numericalValue, const std::string & _textValue, const double _valueMin, const double _valueMax) : parameterName(_parameterName), numericalValue(_numericalValue), textValue(_textValue), valueMin(_valueMin), valueMax(_valueMax) { }

    inline const std::string & getParameterName() const
    {
        return parameterName;
    }
    inline double getNumericalValue() const
    {
        return numericalValue;
    }
    inline const std::string & getTextValue() const
    {
        return textValue;
    }
    inline double getValueMin() const
    {
        return valueMin;
    }
    inline double getValueMax() const
    {
        return valueMax;
    }

    static AnalysisRuleParameterType createFromXmlNode(xmlNode * node);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_ANALYSIS_RULE_TYPE_HXX__
