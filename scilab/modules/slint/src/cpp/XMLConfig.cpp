/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <unordered_map>

#include "string.hxx"
#include "config/XMLConfig.hxx"
#include "SLintOptions.hxx"
#include "config/cnes/ToolConfiguration.hxx"
#include "config/cnes/AnalysisConfiguration.hxx"
#include "config/cnes/CNESConfig.hxx"

#define SLINT_INSERT_IN_MAP(name) callbacks.emplace(#name, &createFromXmlNode<name##Checker>)

namespace slint
{

std::unordered_map<std::string, XMLConfig::CBType> XMLConfig::callbacks = initCallbacks();

void XMLConfig::getOptions(types::String & str, SLintOptions & options)
{
    const std::wstring customer(str.get(0));
    if (customer == L"cnes")
    {
        const CNES::ToolConfiguration tc = CNES::ToolConfiguration::createFromXml(str.get(1));
        const CNES::AnalysisConfiguration ac = CNES::AnalysisConfiguration::createFromXml(str.get(2));
        const CNES::AnalysisConfigurationType & act = ac.getAnalysisConfiguration();
        CNES::CNESConfig::getOptions(tc.getToolConfiguration(), act, options);
        for (const CNES::ExcludedProjectFileType & epft : act.getExcludedProjectFile())
        {
            options.addExcludedFile(epft.getFilename());
        }
        options.setId(act.getId());
    }
}

void XMLConfig::getOptions(const std::wstring & path, SLintOptions & options)
{
    xmlDoc * doc = slint::XMLtools::readXML(path);
    xmlNode * root = xmlDocGetRootElement(doc);
    for (xmlNode * child = root->children; child; child = child->next)
    {
        const std::string nodeName((const char *)child->name);
        auto i = callbacks.find(nodeName);
        if (i != callbacks.end())
        {
            if (SLintChecker * checker = i->second(child))
            {
                options.addDefault(checker);
            }
        }
    }

    xmlFreeDoc(doc);
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<FunctionNameChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::wstring pattern;
        int min = -1;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getWString(node, "namePattern", pattern);
        XMLtools::getInt(node, "minLength", min);
        XMLtools::getInt(node, "maxLength", max);

        return new FunctionNameChecker(id, pattern, min, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<VariableNameChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::wstring pattern;
        int min = -1;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getWString(node, "namePattern", pattern);
        XMLtools::getInt(node, "minLength", min);
        XMLtools::getInt(node, "maxLength", max);

        return new VariableNameChecker(id, pattern, min, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<McCabeChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new McCabeChecker(id, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<DecimalChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::wstring character;
        bool checkDot = false;

        XMLtools::getWString(node, "id", id);
        XMLtools::getWString(node, "character", character);
        XMLtools::getBool(node, "checkDot", checkDot);

        return new DecimalChecker(id, character, checkDot);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<LineLengthChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new LineLengthChecker(id, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<LinesCountChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new LinesCountChecker(id, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<StructChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::wstring pattern;
        int min = -1;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getWString(node, "fieldPattern", pattern);
        XMLtools::getInt(node, "minLength", min);
        XMLtools::getInt(node, "maxLength", max);

        return new StructChecker(id, pattern, min, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<BreaksInLoopChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int maxBreaks = -1;
        int maxContinues = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "maxBreaks", maxBreaks);
        XMLtools::getInt(node, "maxContinues", maxContinues);

        return new BreaksInLoopChecker(id, maxBreaks, maxContinues);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<NestedBlocksChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new NestedBlocksChecker(id, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<ReturnsCountChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new ReturnsCountChecker(id, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<ExpInCondChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new ExpInCondChecker(id, max);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<IllegalCallsChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::vector<std::wstring> names;
        for (xmlNode * child = node->children; child; child = child->next)
        {
            const std::string nodeName((const char *)child->name);
            if (nodeName == "keyword")
            {
                std::wstring name;
                XMLtools::getWString(child, "name", name);
                if (!name.empty())
                {
                    names.emplace_back(name);
                }
            }
        }

        XMLtools::getWString(node, "id", id);

        return new IllegalCallsChecker(id, names);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<FunctionTestReturnChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::unordered_map<std::wstring, std::vector<unsigned int>> namesArgs;
        for (xmlNode * child = node->children; child; child = child->next)
        {
            const std::string nodeName((const char *)child->name);
            if (nodeName == "keyword")
            {
                std::wstring name;
                XMLtools::getWString(child, "name", name);
                if (!name.empty())
                {
                    std::vector<unsigned int> arg;
                    if (XMLtools::getUIntVector(child, "arg", arg))
                    {
                        namesArgs.emplace(name, arg);
                    }
                }
            }
        }

        XMLtools::getWString(node, "id", id);

        return new FunctionTestReturnChecker(id, namesArgs);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<SelectChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        bool checkDefault = false;
        bool checkHomogeneity = false;
        bool checkEmpty = false;
        bool checkOneCase = false;

        XMLtools::getWString(node, "id", id);
        XMLtools::getBool(node, "default", checkDefault);
        XMLtools::getBool(node, "homogeneity", checkHomogeneity);
        XMLtools::getBool(node, "empty", checkEmpty);
        XMLtools::getBool(node, "oneCase", checkOneCase);

        return new SelectChecker(id, checkDefault, checkHomogeneity, checkEmpty, checkOneCase);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<CommentRatioChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        double ratio = 0;

        XMLtools::getWString(node, "id", id);
        XMLtools::getDouble(node, "ratioMin", ratio);
        if (ratio < 0)
        {
            ratio = 0;
        }
        else if (ratio > 1)
        {
            ratio = 1;
        }

        return new CommentRatioChecker(id, ratio);
    }

    return nullptr;
}

template<>
SLintChecker * XMLConfig::createFromXmlNode<NotEqualChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        std::wstring op;

        XMLtools::getWString(node, "id", id);
        XMLtools::getWString(node, "operator", op);
        if (!op.empty() && (op == L"<>" || op == L"~=" || op == L"@="))
        {
            return new NotEqualChecker(id, op);
        }
    }

    return nullptr;
}

std::unordered_map<std::string, XMLConfig::CBType> XMLConfig::initCallbacks()
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
    SLINT_INSERT_IN_MAP(NotEqual);

    return callbacks;
}

} // namespace slint

#undef SLINT_INSERT_IN_MAP
