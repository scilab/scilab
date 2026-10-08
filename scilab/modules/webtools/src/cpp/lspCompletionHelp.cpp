/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Implements editor assistance related to completion and contextual help:
// completion items, signature help, and hover information.

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <set>
#include <sstream>

#include "lspEndpoint.hxx"
#include "lspProtocolUtils.hxx"
#include "lsp_dap_tools.hxx"
#include "documentAnalysisVisitor.hxx"
#include "UTF8.hxx"
#include "internal.hxx"
#include "callable.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "visitor_common.hxx"
#include "url_tools.hxx"

static const DocumentSymbolEntry* find_document_symbol(
    const std::vector<DocumentSymbolEntry>& symbols, const std::string& name)
{
    for (const DocumentSymbolEntry& symbol : symbols)
    {
        if (symbol.name == name
                && (symbol.role == DocumentSymbolRole::Function
                    || symbol.role == DocumentSymbolRole::Method
                    || symbol.role == DocumentSymbolRole::Constructor))
        {
            return &symbol;
        }

        const DocumentSymbolEntry* child = find_document_symbol(symbol.children, name);
        if (child != nullptr)
        {
            return child;
        }
    }
    return nullptr;
}

static std::string document_symbol_signature(const DocumentSymbolEntry* symbol,
    const std::vector<std::string>& lines)
{
    if (symbol == nullptr || symbol->range.firstLine < 0
            || static_cast<size_t>(symbol->range.firstLine) >= lines.size())
    {
        return std::string();
    }

    const std::string& line = lines[symbol->range.firstLine];
    size_t first = line.find_first_not_of(" \t");
    size_t last = line.find_last_not_of(" \t\r");
    if (first == std::string::npos)
    {
        return std::string();
    }

    return line.substr(first, last - first + 1);
}

// A hover shows the calling form, as the help does: "y = foo(x)".
static std::string hover_prototype(const std::string& signature)
{
    const std::string keyword = "function";
    if (signature.compare(0, keyword.size(), keyword) == 0
            && signature.size() > keyword.size()
            && std::isspace(static_cast<unsigned char>(signature[keyword.size()])))
    {
        size_t start = signature.find_first_not_of(" \t", keyword.size());
        return start == std::string::npos ? signature : signature.substr(start);
    }
    return signature;
}

static std::string document_macro_hover(const std::string& word,
    const DocumentSymbolEntry* symbol, const std::vector<std::string>& lines,
    const std::string& source)
{
    std::string signature = document_symbol_signature(symbol, lines);
    return signature.empty()
        ? "`" + word + "` - macro defined in " + source
        : "```scilab\n" + hover_prototype(signature) + "\n```\n\nMacro defined in " + source + ".";
}

static std::string runtime_macro_signature(types::InternalType* value, const std::string& fallbackName)
{
    types::Macro* macro = nullptr;
    if (value != nullptr && value->isMacro())
    {
        macro = value->getAs<types::Macro>();
    }
    else if (value != nullptr && value->isMacroFile())
    {
        macro = value->getAs<types::MacroFile>()->getMacro();
    }
    if (macro == nullptr)
    {
        return std::string();
    }

    auto variables = [](const std::vector<symbol::Variable*>* values) {
        std::string names;
        if (values == nullptr)
        {
            return names;
        }
        for (symbol::Variable* variable : *values)
        {
            if (!names.empty())
            {
                names += ", ";
            }
            names += scilab::UTF8::toUTF8(variable->getSymbol().getName());
        }
        return names;
    };

    std::string inputs = variables(macro->getInputs());
    std::string outputs = variables(macro->getOutputs());
    std::string macroName = macro->getName().empty()
        ? fallbackName : scilab::UTF8::toUTF8(macro->getName());
    std::string signature = "function ";
    if (!outputs.empty())
    {
        signature += macro->getOutputs()->size() > 1 ? "[" + outputs + "]" : outputs;
        signature += " = ";
    }
    return signature + macroName + "(" + inputs + ")";
}

static std::string join_names(const std::vector<std::string>& names)
{
    std::string joined;
    for (const std::string& name : names)
    {
        if (!joined.empty())
        {
            joined += ", ";
        }
        joined += name;
    }
    return joined;
}

