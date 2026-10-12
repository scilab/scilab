/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Produces LSP semantic tokens by combining AST information with symbols
// available in the current Scilab runtime context.

#include <algorithm>
#include <cctype>
#include <set>

#include "lspEndpoint.hxx"
#include "lspProtocolUtils.hxx"
#include "lsp_dap_tools.hxx"
#include "documentAnalysisVisitor.hxx"
#include "UTF8.hxx"
#include "internal.hxx"
#include "visitor_common.hxx"

extern "C"
{
    #include "commandwords.h"
    #include "freeArrayOfString.h"
    #include "getdeprecated.h"
    #include "sci_malloc.h"
}

// These indexes must match the semantic token legend advertised at initialization.
static constexpr int SEMANTIC_MODIFIER_DEFINITION = 1 << 1;
static constexpr int SEMANTIC_MODIFIER_READONLY = 1 << 2;
static constexpr int SEMANTIC_MODIFIER_DEPRECATED = 1 << 4;
static constexpr int SEMANTIC_MODIFIER_DEFAULT_LIBRARY = 1 << 9;
static constexpr int SEMANTIC_MODIFIER_SCILAB_RUNTIME = 1 << 10;

static constexpr int SEMANTIC_TOKEN_VARIABLE = 8;
static constexpr int SEMANTIC_TOKEN_PARAMETER = 7;
static constexpr int SEMANTIC_TOKEN_PROPERTY = 9;
static constexpr int SEMANTIC_TOKEN_FUNCTION = 12;
static constexpr int SEMANTIC_TOKEN_MACRO = 14;
static constexpr int SEMANTIC_TOKEN_KEYWORD = 15;
static constexpr int SEMANTIC_TOKEN_STRING = 18;
static constexpr int SEMANTIC_TOKEN_OPERATOR_SPECIAL = 23;
static constexpr int SEMANTIC_TOKEN_FUNCTION_KEYWORD = 24;

static constexpr int RUNTIME_TOKEN_PRIORITY = 10;
static constexpr int RUNTIME_FIELD_TOKEN_PRIORITY = -1;

static void collect_runtime_semantic_tokens(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, std::vector<SemanticTokenEntry>& tokens,
    const RuntimeSnapshot* snapshot);
static void collect_parser_syntax_semantic_tokens(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, std::vector<SemanticTokenEntry>& tokens,
    bool includeRuntime);
static int semantic_token_type(const std::string& word, int& modifiers,
    const RuntimeSnapshot* snapshot);
static bool is_field_identifier(const std::string& line, size_t column);
static bool is_language_keyword(const std::string& word);
static bool is_class_body_keyword(const std::string& word);
static bool is_inside_class(const DocumentAnalysis& analysis, int line);
static bool is_class_section_header(const DocumentAnalysis& analysis,
    const std::string& line, int lineIndex, size_t start);
static bool is_argument_validator(const std::string& word);
static bool is_boolean_literal(const std::string& word);
static bool is_language_variable(const std::string& word);
static bool is_deprecated_symbol(const std::string& word);
static bool is_call_identifier(const std::string& line, size_t endColumn);
static bool is_identifier_start(char c);
static bool is_identifier_part(char c);

static bool is_string_syntax_range(const DocumentAnalysis& analysis,
    int line, int start, int length)
{
    for (const SyntaxTokenHint& token : analysis.syntaxTokens)
    {
        if (token.role == SyntaxRole::String && token.line == line
                && start >= token.start && start + length <= token.start + token.length)
        {
            return true;
        }
    }
    return false;
}

static void collect_macro_names(const std::vector<DocumentSymbolEntry>& symbols,
    std::set<std::string>& names)
{
    for (const DocumentSymbolEntry& symbol : symbols)
    {
        if (symbol.role == DocumentSymbolRole::Function
                || symbol.role == DocumentSymbolRole::Method
                || symbol.role == DocumentSymbolRole::Constructor)
        {
            names.insert(symbol.name);
        }
        collect_macro_names(symbol.children, names);
    }
}

