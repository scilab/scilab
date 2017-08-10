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

#ifndef __SLINT_CNES_STANDARD_RULE_TYPE_HXX__
#define __SLINT_CNES_STANDARD_RULE_TYPE_HXX__

#include <string>
#include <vector>
#include <libxml/tree.h>

#include "StandardRuleParameterType.hxx"

namespace slint
{

namespace CNES
{

class StandardRuleType
{

    const std::string standardRuleId;
    const std::string category;
    const std::string entitled;
    const std::string desc;
    const std::string classification;
    const int maintainability;
    const int reliability;
    const int portability;
    const std::string justification;
    const std::string example;
    const std::string counterExample;

    std::vector<StandardRuleParameterType> parameter;

public:

    StandardRuleType(const std::string & _standardRuleId, const std::string & _category, const std::string & _entitled, const std::string & _desc, const std::string & _classification, const int _maintainability, const int _reliability, const int _portability, const std::string & _justification, const std::string & _example, const std::string & _counterExample) : standardRuleId(_standardRuleId), category(_category), entitled(_entitled), desc(_desc), classification(_classification), maintainability(_maintainability), reliability(_reliability), portability(_portability), justification(_justification), example(_example), counterExample(_counterExample) { }

    inline const std::string & getStandardRuleId() const
    {
        return standardRuleId;
    }
    inline const std::string & getCategory() const
    {
        return category;
    }
    inline const std::string & getEntitled() const
    {
        return entitled;
    }
    inline const std::string & getDesc() const
    {
        return desc;
    }
    inline const std::string & getClassification() const
    {
        return classification;
    }
    inline int getMaintainability() const
    {
        return maintainability;
    }
    inline int getReliability() const
    {
        return reliability;
    }
    inline int getPortability() const
    {
        return portability;
    }
    inline const std::string & getJustification() const
    {
        return justification;
    }
    inline const std::string & getExample() const
    {
        return example;
    }
    inline const std::string & getCounterExample() const
    {
        return counterExample;
    }
    inline const std::vector<StandardRuleParameterType> & getParameter() const
    {
        return parameter;
    }

    inline void add(const StandardRuleParameterType & pt)
    {
        parameter.emplace_back(pt);
    }

    static StandardRuleType createFromXmlNode(xmlNode * node);
};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_STANDARD_RULE_TYPE_HXX__
