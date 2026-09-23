/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Builds diagnostics from the Scilab parser and AST analysis, including
// syntax errors and declarations that shadow existing Scilab symbols.

#include <algorithm>
#include <regex>
#include <set>
#include <tuple>

#include "lspDiagnostics.hxx"
#include "lsp_dap_tools.hxx"
#include "documentAnalysisVisitor.hxx"
#include "UTF8.hxx"
#include "internal.hxx"
#include "localization.hxx"
#include "visitor_common.hxx"
#include "runtimeaccess.hxx"
#include "runtimeSnapshot.hxx"

using json = nlohmann::json;

static json syntax_diagnostics(const std::vector<std::string>& lines);
static json syntax_diagnostics(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, const std::string& error,
    const RuntimeSnapshot* snapshot = nullptr);
static bool parse_syntax_error_diagnostic(const std::string& error, const std::vector<std::string>& lines, json& diagnostic);

json syntax_diagnostics_notification(const std::string& uri, const std::vector<std::string>& lines)
{
    return {
        { "jsonrpc", "2.0" },
        { "method", "textDocument/publishDiagnostics" },
        { "params", {
            { "uri", uri },
            { "diagnostics", syntax_diagnostics(lines) }
        }}
    };
}

json syntax_diagnostics_notification(const std::string& uri,
    const std::vector<std::string>& lines, const DocumentAnalysis& analysis,
    const std::string& parseError, const RuntimeSnapshot* snapshot)
{
    return {
        { "jsonrpc", "2.0" },
        { "method", "textDocument/publishDiagnostics" },
        { "params", {
            { "uri", uri },
            { "diagnostics", syntax_diagnostics(lines, analysis, parseError, snapshot) }
        }}
    };
}

json syntax_diagnostics(const std::vector<std::string>& lines)
{
    ast::Exp* tree = nullptr;
    std::string error;
    parseDocumentWithRecovery(lines, &tree, &error);
    DocumentAnalysis analysis;
    if (tree != nullptr)
    {
        analysis = analyzeDocument(tree, lines);
        delete tree;
    }

    return syntax_diagnostics(lines, analysis, error, nullptr);
}

json syntax_diagnostics(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, const std::string& error,
    const RuntimeSnapshot* snapshot)
{
    json diagnostics = json::array();
    if (!error.empty())
    {
        // The parser localizes this prefix (for example "Erreur : " in
        // French), so looking for the English "Error:" drops every syntax
        // diagnostic when Scilab uses another locale.
        const std::string errorPrefix = scilab::UTF8::toUTF8(_W("Error: "));
        size_t position = 0;
        while ((position = error.find(errorPrefix, position)) != std::string::npos)
        {
            size_t lineEnd = error.find_first_of("\r\n", position);
            std::string parserError = error.substr(position, lineEnd - position);
            json diagnostic;
            if (parse_syntax_error_diagnostic(parserError, lines, diagnostic))
            {
                diagnostics.push_back(diagnostic);
            }

            if (lineEnd == std::string::npos)
            {
                break;
            }
            position = lineEnd + 1;
        }
    }

    RuntimeTryLockGuard runtimeGuard;
    if (!runtimeGuard.ownsLock() && (snapshot == nullptr || snapshot->empty()))
    {
        return diagnostics;
    }

    symbol::Context* context = runtimeGuard.ownsLock()
        ? symbol::Context::getInstance() : nullptr;
    std::set<std::tuple<int, int, int>> reported;
    for (const DocumentIdentifierEntry& identifier : analysis.identifiers)
    {
        const DocumentRange& range = identifier.range;
        if (!identifier.definition
                || (identifier.role != SyntaxRole::Parameter && identifier.role != SyntaxRole::Variable)
                || range.firstLine < 0 || static_cast<size_t>(range.firstLine) >= lines.size()
                || range.firstColumn < 0 || range.lastColumn <= range.firstColumn
                || range.lastLine != range.firstLine
                || static_cast<size_t>(range.lastColumn) > lines[range.firstLine].size()
                || !reported.insert({range.firstLine, range.firstColumn,
                    range.lastColumn - range.firstColumn}).second)
        {
            continue;
        }

        const std::string& name = identifier.name;
        const RuntimeSymbolSnapshot* symbol = context == nullptr && snapshot != nullptr
            ? snapshot->find(name) : nullptr;
        types::InternalType* value = context != nullptr
            ? context->get(symbol::Symbol(scilab::UTF8::toWide(name))) : nullptr;
        if (value == nullptr && symbol == nullptr)
        {
            continue;
        }

        const char* kind = nullptr;
        if (symbol != nullptr)
        {
            if (symbol->kind == RuntimeSymbolKind::Function)
            {
                kind = "primitive";
            }
            else if (symbol->kind == RuntimeSymbolKind::Macro)
            {
                kind = "macro";
            }
        }
        else
        {
            switch (value->getType())
            {
                case types::InternalType::ScilabFunction: kind = "primitive"; break;
                case types::InternalType::ScilabMacro:
                case types::InternalType::ScilabMacroFile: kind = "macro"; break;
                default: break;
            }
        }
        if (kind == nullptr)
        {
            continue;
        }

        diagnostics.push_back({
            { "severity", 2 },
            { "source", "Scilab" },
            { "code", "shadowed-scilab-symbol" },
            { "message", "Declaration '" + name + "' shadows the Scilab " + kind + " with the same name." },
            { "range", {
                { "start", {{ "line", range.firstLine }, { "character", range.firstColumn }}},
                { "end", {{ "line", range.lastLine }, { "character", range.lastColumn }}}
            }}
        });
    }

    return diagnostics;
}

bool parse_syntax_error_diagnostic(const std::string& error, const std::vector<std::string>& lines, json& diagnostic)
{
    std::smatch match;
    const std::regex error_regex(R"(([0-9]+)\.([0-9]+)->([0-9]+)\.([0-9]+)\s+(.+))");
    if (!std::regex_search(error, match, error_regex))
    {
        return false;
    }

    int start_line = std::max(0, std::stoi(match[1].str()) - 1);
    int start_column = std::max(0, std::stoi(match[2].str()) - 1);
    int end_line = std::max(start_line, std::stoi(match[3].str()) - 1);
    int end_column = std::max(start_column + 1, std::stoi(match[4].str()) - 1);
    if (static_cast<size_t>(start_line) < lines.size())
    {
        start_column = std::min(start_column, static_cast<int>(lines[start_line].size()));
    }
    if (static_cast<size_t>(end_line) < lines.size())
    {
        end_column = std::min(end_column, static_cast<int>(lines[end_line].size()));
    }
    if (end_line == start_line && end_column <= start_column)
    {
        end_column = start_column + 1;
    }

    diagnostic = {
        { "severity", 1 },
        { "source", "Scilab parser" },
        { "message", match[5].str() },
        { "range", {
            { "start", {
                { "line", start_line },
                { "character", start_column }
            }},
            { "end", {
                { "line", end_line },
                { "character", end_column }
            }}
        }}
    };
    return true;
}
