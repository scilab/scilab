/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "runtimeSnapshot.hxx"

#include <algorithm>
#include <list>

#include "UTF8.hxx"
#include "context.hxx"
#include "internal.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "lsp_dap_tools.hxx"

namespace
{
std::string macroSourceFile(std::string filename)
{
    if (filename.size() >= 4 && filename.rfind(".bin") == filename.size() - 4)
    {
        filename.replace(filename.size() - 4, 4, ".sci");
    }
    return filename;
}

std::vector<std::wstring> collectNames(symbol::Context* context)
{
    std::vector<std::wstring> names;
    auto append = [&names](std::list<std::wstring>& values) {
        for (std::wstring& value : values)
        {
            names.push_back(std::move(value));
        }
        values.clear();
    };

    std::list<std::wstring> values;
    context->getVarsName(values);
    append(values);
    context->getFunctionsName(values);
    append(values);
    context->getMacrosName(values);
    append(values);

    std::sort(names.begin(), names.end());
    names.erase(std::unique(names.begin(), names.end()), names.end());
    return names;
}

std::string macroSignature(types::Macro* macro, const std::string& fallbackName)
{
    auto variableNames = [](const std::vector<symbol::Variable*>* variables) {
        std::string result;
        if (variables != nullptr)
        {
            for (symbol::Variable* variable : *variables)
            {
                if (!result.empty())
                {
                    result += ", ";
                }
                result += scilab::UTF8::toUTF8(variable->getSymbol().getName());
            }
        }
        return result;
    };

    std::string inputs = variableNames(macro->getInputs());
    std::string outputs = variableNames(macro->getOutputs());
    std::string name = macro->getName().empty()
        ? fallbackName : scilab::UTF8::toUTF8(macro->getName());
    std::string signature = "function ";
    if (!outputs.empty())
    {
        signature += macro->getOutputs()->size() > 1 ? "[" + outputs + "]" : outputs;
        signature += " = ";
    }
    return signature + name + "(" + inputs + ")";
}
}

void RuntimeSnapshot::capture()
{
    symbol::Context* context = symbol::Context::getInstance();
    std::vector<std::wstring> names = collectNames(context);

    std::vector<RuntimeSymbolSnapshot> symbols;
    std::vector<RuntimeMacroSnapshot> macros;
    symbols.reserve(names.size());
    for (const std::wstring& wideName : names)
    {
        symbol::Symbol symbolName(wideName);
        types::InternalType* value = context->get(symbolName);
        if (value == nullptr || !isRuntimeValueAvailable(value))
        {
            continue;
        }

        RuntimeSymbolSnapshot info;
        info.name = scilab::UTF8::toUTF8(wideName);
        info.readonly = isRuntimeValueProtected(wideName);
        if (value->isMacro())
        {
            info.kind = RuntimeSymbolKind::Macro;
            types::Macro* macro = value->getAs<types::Macro>();
            if (macro != nullptr)
            {
                RuntimeMacroSnapshot metadata;
                metadata.sourcePath = macroSourceFile(scilab::UTF8::toUTF8(macro->getFileName()));
                metadata.signature = macroSignature(macro, info.name);
                if (macro->getBody() != nullptr)
                {
                    const Location& location = macro->getBody()->getLocation();
                    metadata.sourceStartLine = std::max(0, location.first_line - 1);
                    metadata.sourceStartColumn = std::max(0, location.first_column - 1);
                    metadata.sourceEndLine = std::max(0, location.last_line - 1);
                    metadata.sourceEndColumn = std::max(0, location.last_column - 1);
                }
                info.macroIndex = static_cast<uint32_t>(macros.size());
                macros.push_back(std::move(metadata));
            }
        }
        else if (value->isMacroFile())
        {
            info.kind = RuntimeSymbolKind::Macro;
            types::MacroFile* macroFile = value->getAs<types::MacroFile>();
            if (macroFile != nullptr)
            {
                // getPath() is metadata only; unlike getMacro(), it does not
                // parse and load the macro file while capturing the snapshot.
                RuntimeMacroSnapshot metadata;
                metadata.sourcePath = macroSourceFile(scilab::UTF8::toUTF8(macroFile->getPath()));
                info.macroIndex = static_cast<uint32_t>(macros.size());
                macros.push_back(std::move(metadata));
            }
        }
        else if (value->isFunction())
        {
            info.kind = RuntimeSymbolKind::Function;
        }
        else
        {
            info.kind = RuntimeSymbolKind::Variable;
        }
        symbols.push_back(std::move(info));
    }

    std::sort(symbols.begin(), symbols.end(), [](const RuntimeSymbolSnapshot& left,
        const RuntimeSymbolSnapshot& right) {
        return left.name < right.name;
    });
    m_symbols.swap(symbols);
    m_macros.swap(macros);
}

bool RuntimeSnapshot::empty() const
{
    return m_symbols.empty();
}

const RuntimeSymbolSnapshot* RuntimeSnapshot::find(const std::string& name) const
{
    auto found = std::lower_bound(m_symbols.begin(), m_symbols.end(), name,
        [](const RuntimeSymbolSnapshot& symbol, const std::string& candidate) {
            return symbol.name < candidate;
        });
    return found != m_symbols.end() && found->name == name ? &*found : nullptr;
}

const std::vector<RuntimeSymbolSnapshot>& RuntimeSnapshot::symbols() const
{
    return m_symbols;
}

const RuntimeMacroSnapshot* RuntimeSnapshot::macro(const RuntimeSymbolSnapshot& symbol) const
{
    return symbol.macroIndex < m_macros.size() ? &m_macros[symbol.macroIndex] : nullptr;
}
