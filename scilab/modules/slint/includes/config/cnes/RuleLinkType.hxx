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

#ifndef __SLINT_CNES_RULE_LINK_TYPE_HXX__
#define __SLINT_CNES_RULE_LINK_TYPE_HXX__

#include <string>
#include <libxml/tree.h>

namespace slint
{

namespace CNES
{

class RuleLinkType
{

    const std::string standardRuleId;
    const std::string analysisRuleId;
    const std::string ruleCoverage;
    const std::string standardId;

public:

    RuleLinkType(const std::string & _standardRuleId, const std::string & _analysisRuleId, const std::string & _ruleCoverage, const std::string & _standardId) : standardRuleId(_standardRuleId), analysisRuleId(_analysisRuleId), ruleCoverage(_ruleCoverage), standardId(_standardId) { }

    inline const std::string & getStandardRuleId() const
    {
        return standardRuleId;
    }
    inline const std::string & getAnalysisRuleId() const
    {
        return analysisRuleId;
    }
    inline const std::string & getRuleCoverage() const
    {
        return ruleCoverage;
    }
    inline const std::string & getStandardId() const
    {
        return standardId;
    }

    static RuleLinkType createFromXmlNode(xmlNode * node);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_RULE_LINK_TYPE_HXX__
