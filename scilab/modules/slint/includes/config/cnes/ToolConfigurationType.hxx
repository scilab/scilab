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

#ifndef __SLINT_CNES_TOOL_CONFIGURATION_TYPE_HXX__
#define __SLINT_CNES_TOOL_CONFIGURATION_TYPE_HXX__

#include <string>
#include <unordered_map>
#include <libxml/tree.h>

#include "RuleLinkType.hxx"

namespace slint
{

namespace CNES
{

class ToolConfigurationType
{

    const std::string id;
    const std::string name;
    const std::string version;

    std::unordered_map<std::string, RuleLinkType> ruleLink;

public:

    ToolConfigurationType(const std::string & _id, const std::string & _name, const std::string & _version) : id(_id), name(_name), version(_version) { }

    inline const std::string & getId() const
    {
        return id;
    }
    inline const std::string & getName() const
    {
        return name;
    }
    inline const std::string & getVersion() const
    {
        return version;
    }
    inline const std::unordered_map<std::string, RuleLinkType> & getRuleLink() const
    {
        return ruleLink;
    }

    inline void add(const RuleLinkType & rlt)
    {
        ruleLink.emplace(rlt.getAnalysisRuleId(), rlt);
    }

    static ToolConfigurationType createFromXmlNode(xmlNode * node);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_TOOL_CONFIGURATION_TYPE_HXX__