static json semantic_tokens(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, const std::set<std::string>& openDocumentMacros,
    const RuntimeSnapshot* snapshot)
{
    std::vector<SemanticTokenEntry> tokens;
    collect_runtime_semantic_tokens(lines, analysis, tokens, snapshot);
    collect_parser_syntax_semantic_tokens(lines, analysis, tokens, snapshot == nullptr);

    if (!openDocumentMacros.empty())
    {
        for (const DocumentIdentifierEntry& identifier : analysis.identifiers)
        {
            if (!identifier.local && openDocumentMacros.find(identifier.name) != openDocumentMacros.end()
                    && identifier.range.firstLine >= 0
                    && static_cast<size_t>(identifier.range.firstLine) < lines.size()
                    && is_call_identifier(lines[identifier.range.firstLine], identifier.range.lastColumn))
            {
                tokens.push_back({
                    identifier.range.firstLine,
                    identifier.range.firstColumn,
                    identifier.range.lastColumn - identifier.range.firstColumn,
                    SEMANTIC_TOKEN_MACRO,
                    0,
                    RUNTIME_FIELD_TOKEN_PRIORITY
                });
            }
        }
    }

    // LSP encodes semantic tokens as deltas, so tokens must be ordered by position.
    std::sort(tokens.begin(), tokens.end(), [](const SemanticTokenEntry& left, const SemanticTokenEntry& right) {
        if (left.line != right.line)
        {
            return left.line < right.line;
        }
        if (left.start != right.start)
        {
            return left.start < right.start;
        }
        if (left.length != right.length)
        {
            return left.length < right.length;
        }
        if (left.priority != right.priority)
        {
            return left.priority < right.priority;
        }
        return left.type < right.type;
    });

    json data = json::array();
    int previous_line = 0;
    int previous_start = 0;
    bool has_previous = false;

    // Runtime and AST passes may produce the same range. Keep the
    // highest-priority type, but retain runtime provenance when both sources
    // agree on that type.
    for (size_t index = 0; index < tokens.size();)
    {
        SemanticTokenEntry token = tokens[index];
        size_t next = index + 1;
        while (next < tokens.size()
                && tokens[next].line == token.line
                && tokens[next].start == token.start
                && tokens[next].length == token.length)
        {
            if (tokens[next].type == token.type)
            {
                token.modifiers |= tokens[next].modifiers;
            }
            ++next;
        }

        int delta_line = has_previous ? token.line - previous_line : token.line;
        int delta_start = delta_line == 0 ? token.start - previous_start : token.start;

        data.push_back(delta_line);
        data.push_back(delta_start);
        data.push_back(token.length);
        data.push_back(token.type);
        data.push_back(token.modifiers);

        previous_line = token.line;
        previous_start = token.start;
        has_previous = true;
        index = next;
    }

    return data;
}