static json callable_signature(const std::string& name,
    const std::vector<std::string>& inputs, const std::vector<std::string>& outputs)
{
    std::string label = "function ";
    if (!outputs.empty())
    {
        label += outputs.size() > 1 ? "[" + join_names(outputs) + "]" : outputs.front();
        label += " = ";
    }
    label += name + "(" + join_names(inputs) + ")";

    json parameters = json::array();
    for (const std::string& input : inputs)
    {
        parameters.push_back({ { "label", input } });
    }
    return { { "label", label }, { "parameters", parameters } };
}

static json document_callable_signatures(const DocumentAnalysis& analysis,
    const std::string& name)
{
    // Callables are collected before their bodies, so reverse order selects
    // the innermost function first when functions are nested.
    for (auto it = analysis.callables.rbegin(); it != analysis.callables.rend(); ++it)
    {
        const DocumentCallableEntry& callable = *it;
        if (callable.name == name)
        {
            return json::array({ callable_signature(callable.name,
                callable.inputs, callable.outputs) });
        }
    }
    return json::array();
}

static json runtime_macro_signatures(types::InternalType* value,
    const std::string& fallbackName)
{
    types::Macro* macro = nullptr;
    if (value != nullptr && value->isMacro())
    {
        macro = value->getAs<types::Macro>();
    }
    else if (value != nullptr && value->isMacroFile())
    {
        macro = value->getAs<types::MacroFile>()->getMacro();
    }
    if (macro == nullptr)
    {
        return json::array();
    }

    auto names = [](const std::vector<symbol::Variable*>* variables) {
        std::vector<std::string> result;
        if (variables != nullptr)
        {
            for (symbol::Variable* variable : *variables)
            {
                result.push_back(scilab::UTF8::toUTF8(variable->getSymbol().getName()));
            }
        }
        return result;
    };
    std::string name = macro->getName().empty()
        ? fallbackName : scilab::UTF8::toUTF8(macro->getName());
    return json::array({ callable_signature(name,
        names(macro->getInputs()), names(macro->getOutputs())) });
}

static std::string runtime_macro_name(types::InternalType* value,
    const std::string& fallbackName)
{
    types::Callable* callable = nullptr;
    if (value != nullptr && (value->isMacro() || value->isMacroFile()))
    {
        callable = value->getAs<types::Callable>();
    }
    return callable == nullptr || callable->getName().empty()
        ? fallbackName : scilab::UTF8::toUTF8(callable->getName());
}

static int lexical_active_parameter(const std::string& line, size_t openingParenthesis, size_t cursor)
{
    int parameter = 0;
    int nestingDepth = 0;
    char quote = 0;
    size_t end = std::min(cursor, line.size());
    for (size_t column = openingParenthesis + 1; column < end; ++column)
    {
        char current = line[column];
        if (quote != 0)
        {
            if (current == quote)
            {
                if (column + 1 < end && line[column + 1] == quote)
                {
                    ++column;
                }
                else
                {
                    quote = 0;
                }
            }
            continue;
        }

        if (current == '"' || (current == '\'' && is_single_quote_string_start(line, column)))
        {
            quote = current;
        }
        else if (current == '/' && column + 1 < end && line[column + 1] == '/')
        {
            break;
        }
        else if (current == '(' || current == '[' || current == '{')
        {
            ++nestingDepth;
        }
        else if (current == ')')
        {
            if (nestingDepth == 0)
            {
                return -1;
            }
            --nestingDepth;
        }
        else if ((current == ']' || current == '}') && nestingDepth > 0)
        {
            --nestingDepth;
        }
        else if (current == ',' && nestingDepth == 0)
        {
            ++parameter;
        }
    }
    return parameter;
}

static const char* parameter_kind(DocumentArgumentRole role)
{
    switch (role)
    {
        case DocumentArgumentRole::InputOutput:
            return "input/output argument";
        case DocumentArgumentRole::Input:
            return "input argument";
        case DocumentArgumentRole::Output:
            return "output argument";
        default:
            return "parameter";
    }
}

static std::string completion_source_name(const std::string& uri)
{
    std::string decoded;
    if (url_decode(uri, decoded) != 0)
    {
        decoded = uri;
    }
    std::string name = std::filesystem::path(decoded).filename().string();
    return name.empty() ? uri : name;
}

static void append_completion_item(json& items, std::set<std::string>& labels,
    const std::string& label, const char* type, int kind, const std::string& detail)
{
    if (label.empty() || !labels.insert(label).second)
    {
        return;
    }
    items.push_back({
        { "label", label },
        { "type", type },
        { "kind", kind },
        { "detail", detail }
    });
}

