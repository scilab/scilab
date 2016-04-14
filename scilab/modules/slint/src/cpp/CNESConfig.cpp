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

#include <unordered_map>

#include "config/cnes/CNESConfig.hxx"
#include "SLintOptions.hxx"

#define SLINT_INSERT_IN_MAP(name) callbacks.emplace(#name, &create<name##Checker>)

namespace slint
{
namespace CNES
{

std::unordered_map<std::string, CNESConfig::CBType> CNESConfig::callbacks = initCallbacks();

void CNESConfig::getOptions(const ToolConfigurationType & tct, const AnalysisConfigurationType & act, SLintOptions & options)
{
    for (const auto & art : act.getAnalysisRuleType())
    {
        auto i = callbacks.find(art.getId());
        if (i != callbacks.end())
        {
            if (SLintChecker * checker = i->second(tct, art))
            {
                options.addDefault(checker);
            }
        }
    }
}

void CNESConfig::getWString(const AnalysisRuleType & art, const std::string & name, std::wstring & value)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        value = scilab::UTF8::toWide(i->second.getTextValue());
    }
}

void CNESConfig::getString(const AnalysisRuleType & art, const std::string & name, std::string & value)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        value = i->second.getTextValue();
    }
}

void CNESConfig::getDouble(const AnalysisRuleType & art, const std::string & name, double & value)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        value = i->second.getNumericalValue();
    }
}

void CNESConfig::getInt(const AnalysisRuleType & art, const std::string & name, int & value)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        value = (int)i->second.getNumericalValue();
    }
}

void CNESConfig::getInt(const AnalysisRuleType & art, const std::string & name, int & min, int & max)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        min = (int)i->second.getValueMin();
        max = (int)i->second.getValueMax();
    }
}

void CNESConfig::getUInt(const AnalysisRuleType & art, const std::string & name, unsigned int & value)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        value = (unsigned int)i->second.getNumericalValue();
    }
}

void CNESConfig::getBool(const AnalysisRuleType & art, const std::string & name, bool & value)
{
    auto i = art.getAnalysisRuleParameter().find(name);
    if (i != art.getAnalysisRuleParameter().end())
    {
        value = i->second.getNumericalValue() != 0.;
    }
}

const std::wstring CNESConfig::getId(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    /*auto i = tct.getRuleLink().find(art.getId());
    if (i == tct.getRuleLink().end())
    {
        return scilab::UTF8::toWide(art.getId());
    }
    else
    {
        return scilab::UTF8::toWide(i->second.getStandardRuleId());
    }*/
    return scilab::UTF8::toWide(art.getId());
}