void collect_runtime_semantic_tokens(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, std::vector<SemanticTokenEntry>& tokens,
    const RuntimeSnapshot* snapshot)
{
    for (size_t line_index = 0; line_index < lines.size(); ++line_index)
    {
        const std::string& line = lines[line_index];
        bool in_string = false;
        char string_delimiter = '\0';

        for (size_t column = 0; column < line.size();)
        {
            char current = line[column];

            if (in_string)
            {
                if (current == string_delimiter)
                {
                    if (column + 1 < line.size() && line[column + 1] == string_delimiter)
                    {
                        column += 2;
                        continue;
                    }

                    in_string = false;
                }

                ++column;
                continue;
            }

            if (current == '"' || (current == '\'' && is_single_quote_string_start(line, column)))
            {
                in_string = true;
                string_delimiter = current;
                ++column;
                continue;
            }

            if (current == '/' && column + 1 < line.size() && line[column + 1] == '/')
            {
                break;
            }

            if (!is_identifier_start(current))
            {
                ++column;
                continue;
            }

            size_t start = column;
            ++column;
            while (column < line.size() && is_identifier_part(line[column]))
            {
                ++column;
            }

            std::string word = line.substr(start, column - start);
            if (is_string_syntax_range(analysis, static_cast<int>(line_index),
                    static_cast<int>(start), static_cast<int>(word.size())))
            {
                continue;
            }

            if (is_field_identifier(line, start))
            {
                continue;
            }

            // The AST visitor classifies "end" according to its syntax role,
            // including the special indexing form used like "$".
            if (word == "end")
            {
                continue;
            }

            // Boolean literals are parser tokens, not runtime variables. Keep
            // their constant color even when a syntax error prevents the AST
            // from containing the corresponding BoolExp.
            if (is_boolean_literal(word))
            {
                tokens.push_back({
                    static_cast<int>(line_index),
                    static_cast<int>(start),
                    static_cast<int>(word.size()),
                    SEMANTIC_TOKEN_VARIABLE,
                    SEMANTIC_MODIFIER_READONLY,
                    RUNTIME_TOKEN_PRIORITY
                });
                continue;
            }

            // varargin, varargout, nargin and nargout are defined by the
            // language in every macro body, so, like the boolean literals
            // above, they are readonly values the runtime context never
            // reports and they carry no scilabRuntime provenance. The AST pass
            // has priority over this one, so a function declaring varargin or
            // varargout keeps the input/output-argument role everywhere.
            if (is_language_variable(word))
            {
                tokens.push_back({
                    static_cast<int>(line_index),
                    static_cast<int>(start),
                    static_cast<int>(word.size()),
                    SEMANTIC_TOKEN_VARIABLE,
                    SEMANTIC_MODIFIER_READONLY,
                    RUNTIME_TOKEN_PRIORITY
                });
                continue;
            }

            bool keyword = is_language_keyword(word)
                && (is_class_body_keyword(word)
                    ? is_inside_class(analysis, static_cast<int>(line_index))
                        && (!is_call_identifier(line, column)
                            || is_class_section_header(analysis, line, static_cast<int>(line_index), start))
                    : !is_call_identifier(line, column));
            if (is_argument_validator(word) || keyword)
            {
                tokens.push_back({
                    static_cast<int>(line_index),
                    static_cast<int>(start),
                    static_cast<int>(word.size()),
                    SEMANTIC_TOKEN_KEYWORD,
                    0,
                    RUNTIME_TOKEN_PRIORITY
                });
                continue;
            }

            int modifiers = 0;
            int token_type = semantic_token_type(word, modifiers, snapshot);
            if (token_type < 0)
            {
                continue;
            }

            tokens.push_back({
                static_cast<int>(line_index),
                static_cast<int>(start),
                static_cast<int>(word.size()),
                token_type,
                modifiers,
                RUNTIME_TOKEN_PRIORITY
            });
        }
    }
}

void collect_parser_syntax_semantic_tokens(const std::vector<std::string>& lines,
    const DocumentAnalysis& analysis, std::vector<SemanticTokenEntry>& tokens,
    bool includeRuntime)
{
    for (const SyntaxTokenHint& token : analysis.syntaxTokens)
    {
        int type = SEMANTIC_TOKEN_FUNCTION_KEYWORD;
        switch (token.role)
        {
            case SyntaxRole::FunctionKeyword: type = SEMANTIC_TOKEN_FUNCTION_KEYWORD; break;
            case SyntaxRole::OperatorSpecial: type = SEMANTIC_TOKEN_OPERATOR_SPECIAL; break;
            case SyntaxRole::Property: type = SEMANTIC_TOKEN_PROPERTY; break;
            case SyntaxRole::Callable: type = SEMANTIC_TOKEN_FUNCTION; break;
            case SyntaxRole::Macro: type = SEMANTIC_TOKEN_MACRO; break;
            case SyntaxRole::Parameter: type = SEMANTIC_TOKEN_PARAMETER; break;
            case SyntaxRole::Variable: type = SEMANTIC_TOKEN_VARIABLE; break;
            case SyntaxRole::Constant:
                type = SEMANTIC_TOKEN_VARIABLE;
                break;
            case SyntaxRole::String: type = SEMANTIC_TOKEN_STRING; break;
        }
        int modifiers = token.definition ? SEMANTIC_MODIFIER_DEFINITION : 0;
        if (token.role == SyntaxRole::Constant)
        {
            modifiers |= SEMANTIC_MODIFIER_READONLY;
        }
        tokens.push_back({token.line, token.start, token.length, type, modifiers, 0});

        if (!includeRuntime || token.role != SyntaxRole::Property
                || token.runtimeExpression.empty() || !token.runtimeSafe)
        {
            continue;
        }

        types::InternalType* value = nullptr;
        std::string runtimeError = eval(token.runtimeExpression, &value);
        if (runtimeError.empty() && value != nullptr && (value->isMacro() || value->isMacroFile()))
        {
            tokens.push_back({token.line, token.start, token.length,
                SEMANTIC_TOKEN_MACRO, SEMANTIC_MODIFIER_SCILAB_RUNTIME,
                RUNTIME_FIELD_TOKEN_PRIORITY});
        }
    }
}