static std::string to_lower(const std::string& value)
{
    std::string lowered = value;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(),
        [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
    return lowered;
}

/*
 * Monaco orders the suggestions by sortText, not by how closely they match
 * what is typed: its suggest widget sorts the provider result once, comparing
 * sortText then the label. The ranking is therefore decided here — what starts
 * with the typed text first, snippets ahead within each group — otherwise the
 * "arguments" snippet would answer "argn" before the "argn" variable itself.
 */
static void rank_completion_items(json& items, const std::string& token)
{
    const std::string prefix = to_lower(token);
    for (json& item : items)
    {
        if (!item.contains("label") || !item["label"].is_string())
        {
            continue;
        }

        const std::string label = to_lower(item["label"].get<std::string>());
        const bool starts = label.compare(0, prefix.size(), prefix) == 0;
        const bool snippet = item.value("kind", 0) == 15; // LSP CompletionItemKind.Snippet
        const char group = starts ? (snippet ? '0' : '1') : (snippet ? '2' : '3');
        item["sortText"] = std::string(1, group) + label;
    }
}

// Callables are collected before their bodies, so reverse order gives the
// innermost function first when they are nested.
static std::vector<std::string> enclosing_callable_inputs(const DocumentAnalysis& analysis,
    int line, int column)
{
    for (auto it = analysis.callables.rbegin(); it != analysis.callables.rend(); ++it)
    {
        const DocumentRange& range = it->range;
        bool afterStart = line > range.firstLine
            || (line == range.firstLine && column >= range.firstColumn);
        bool beforeEnd = line < range.lastLine
            || (line == range.lastLine && column <= range.lastColumn);
        if (afterStart && beforeEnd)
        {
            return it->inputs;
        }
    }

    return std::vector<std::string>();
}

// One line per declared input, each followed by a tabstop for its class or
// its validators.
static std::string arguments_snippet_text(const std::vector<std::string>& inputs)
{
    std::string text = "arguments\n";
    size_t tabstop = 0;
    for (const std::string& input : inputs)
    {
        text += "\t" + input + " $" + std::to_string(++tabstop) + "\n";
    }

    return text + "end";
}

static void append_snippet_completion_items(json& items, std::set<std::string>& labels,
    const std::vector<std::string>& argumentsInputs)
{
    struct Snippet
    {
        const char* label;
        const char* insertText;
        const char* documentation;
    };

    static const Snippet snippets[] = {
        { "case", "case ${1:expression} then\n\t$0", "case statement" },
        { "elseif", "elseif ${1:condition} then\n\t$0", "elseif statement" },
        { "exec", "exec(\"${0:script.sce}\")", "script file execution" },
        { "for", "for ${1:i} = ${2:array}\n\t$0\nend", "Execute statements specified number of times" },
        { "arguments", "arguments\n\t$1\nend", "Input arguments checks" },
        { "function", "function ${3:result} = ${1:name}(${2:arg})\n\t$0\nendfunction", "Declare function name, inputs, and outputs" },
        { "ifelse", "if ${1:condition} then\n\t$2\nelse\n\t$3\nend", "if else statement" },
        { "if", "if ${1:condition} then\n\t$0\nend", "if statement" },
        { "select", "select ${1:variable}\ncase ${2:expression} then\n\t$0\nend", "select ... case ... end" },
        { "while", "while ${1:condition} then\n\t$0\nend", "Repeat execution of statements while condition is true" },
        { "classdef", "classdef ${1:name}\n\tproperties\n\tend\n\tmethods\n\tend\nend", "Declare a class" }
    };

    for (const Snippet& snippet : snippets)
    {
        std::string insertText = snippet.insertText;
        if (!argumentsInputs.empty() && std::string("arguments") == snippet.label)
        {
            insertText = arguments_snippet_text(argumentsInputs);
        }

        items.push_back({
            { "label", snippet.label },
            { "detail", "Scilab snippet" },
            { "kind", 15 }, // LSP CompletionItemKind.Snippet
            { "insertText", insertText },
            { "insertTextFormat", 2 }, // LSP InsertTextFormat.Snippet
            { "documentation", snippet.documentation }
        });
        labels.insert(snippet.label);
    }
}

static void append_document_symbols(const std::vector<DocumentSymbolEntry>& symbols,
    json& items, std::set<std::string>& labels, const std::string& detail)
{
    for (const DocumentSymbolEntry& symbol : symbols)
    {
        if (symbol.role == DocumentSymbolRole::Function
                || symbol.role == DocumentSymbolRole::Method
                || symbol.role == DocumentSymbolRole::Constructor
                || symbol.role == DocumentSymbolRole::Class)
        {
            // Functions, Scilab macros, methods, and constructors intentionally
            // share the same completion presentation.
            append_completion_item(items, labels, symbol.name, "function", 3, detail);
        }
        append_document_symbols(symbol.children, items, labels, detail);
    }
}

static void append_current_document_identifiers(const DocumentAnalysis& analysis,
    json& items, std::set<std::string>& labels)
{
    append_document_symbols(analysis.symbols, items, labels, "Current document");
    for (const DocumentIdentifierEntry& identifier : analysis.identifiers)
    {
        // Unresolved identifiers keep Variable as their default AST role.
        // Do not expose that fallback as semantic information: the lexical
        // document-word completion handles them later without shadowing a
        // macro declared in another open document.
        if (!identifier.hasRole || identifier.role == SyntaxRole::Property)
        {
            continue;
        }
        bool callable = identifier.role == SyntaxRole::Callable
            || identifier.role == SyntaxRole::Macro;
        append_completion_item(items, labels, identifier.name,
            callable ? "function" : "variable", callable ? 3 : 6,
            "Current document");
    }
}

static bool is_completion_identifier_start(char value)
{
    unsigned char character = static_cast<unsigned char>(value);
    return std::isalpha(character) || character >= 0x80
        || value == '_' || value == '%' || value == '$'
        || value == '!' || value == '#' || value == '?';
}

static bool is_completion_identifier_part(char value)
{
    unsigned char character = static_cast<unsigned char>(value);
    return std::isalnum(character) || character >= 0x80
        || value == '_' || value == '%' || value == '$'
        || value == '!' || value == '#' || value == '?';
}

static bool is_completion_field_identifier(const std::string& line, size_t column)
{
    while (column > 0)
    {
        unsigned char character = static_cast<unsigned char>(line[--column]);
        if (!std::isspace(character))
        {
            return line[column] == '.';
        }
    }
    return false;
}

static void append_current_document_words(const std::vector<std::string>& document,
    json& items, std::set<std::string>& labels)
{
    bool inBlockComment = false;
    for (const std::string& line : document)
    {
        char quote = 0;
        for (size_t column = 0; column < line.size();)
        {
            char current = line[column];
            if (inBlockComment)
            {
                if (current == '*' && column + 1 < line.size() && line[column + 1] == '/')
                {
                    inBlockComment = false;
                    column += 2;
                }
                else
                {
                    ++column;
                }
                continue;
            }

            if (quote != 0)
            {
                if (current == quote)
                {
                    if (column + 1 < line.size() && line[column + 1] == quote)
                    {
                        column += 2;
                        continue;
                    }
                    quote = 0;
                }
                ++column;
                continue;
            }

            if (current == '/' && column + 1 < line.size())
            {
                if (line[column + 1] == '/')
                {
                    break;
                }
                if (line[column + 1] == '*')
                {
                    inBlockComment = true;
                    column += 2;
                    continue;
                }
            }

            if (current == '"' || (current == '\'' && is_single_quote_string_start(line, column)))
            {
                quote = current;
                ++column;
                continue;
            }

            if (!is_completion_identifier_start(current))
            {
                ++column;
                continue;
            }

            size_t start = column++;
            while (column < line.size() && is_completion_identifier_part(line[column]))
            {
                ++column;
            }

            if (!is_completion_field_identifier(line, start))
            {
                append_completion_item(items, labels, line.substr(start, column - start),
                    "variable", 6, "Current document (text)");
            }
        }
    }
}

void LspEndpoint::handleCompletion(const json& json_msg)
{
    json resp = response(json_msg);

    std::string filename = json_msg["params"]["textDocument"]["uri"];
    int line = json_msg["params"]["position"]["line"];
    int iColumn = json_msg["params"]["position"]["character"];

    std::string token;
    std::string exp;
    const std::vector<std::string>* document = findDocument(filename);
    if (document == nullptr || line < 0 || static_cast<size_t>(line) >= document->size())
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }
    std::string text(document->at(line));
    switch(completion_info(text.substr(0, iColumn), token, exp))
    {
        case DICT:
        {
            json items = getDictCompletion("", &runtimeSnapshot());
            std::set<std::string> labels;
            for (const json& item : items)
            {
                if (item.contains("label") && item["label"].is_string())
                {
                    labels.insert(item["label"].get<std::string>());
                }
            }

            const DocumentAnalysisCacheEntry* current = findDocumentAnalysis(filename);
            if (current != nullptr)
            {
                append_current_document_identifiers(current->analysis, items, labels);
            }

            std::vector<std::string> openedUris = visibleDocumentUris();
            std::sort(openedUris.begin(), openedUris.end());
            for (const std::string& openedUri : openedUris)
            {
                if (openedUri == filename)
                {
                    continue;
                }
                const DocumentAnalysisCacheEntry* opened = findDocumentAnalysis(openedUri);
                if (opened != nullptr)
                {
                    append_document_symbols(opened->analysis.symbols, items, labels,
                        "Defined in " + completion_source_name(openedUri));
                }
            }

            // Plain document words are a lexical fallback. Add them after all
            // semantic symbols so accepting a macro once does not make its new
            // occurrence shadow the macro on the next completion request.
            append_snippet_completion_items(items, labels, current == nullptr
                ? std::vector<std::string>()
                : enclosing_callable_inputs(current->analysis, line, iColumn));
            append_current_document_words(*document, items, labels);
            rank_completion_items(items, token);
            resp["result"]["items"] = std::move(items);
            break;
        }
        case FIELD_STRING:
        {
            json result = getFieldCompletion(token, exp);
            if(result.is_array())
            {
                resp["result"]["items"] = result;
                break;
            }

            // goto STRING
        }
        case STRING:
        {
            resp["result"]["items"] = getFileCompletion(token);
            break;
        }
        case FIELD:
        {
            json result = getFieldCompletion(token, exp);
            if(result.is_array())
            {
                resp["result"]["items"] = result;
                break;
            }

            // goto default
        }
        default:
        {
            resp["result"] = nullptr;
        }
    }

    sendMessage(resp.dump());
}

