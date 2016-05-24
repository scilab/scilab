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

#ifndef __SLINT_CNES_ANALYSIS_CONFIGURATION_TYPE_HXX__
#define __SLINT_CNES_ANALYSIS_CONFIGURATION_TYPE_HXX__

#include <string>
#include <vector>
#include <libxml/tree.h>

#include "ExcludedProjectFileType.hxx"
#include "AnalysisRuleType.hxx"

namespace slint
{

namespace CNES
{

class AnalysisConfigurationType
{

    const std::string id;
    const std::string name;
    const std::string toolConfigurationId;
    const std::string projectDevLevel;

    std::vector<ExcludedProjectFileType> excludedProjectFile;
    std::vector<AnalysisRuleType> analysisRuleType;

public:

    AnalysisConfigurationType(const std::string & _id, const std::string & _name, const std::string & _toolConfigurationId, const std::string & _projectDevLevel) : id(_id), name(_name), toolConfigurationId(_toolConfigurationId), projectDevLevel(_projectDevLevel) { }

    inline const std::string & getId() const
    {
        return id;
    }
    inline const std::string & getName() const
    {
        return name;
    }
    inline const std::string & getToolConfigurationId() const
    {
        return toolConfigurationId;
    }
    inline const std::string & getProjectDevLevel() const
    {
        return projectDevLevel;
    }
    inline const std::vector<ExcludedProjectFileType> & getExcludedProjectFile() const
    {
        return excludedProjectFile;
    }
    inline const std::vector<AnalysisRuleType> & getAnalysisRuleType() const
    {
        return analysisRuleType;
    }

    inline void add(const ExcludedProjectFileType & epft)
    {
        excludedProjectFile.emplace_back(epft);
    }
    inline void add(const AnalysisRuleType & art)
    {
        analysisRuleType.emplace_back(art);
    }

    static AnalysisConfigurationType createFromXmlNode(xmlNode * node);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_ANALYSIS_CONFIGURATION_TYPE_HXX__