int semantic_token_type(const std::string& word, int& modifiers,
    const RuntimeSnapshot* snapshot)
{
    if (is_deprecated_symbol(word))
    {
        modifiers |= SEMANTIC_MODIFIER_DEPRECATED;
    }

    if (snapshot != nullptr)
    {
        const RuntimeSymbolSnapshot* symbol = snapshot->find(word);
        if (symbol == nullptr)
        {
            return modifiers & SEMANTIC_MODIFIER_DEPRECATED
                ? SEMANTIC_TOKEN_FUNCTION : -1;
        }

        modifiers |= SEMANTIC_MODIFIER_SCILAB_RUNTIME;
        switch (symbol->kind)
        {
            case RuntimeSymbolKind::Function:
                modifiers |= SEMANTIC_MODIFIER_DEFAULT_LIBRARY;
                return SEMANTIC_TOKEN_FUNCTION;
            case RuntimeSymbolKind::Macro:
                return SEMANTIC_TOKEN_MACRO;
            case RuntimeSymbolKind::Variable:
                if (symbol->readonly)
                {
                    modifiers |= SEMANTIC_MODIFIER_READONLY;
                }
                return SEMANTIC_TOKEN_VARIABLE;
        }
    }

    symbol::Context* pCtx = symbol::Context::getInstance();
    std::wstring wword = scilab::UTF8::toWide(word);
    symbol::Symbol symbolName(wword);
    types::InternalType* pIT = pCtx->get(symbolName);

    if (pIT == nullptr || !isRuntimeValueAvailable(pIT))
    {
        return modifiers & SEMANTIC_MODIFIER_DEPRECATED
            ? SEMANTIC_TOKEN_FUNCTION : -1;
    }

    switch (pIT->getType())
    {
        case types::InternalType::ScilabFunction:
            modifiers |= SEMANTIC_MODIFIER_DEFAULT_LIBRARY | SEMANTIC_MODIFIER_SCILAB_RUNTIME;
            return SEMANTIC_TOKEN_FUNCTION;
        case types::InternalType::ScilabMacro:
        case types::InternalType::ScilabMacroFile:
            modifiers |= SEMANTIC_MODIFIER_SCILAB_RUNTIME;
            return SEMANTIC_TOKEN_MACRO;
        default:
            modifiers |= SEMANTIC_MODIFIER_SCILAB_RUNTIME;
            // Protected runtime names are exposed as readonly semantic
            // variables, matching Scilab's predef("names") classification.
            if (isRuntimeValueProtected(wword))
            {
                modifiers |= SEMANTIC_MODIFIER_READONLY;
            }
            return SEMANTIC_TOKEN_VARIABLE;
    }
}

bool is_field_identifier(const std::string& line, size_t column)
{
    while (column > 0)
    {
        --column;
        unsigned char c = static_cast<unsigned char>(line[column]);
        if (!std::isspace(c))
        {
            if (line[column] != '.')
            {
                return false;
            }

            // The trailing dot of operators such as .*. is not a field
            // separator, even when spaces precede the following identifier.
            while (column > 0)
            {
                --column;
                c = static_cast<unsigned char>(line[column]);
                if (!std::isspace(c))
                {
                    return line[column] != '*' && line[column] != '/' && line[column] != '\\';
                }
            }

            return true;
        }
    }

    return false;
}

bool is_language_keyword(const std::string& word)
{
    static const std::set<std::string> keywords = []() {
        std::set<std::string> result;
        int count = 0;
        char** values = getcommandkeywords(&count);
        for (int index = 0; index < count; ++index)
        {
            result.emplace(values[index]);
            FREE(values[index]);
        }
        FREE(values);
        return result;
    }();
    return keywords.find(word) != keywords.end() || is_argument_validator(word);
}

// These three only open a block inside a classdef. Everywhere else they are
// the Scilab functions of the same name.
bool is_class_body_keyword(const std::string& word)
{
    return word == "properties" || word == "methods" || word == "enumeration";
}

bool is_inside_class(const DocumentAnalysis& analysis, int line)
{
    for (const DocumentRegionEntry& region : analysis.regions)
    {
        if (region.role == DocumentRegionRole::Class
                && line >= region.range.firstLine && line <= region.range.lastLine)
        {
            return true;
        }
    }

    return false;
}