void LspEndpoint::handleCompletionResolve(const json& json_msg)
{
    json resp = response(json_msg);
    if (json_msg["params"].value("kind", 0) == 15)
    {
        // Snippets already carry their documentation. Resolving their label as
        // a Scilab symbol could replace it with unrelated function help.
        resp["result"] = json_msg["params"];
        sendMessage(resp.dump());
        return;
    }

    std::string label = json_msg["params"]["label"];
    // Completion items are resolved lazily to keep the initial list fast.
    // LSP requires the original item to be returned with the new information.
    resp["result"] = json_msg["params"];
    resp["result"]["documentation"] = {
        { "kind", "markdown" }, // or plaintext
        { "value", getDoc(label, getWebtoolsEtcFilePath("help_md.xsl")) }
    };
    sendMessage(resp.dump());
}

void LspEndpoint::handleSignatureHelp(const json& json_msg)
{
    json resp = response(json_msg);
    std::string uri = json_msg["params"]["textDocument"]["uri"];
    size_t line_pos = json_msg["params"]["position"]["line"];
    size_t char_pos = json_msg["params"]["position"]["character"];

    const std::vector<std::string>* found = findDocument(uri);
    if(found == nullptr)
    {
        // file not found
        resp["error"] = {
            { "code", InvalidParams },
            { "message", "File not found" }
        };
        sendMessage(resp.dump());
        return;
    }
    if (line_pos >= found->size())
    {
        resp["result"]["signatures"] = json::array();
        return sendMessage(resp.dump());
    }

    int activeSignature = -1;
    int activeParameter = -1;
    std::string word;
    std::string callableExpression;
    bool runtimeSafe = false;
    bool resolvedAsVariable = false;

    const DocumentAnalysisCacheEntry* cached = findDocumentAnalysis(uri);
    if (cached != nullptr)
    {
        const DocumentAnalysis& analysis = cached->analysis;
        auto positionBeforeOrEqual = [](size_t line, size_t column, size_t otherLine, size_t otherColumn) {
            return line < otherLine || (line == otherLine && column <= otherColumn);
        };
        // DocumentRange comes from the AST and is signed: clamp it once, then stay unsigned.
        auto toOffset = [](int value) {
            return static_cast<size_t>(std::max(0, value));
        };
        for (const DocumentCallEntry& call : analysis.calls)
        {
            const size_t callFirstLine = toOffset(call.range.firstLine);
            const size_t callFirstColumn = toOffset(call.range.firstColumn);
            bool afterStart = positionBeforeOrEqual(callFirstLine, callFirstColumn,
                line_pos, char_pos);
            bool beforeEnd = positionBeforeOrEqual(line_pos, char_pos,
                toOffset(call.range.lastLine), toOffset(call.range.lastColumn));
            if (!afterStart || !beforeEnd)
            {
                continue;
            }

            std::string callPrefix;
            for (size_t line = callFirstLine;
                    line <= line_pos && line < found->size(); ++line)
            {
                size_t begin = line == callFirstLine ? callFirstColumn : 0;
                size_t end = line == line_pos
                    ? std::min(char_pos, found->at(line).size()) : found->at(line).size();
                if (!callPrefix.empty())
                {
                    callPrefix += '\n';
                }
                if (begin < end)
                {
                    callPrefix.append(found->at(line), begin, end - begin);
                }
            }
            size_t openingParenthesis = callPrefix.find('(');
            if (openingParenthesis == std::string::npos)
            {
                continue;
            }

            int candidateParameter = lexical_active_parameter(callPrefix,
                openingParenthesis, callPrefix.size());
            if (candidateParameter < 0)
            {
                continue;
            }

            // Calls are collected bottom-up, so the first call whose
            // main parenthesis is still open is the active inner call.
            word = call.callable;
            callableExpression = call.callable;
            runtimeSafe = call.runtimeSafe;
            resolvedAsVariable = call.resolvedAsVariable;
            size_t fieldSeparator = word.rfind('.');
            if (fieldSeparator != std::string::npos)
            {
                word.erase(0, fieldSeparator + 1);
            }
            activeParameter = candidateParameter;
            break;
        }
    }
    bool hasContext = json_msg["params"].contains("context");
    if (word.empty())
    {
        std::string line = found->at(line_pos);
        if(char_pos < line.size())
        {
            line.erase(char_pos, line.size());
        }

        // Ensure closing the signature help after the last ")".
        size_t paren_pos = line.rfind('(');
        int lexicalParameter = -1;
        while (paren_pos != std::string::npos)
        {
            lexicalParameter = lexical_active_parameter(line, paren_pos, line.size());
            if (lexicalParameter >= 0)
            {
                break;
            }
            paren_pos = paren_pos == 0 ? std::string::npos : line.rfind('(', paren_pos - 1);
        }

        if (paren_pos == std::string::npos || paren_pos < 2 || lexicalParameter < 0)
        {
            resp["result"]["signatures"] = json::array();
            return sendMessage(resp.dump());
        }

        word = getWord(line, paren_pos - 2);
        callableExpression = word;
        if (hasContext && json_msg["params"]["context"]["isRetrigger"])
        {
            activeParameter = lexicalParameter;
        }
    }

    json signatures = json::array();
    bool fieldCall = callableExpression.find('.') != std::string::npos;
    if (!resolvedAsVariable && !fieldCall && cached != nullptr)
    {
        signatures = document_callable_signatures(cached->analysis, word);
    }
    if (!resolvedAsVariable && !fieldCall && signatures.empty())
    {
        std::vector<std::string> openedUris;
        for (const std::string& openedUri : visibleDocumentUris())
        {
            if (openedUri != uri)
            {
                openedUris.push_back(openedUri);
            }
        }
        std::sort(openedUris.begin(), openedUris.end());
        for (const std::string& openedUri : openedUris)
        {
            const DocumentAnalysisCacheEntry* opened = findDocumentAnalysis(openedUri);
            if (opened == nullptr)
            {
                continue;
            }
            signatures = document_callable_signatures(opened->analysis, word);
            if (!signatures.empty())
            {
                break;
            }
        }
    }
    if (!resolvedAsVariable && signatures.empty())
    {
        RuntimeTryLockGuard runtimeGuard;
        types::InternalType* value = nullptr;
        if (runtimeGuard.ownsLock() && runtimeSafe
                && callableExpression.find('.') != std::string::npos)
        {
            if (!eval(callableExpression, &value).empty())
            {
                value = nullptr;
            }
        }
        else if (runtimeGuard.ownsLock() && callableExpression == word)
        {
            symbol::Context* context = symbol::Context::getInstance();
            value = context->get(symbol::Symbol(scilab::UTF8::toWide(word)));
        }
        if (value != nullptr && (value->isMacro() || value->isMacroFile()))
        {
            signatures = getSignatures(runtime_macro_name(value, word), activeParameter,
                getWebtoolsEtcFilePath("help_proto.xsl"));
        }
        if (signatures.empty())
        {
            signatures = runtime_macro_signatures(value, word);
        }
        if (!runtimeGuard.ownsLock() && callableExpression == word)
        {
            const RuntimeSnapshot& snapshot = runtimeSnapshot();
            const RuntimeSymbolSnapshot* symbol = snapshot.find(word);
            const RuntimeMacroSnapshot* macro = symbol == nullptr
                ? nullptr : snapshot.macro(*symbol);
            if (macro != nullptr && !macro->signature.empty())
            {
                signatures = json::array({{{"label", macro->signature}}});
            }
        }
    }
    if (!resolvedAsVariable && signatures.empty())
    {
        signatures = getSignatures(word, activeParameter,
            getWebtoolsEtcFilePath("help_proto.xsl"));
    }
    resp["result"]["signatures"] = signatures;
    if (activeParameter >= 0)
    {
        resp["result"]["activeParameter"] = activeParameter;
    }

    if(hasContext)
    {
        json context = json_msg["params"]["context"];
        if(context["isRetrigger"] && context.contains("activeSignatureHelp"))
        {
            json activeSignatureHelp = context["activeSignatureHelp"];
            if(activeSignatureHelp.contains("activeSignature"))
            {
                activeSignature = activeSignatureHelp["activeSignature"];
                std::string currentSignature = context["activeSignatureHelp"]["signatures"][activeSignature]["label"];
                int iter = 0;
                activeSignature = 0;
                for(const auto& sig : resp["result"]["signatures"])
                {
                    if(sig["label"] == currentSignature)
                    {
                        activeSignature = iter;
                        break;
                    }

                    iter++;
                }

                resp["result"]["activeSignature"] = activeSignature;
                resp["result"]["activeParameter"] = activeParameter;
            }
        }
    }

    sendMessage(resp.dump());
    return;
}

