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

#ifndef __SLINT_CNES_ANALYSIS_RULE_TYPE_HXX__
#define __SLINT_CNES_ANALYSIS_RULE_TYPE_HXX__

#include <string>
#include <unordered_map>

#include "AnalysisRuleParameterType.hxx"

namespace slint
{

namespace CNES
{

class AnalysisRuleType
{

    const std::string id;
    const bool activation;

    std::unordered_multimap<std::string, AnalysisRuleParameterType> analysisRuleParameter;

public:

    AnalysisRuleType(const std::string & _id, const bool _activation) : id(_id), activation(_activation) { }

    inline const std::string & getId() const
    {
        return id;
    }
    inline bool getActivation() const
    {
        return activation;
    }
    inline const std::unordered_multimap<std::string, AnalysisRuleParameterType> & getAnalysisRuleParameter() const
    {
        return analysisRuleParameter;
    }

    inline void add(const AnalysisRuleParameterType & arpt)
    {
        analysisRuleParameter.emplace(arpt.getParameterName(), arpt);
    }

    static AnalysisRuleType createFromXmlNode(xmlNode * node);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_ANALYSIS_RULE_TYPE_HXX__
