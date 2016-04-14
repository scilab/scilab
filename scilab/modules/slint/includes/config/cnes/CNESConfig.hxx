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

#ifndef __SLINT_CNES_CONFIG_HXX__
#define __SLINT_CNES_CONFIG_HXX__

#include "checkers/AllCheckers.hxx"
#include "CNESException.hxx"
#include "ToolConfigurationType.hxx"
#include "AnalysisConfigurationType.hxx"
#include "UTF8.hxx"

namespace slint
{

class SLintOptions;

namespace CNES
{
class CNESConfig
{

    typedef SLintChecker * (*CBType)(const ToolConfigurationType &, const AnalysisRuleType &);

    static std::unordered_map<std::string, CBType> callbacks;

public:

    static void getOptions(const ToolConfigurationType & tct, const AnalysisConfigurationType & act, SLintOptions & options);

private:

    static void getWString(const AnalysisRuleType & art, const std::string & name, std::wstring & value);
    static void getString(const AnalysisRuleType & art, const std::string & name, std::string & value);
    static void getDouble(const AnalysisRuleType & art, const std::string & name, double & value);
    static void getInt(const AnalysisRuleType & art, const std::string & name, int & value);
    static void getInt(const AnalysisRuleType & art, const std::string & name, int & min, int & max);
    static void getUInt(const AnalysisRuleType & art, const std::string & name, unsigned int & value);
    static void getBool(const AnalysisRuleType & art, const std::string & name, bool & value);
    static const std::wstring getId(const ToolConfigurationType & tct, const AnalysisRuleType & art);

    template<typename T>
    static SLintChecker * create(const ToolConfigurationType & tct, const AnalysisRuleType & art)
    {
        if (art.getActivation())
        {
            return new T(getId(tct, art));
        }

        return nullptr;
    }

    static std::unordered_map<std::string, CBType> initCallbacks();

};

} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_CONFIG_HXX__
