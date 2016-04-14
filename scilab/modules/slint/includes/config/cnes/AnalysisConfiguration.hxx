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

#ifndef __SLINT_CNES_ANALYSIS_CONFIGURATION_HXX__
#define __SLINT_CNES_ANALYSIS_CONFIGURATION_HXX__

#include "AnalysisConfigurationType.hxx"

namespace slint
{

namespace CNES
{

class AnalysisConfiguration
{

    const AnalysisConfigurationType analysisConfiguration;

public:

    AnalysisConfiguration(const AnalysisConfigurationType & _analysisConfiguration) : analysisConfiguration(_analysisConfiguration) { }

    inline const AnalysisConfigurationType & getAnalysisConfiguration() const
    {
        return analysisConfiguration;
    }

    static AnalysisConfiguration createFromXml(const std::wstring & path);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_ANALYSIS_CONFIGURATION_HXX__