void LspEndpoint::handleHover(const json& json_msg)
{
    json resp = response(json_msg);

    std::string uri = json_msg["params"]["textDocument"]["uri"];
    size_t line_pos = json_msg["params"]["position"]["line"];
    size_t char_pos = json_msg["params"]["position"]["character"];

    const std::vector<std::string>* found = findDocument(uri);
    if(found == nullptr)
    {
        // file not found
        resp["error"] = {
            { "code", InvalidParams },
            { "message", "File not found" }
        };
        sendMessage(resp.dump());
        return;
    }
    if (line_pos >= found->size())
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    DocumentIdentifierEntry selectedTarget;
    const DocumentIdentifierEntry* target = nullptr;
    DocumentAnalysis analysis;
    const DocumentAnalysisCacheEntry* cached = findDocumentAnalysis(uri);
    if (cached != nullptr)
    {
        analysis = cached->analysis;
        for (const DocumentIdentifierEntry& identifier : analysis.identifiers)
        {
            if (identifier.range.firstLine == static_cast<int>(line_pos)
                    && static_cast<int>(char_pos) >= identifier.range.firstColumn
                    && static_cast<int>(char_pos) < identifier.range.lastColumn)
            {
                selectedTarget = identifier;
                target = &selectedTarget;
                break;
            }
        }
    }
    std::string word = target == nullptr
        ? getWord(found->at(line_pos), char_pos) : target->name;
    if(word.empty())
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    std::string doc = "";
    std::string kind = "plaintext";
    const DocumentSymbolEntry* sourceSymbol = nullptr;
    const std::vector<std::string>* sourceLines = nullptr;
    std::string sourceDescription;
    if (target != nullptr)
    {
        resp["result"]["range"] = document_range(target->range);
        if (target->local)
        {
            const char* localKind = target->role == SyntaxRole::Parameter
                ? parameter_kind(target->argumentRole) : "local variable";
            resp["result"]["contents"] = {
                { "kind", "markdown" },
                { "value", "`" + word + "` - " + localKind }
            };
            sendMessage(resp.dump());
            return;
        }
        if (target->documentSymbol)
        {
            sourceSymbol = find_document_symbol(analysis.symbols, word);
            sourceLines = found;
            sourceDescription = "this document";
        }
    }

    std::vector<std::string> openedUris;
    for (const std::string& openedUri : visibleDocumentUris())
    {
        if (openedUri != uri)
        {
            openedUris.push_back(openedUri);
        }
    }
    std::sort(openedUris.begin(), openedUris.end());

    for (const std::string& openedUri : openedUris)
    {
        if (sourceSymbol != nullptr)
        {
            break;
        }

        const DocumentAnalysisCacheEntry* openedCached = findDocumentAnalysis(openedUri);
        const std::vector<std::string>* candidateLines = findDocument(openedUri);
        if (openedCached == nullptr || candidateLines == nullptr)
        {
            continue;
        }

        const DocumentSymbolEntry* candidateSymbol = find_document_symbol(
            openedCached->analysis.symbols, word);
        if (candidateSymbol == nullptr)
        {
            continue;
        }

        std::string decodedUri;
        if (url_decode(openedUri, decodedUri) != 0)
        {
            decodedUri = openedUri;
        }
        sourceSymbol = candidateSymbol;
        sourceLines = candidateLines;
        sourceDescription = std::filesystem::path(decodedUri).filename().string();
        break;
    }

    RuntimeTryLockGuard runtimeGuard;
    symbol::Context* pCtx = runtimeGuard.ownsLock()
        ? symbol::Context::getInstance() : nullptr;
    types::InternalType* pIT = nullptr;
    if (pCtx != nullptr && target != nullptr && target->runtimeSafe
            && !target->runtimeExpression.empty())
    {
        if (!eval(target->runtimeExpression, &pIT).empty())
        {
            pIT = nullptr;
        }
    }
    if (pCtx != nullptr && pIT == nullptr
            && (target == nullptr || target->runtimeExpression == word))
    {
        pIT = pCtx->get(symbol::Symbol(scilab::UTF8::toWide(word)));
    }
    if (pIT == nullptr)
    {
        if (sourceSymbol != nullptr)
        {
            doc = document_macro_hover(word, sourceSymbol, *sourceLines,
                sourceDescription);
            kind = "markdown";
        }
        else
        {
            doc = getDoc(word, getWebtoolsEtcFilePath("help_md.xsl"));
            if (doc.empty() && !runtimeGuard.ownsLock())
            {
                const RuntimeSnapshot& snapshot = runtimeSnapshot();
                const RuntimeSymbolSnapshot* symbol = snapshot.find(word);
                const RuntimeMacroSnapshot* macro = symbol == nullptr
                    ? nullptr : snapshot.macro(*symbol);
                if (macro != nullptr && !macro->signature.empty())
                {
                    doc = "```scilab\n" + hover_prototype(macro->signature) + "\n```";
                }
            }
        }
        if (doc.empty())
        {
            resp["result"] = nullptr;
            sendMessage(resp.dump());
            return;
        }

        kind = "markdown";
        resp["result"]["contents"] = {
            { "kind", kind },
            { "value", doc }
        };
        sendMessage(resp.dump());
        return;
    }

    switch(pIT->getType())
    {
        case types::InternalType::ScilabFunction:
        case types::InternalType::ScilabMacro:
        case types::InternalType::ScilabMacroFile:
        {
            kind = "markdown";
            std::string documentationWord = word;
            bool runtimeMacro = pIT->isMacro() || pIT->isMacroFile();
            if (runtimeMacro)
            {
                types::Callable* callable = pIT->getAs<types::Callable>();
                if (callable != nullptr && !callable->getName().empty())
                {
                    documentationWord = scilab::UTF8::toUTF8(callable->getName());
                }
            }
            // An unexecuted source declaration may shadow a documented
            // primitive with the same name. In that case the AST source is
            // authoritative and the unrelated runtime help must not leak in.
            if (sourceSymbol == nullptr || runtimeMacro)
            {
                doc = getDoc(documentationWord, getWebtoolsEtcFilePath("help_md.xsl"));
            }
            if(doc.empty())
            {
                if (sourceSymbol != nullptr)
                {
                    doc = document_macro_hover(word, sourceSymbol, *sourceLines,
                        sourceDescription);
                }
                else
                {
                    std::string displayName = target != nullptr && !target->runtimeExpression.empty()
                        ? target->runtimeExpression : word;
                    std::string signature = runtime_macro_signature(pIT, displayName);
                    if (!signature.empty())
                    {
                        doc = "```scilab\n" + hover_prototype(signature) + "\n```";
                    }
                    else
                    {
                        kind = "plaintext";
                        std::wostringstream os;
                        pIT->toString(os);
                        pIT->clearPrintState();
                        doc = scilab::UTF8::toUTF8(os.str());
                        size_t equals = doc.find("=");
                        if (equals != std::string::npos)
                        {
                            doc.insert(equals + 1, word);
                        }
                    }
                }
            }
            else if (sourceSymbol != nullptr && runtimeMacro)
            {
                doc += "\n\nMacro defined in " + sourceDescription + ".";
            }
        }
        break;
        default:
        {
            resp["result"] = nullptr;
            sendMessage(resp.dump());
            return;
        }
    }

    resp["result"]["contents"] = {
        { "kind", kind }, // markdown or plaintext
        { "value", doc }
    };

    sendMessage(resp.dump());
}