// "methods (Static)" opens a section: its attributes are not call arguments.
// Only at the start of a line of the class body, not in a method.
bool is_class_section_header(const DocumentAnalysis& analysis,
    const std::string& line, int lineIndex, size_t start)
{
    if (line.find_first_not_of(" \t") != start)
    {
        return false;
    }

    for (const DocumentRegionEntry& region : analysis.regions)
    {
        if (region.role == DocumentRegionRole::Function
                && lineIndex >= region.range.firstLine && lineIndex <= region.range.lastLine)
        {
            return false;
        }
    }

    return true;
}

bool is_argument_validator(const std::string& word)
{
    static const std::set<std::string> validators = []() {
        std::set<std::string> result;
        int count = 0;
        char** values = getmustbekeywords(&count);
        for (int index = 0; index < count; ++index)
        {
            result.emplace(values[index]);
            FREE(values[index]);
        }
        FREE(values);
        return result;
    }();
    return validators.find(word) != validators.end();
}

bool is_boolean_literal(const std::string& word)
{
    return word == "%t" || word == "%T" || word == "%f" || word == "%F";
}

bool is_language_variable(const std::string& word)
{
    // Scilab defines these names in every macro body, so neither the runtime
    // snapshot nor symbol::Context ever reports them as symbols.
    return word == "varargin" || word == "varargout"
           || word == "nargin" || word == "nargout";
}

bool is_deprecated_symbol(const std::string& word)
{
    static const std::set<std::string> deprecated = []() {
        std::set<std::string> result;
        int size = 0;
        char** names = getDeprecatedNames(&size);
        for (int index = 0; index < size; ++index)
        {
            result.emplace(names[index]);
        }
        freeArrayOfString(names, size);
        return result;
    }();
    return deprecated.find(word) != deprecated.end();
}

bool is_call_identifier(const std::string& line, size_t endColumn)
{
    while (endColumn < line.size() && std::isspace(static_cast<unsigned char>(line[endColumn])))
    {
        ++endColumn;
    }
    return endColumn < line.size() && line[endColumn] == '(';
}

bool is_identifier_start(char c)
{
    unsigned char uc = static_cast<unsigned char>(c);
    return std::isalpha(uc) || c == '_' || c == '%' || c == '$';
}

bool is_identifier_part(char c)
{
    unsigned char uc = static_cast<unsigned char>(c);
    return std::isalnum(uc) || c == '_' || c == '%' || c == '$';
}

void LspEndpoint::handleSemanticTokens(const json& json_msg)
{
    json resp = response(json_msg);
    std::string uri = json_msg["params"]["textDocument"]["uri"];

    const std::vector<std::string>* found = findDocument(uri);
    if(found == nullptr)
    {
        resp["error"] = {
            { "code", InvalidParams },
            { "message", "File not found" }
        };
        sendMessage(resp.dump());
        return;
    }

    std::set<std::string> openDocumentMacros;
    for (const std::string& openedUri : visibleDocumentUris())
    {
        if (openedUri == uri)
        {
            continue;
        }

        const DocumentAnalysisCacheEntry* openedDocument = findDocumentAnalysis(openedUri);
        if (openedDocument != nullptr)
        {
            collect_macro_names(openedDocument->analysis.symbols, openDocumentMacros);
        }
    }

    const DocumentAnalysisCacheEntry* cached = findDocumentAnalysis(uri);
    if (cached == nullptr)
    {
        resp["result"]["data"] = json::array();
    }
    else
    {
        RuntimeTryLockGuard runtimeGuard;
        if (!runtimeGuard.ownsLock())
        {
            const RuntimeSnapshot& snapshot = runtimeSnapshot();
            if (snapshot.empty())
            {
                resp["error"] = {
                    { "code", ServerCancelled },
                    { "message", "Scilab runtime is busy" },
                    { "data", {{ "retriggerRequest", false }} }
                };
                sendMessage(resp.dump());
                return;
            }
            resp["result"]["data"] = semantic_tokens(*found,
                cached->analysis, openDocumentMacros, &snapshot);
            sendMessage(resp.dump());
            return;
        }
        resp["result"]["data"] = semantic_tokens(*found,
            cached->analysis, openDocumentMacros, nullptr);
    }
    sendMessage(resp.dump());
}
