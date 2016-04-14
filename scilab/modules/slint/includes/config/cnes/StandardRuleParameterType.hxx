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

#ifndef __SLINT_CNES_STANDARD_RULE_PARAMETER_TYPE_HXX__
#define __SLINT_CNES_STANDARD_RULE_PARAMETER_TYPE_HXX__

#include <string>
#include <vector>
#include <libxml/tree.h>

#include "StandardRuleParameterValueType.hxx"

namespace slint
{

namespace CNES
{

class StandardRuleParameterType
{

    const std::string name;

    std::vector<StandardRuleParameterValueType> value;

public:

    StandardRuleParameterType(const std::string & _name) : name(_name) { }

    inline const std::string & getName() const
    {
        return name;
    }
    inline const std::vector<StandardRuleParameterValueType> & getParameterValue() const
    {
        return value;
    }
    inline void add(const StandardRuleParameterValueType & pvt)
    {
        value.emplace_back(pvt);
    }

    static StandardRuleParameterType createFromXmlNode(xmlNode * node);
};

} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_STANDARD_RULE_PARAMETER_TYPE_HXX__