template<>
SLintChecker * CNESConfig::create<FunctionNameChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        std::wstring pattern;
        int min = -1;
        int max = -1;

        CNESConfig::getWString(art, "namePattern", pattern);
        CNESConfig::getInt(art, "length", min, max);

        return new FunctionNameChecker(CNESConfig::getId(tct, art), pattern, min, max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<VariableNameChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        std::wstring pattern;
        int min = -1;
        int max = -1;

        CNESConfig::getWString(art, "namePattern", pattern);
        CNESConfig::getInt(art, "length", min, max);

        return new VariableNameChecker(CNESConfig::getId(tct, art), pattern, min, max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<McCabeChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int max = -1;
        CNESConfig::getInt(art, "max", max);

        return new McCabeChecker(CNESConfig::getId(tct, art), max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<DecimalChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        std::wstring character;
        bool checkDot = false;

        CNESConfig::getWString(art, "character", character);
        CNESConfig::getBool(art, "checkDot", checkDot);

        return new DecimalChecker(CNESConfig::getId(tct, art), character, checkDot);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<LineLengthChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int max = -1;
        CNESConfig::getInt(art, "max", max);

        return new LineLengthChecker(CNESConfig::getId(tct, art), max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<LinesCountChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int max = -1;
        CNESConfig::getInt(art, "max", max);

        return new LinesCountChecker(CNESConfig::getId(tct, art), max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<StructChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        std::wstring pattern;
        int min = -1;
        int max = -1;

        CNESConfig::getWString(art, "fieldPattern", pattern);
        CNESConfig::getInt(art, "length", min, max);

        return new StructChecker(CNESConfig::getId(tct, art), pattern, min, max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<BreaksInLoopChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int maxBreaks = -1;
        int maxContinues = -1;

        CNESConfig::getInt(art, "maxBreaks", maxBreaks);
        CNESConfig::getInt(art, "maxContinues", maxContinues);

        return new BreaksInLoopChecker(CNESConfig::getId(tct, art), maxBreaks, maxContinues);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<NestedBlocksChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int max = -1;
        CNESConfig::getInt(art, "max", max);

        return new NestedBlocksChecker(CNESConfig::getId(tct, art), max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<ReturnsCountChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int max = -1;
        CNESConfig::getInt(art, "max", max);

        return new ReturnsCountChecker(CNESConfig::getId(tct, art), max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<ExpInCondChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        int max = -1;
        CNESConfig::getInt(art, "max", max);

        return new ExpInCondChecker(CNESConfig::getId(tct, art), max);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<IllegalCallsChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        std::vector<std::wstring> names;
        const auto & p = art.getAnalysisRuleParameter().equal_range("keyword");
        for (auto i = p.first; i != p.second; ++i)
        {
            const AnalysisRuleParameterType & arpt = i->second;
            names.emplace_back(scilab::UTF8::toWide(arpt.getTextValue()));
        }

        return new IllegalCallsChecker(CNESConfig::getId(tct, art), names);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<FunctionTestReturnChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        std::unordered_map<std::wstring, std::vector<unsigned int>> namesArgs;
        const auto & p = art.getAnalysisRuleParameter().equal_range("keyword");
        for (auto i = p.first; i != p.second; ++i)
        {
            const AnalysisRuleParameterType & arpt = i->second;
            const std::wstring name = scilab::UTF8::toWide(arpt.getTextValue());
            const double x = arpt.getNumericalValue();
            if (x > 0 && !name.empty())
            {
                const std::vector<unsigned int> arg = { (unsigned int)x };
                namesArgs.emplace(name, arg);
            }
        }

        return new FunctionTestReturnChecker(CNESConfig::getId(tct, art), namesArgs);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<SelectChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        bool checkDefault = false;
        bool checkHomogeneity = false;
        bool checkEmpty = false;
        bool checkOneCase = false;

        CNESConfig::getBool(art, "default", checkDefault);
        CNESConfig::getBool(art, "homogeneity", checkHomogeneity);
        CNESConfig::getBool(art, "empty", checkEmpty);
        CNESConfig::getBool(art, "oneCase", checkOneCase);

        return new SelectChecker(CNESConfig::getId(tct, art), checkDefault, checkHomogeneity, checkEmpty, checkOneCase);
    }

    return nullptr;
}

template<>
SLintChecker * CNESConfig::create<CommentRatioChecker>(const ToolConfigurationType & tct, const AnalysisRuleType & art)
{
    if (art.getActivation())
    {
        double ratio = 0;

        CNESConfig::getDouble(art, "ratioMin", ratio);
        if (ratio < 0)
        {
            ratio = 0;
        }
        else if (ratio > 1)
        {
            ratio = 1;
        }

        return new CommentRatioChecker(CNESConfig::getId(tct, art), ratio);
    }

    return nullptr;
}

std::unordered_map<std::string, CNESConfig::CBType> CNESConfig::initCallbacks()
{
    std::unordered_map<std::string, CBType> callbacks;

    SLINT_INSERT_IN_MAP(GlobalKeyword);
    SLINT_INSERT_IN_MAP(Redefinition);
    SLINT_INSERT_IN_MAP(Variables);
    SLINT_INSERT_IN_MAP(FunctionName);
    SLINT_INSERT_IN_MAP(FunctionArgs);
    SLINT_INSERT_IN_MAP(UselessArg);
    SLINT_INSERT_IN_MAP(UselessRet);
    SLINT_INSERT_IN_MAP(VariableName);
    SLINT_INSERT_IN_MAP(SingleInstr);
    SLINT_INSERT_IN_MAP(EmptyBlock);
    SLINT_INSERT_IN_MAP(SemicolonAtEOL);
    SLINT_INSERT_IN_MAP(MopenMclose);
    SLINT_INSERT_IN_MAP(McCabe);
    SLINT_INSERT_IN_MAP(Decimal);
    SLINT_INSERT_IN_MAP(Printf);
    SLINT_INSERT_IN_MAP(LineLength);
    SLINT_INSERT_IN_MAP(LinesCount);
    SLINT_INSERT_IN_MAP(Todo);
    SLINT_INSERT_IN_MAP(NaN);
    SLINT_INSERT_IN_MAP(EqEq);
    SLINT_INSERT_IN_MAP(UselessOp);
    SLINT_INSERT_IN_MAP(UnreachableCode);
    SLINT_INSERT_IN_MAP(Deprecated);
    SLINT_INSERT_IN_MAP(Select);
    SLINT_INSERT_IN_MAP(ImplicitList);
    SLINT_INSERT_IN_MAP(Struct);
    SLINT_INSERT_IN_MAP(LoadSave);
    SLINT_INSERT_IN_MAP(OldNot);
    SLINT_INSERT_IN_MAP(SpacesAroundOp);
    SLINT_INSERT_IN_MAP(SpacesInArgs);
    SLINT_INSERT_IN_MAP(BreaksInLoop);
    SLINT_INSERT_IN_MAP(NestedBlocks);
    SLINT_INSERT_IN_MAP(BracketedExp);
    SLINT_INSERT_IN_MAP(NotNot);
    SLINT_INSERT_IN_MAP(IllegalCalls);
    SLINT_INSERT_IN_MAP(ExpInCond);
    SLINT_INSERT_IN_MAP(CommentRatio);
    SLINT_INSERT_IN_MAP(FunctionArgsOrder);
    SLINT_INSERT_IN_MAP(FunctionTestReturn);
    SLINT_INSERT_IN_MAP(ReturnsCount);

    return callbacks;
}

} // namspace CNES
} // namespace slint

#undef SLINT_INSERT_IN_MAP
