/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <unordered_map>

#include "string.hxx"
#include "config/XMLConfig.hxx"
#include "SLintOptions.hxx"

#define SLINT_INSERT_IN_MAP(name) callbacks.emplace(#name, &createFromXmlNode<name##Checker>)

namespace slint
{

std::unordered_map<std::string, XMLConfig::CBType> XMLConfig::callbacks = initCallbacks();

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
        wchar_t c = character.empty() ? L'\0' : character.at(0);

        return new DecimalChecker(id, c, checkDot);
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
SLintChecker * XMLConfig::createFromXmlNode<StatInCondChecker>(xmlNode * node)
{
    bool enable = true;
    XMLtools::getBool(node, "enable", enable);
    if (enable)
    {
        std::wstring id;
        int max = -1;

        XMLtools::getWString(node, "id", id);
        XMLtools::getInt(node, "max", max);

        return new StatInCondChecker(id, max);
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
    SLINT_INSERT_IN_MAP(StatInCond);
    SLINT_INSERT_IN_MAP(CommentRatio);
    SLINT_INSERT_IN_MAP(FunctionArgsOrder);
    SLINT_INSERT_IN_MAP(FunctionTestReturn);
    SLINT_INSERT_IN_MAP(ReturnsCount);

    return callbacks;
}

} // namespace slint

#undef SLINT_INSERT_IN_MAP
