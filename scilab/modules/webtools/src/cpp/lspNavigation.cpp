/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Implements source navigation features backed by the AST and Scilab
// runtime: document symbols, folding ranges, and definition lookup.

#include <algorithm>
#include <filesystem>
#ifndef _MSC_VER
#include <curl/curl.h>
#endif

#include "lspEndpoint.hxx"
#include "lspProtocolUtils.hxx"
#include "lsp_dap_tools.hxx"
#include "documentAnalysisVisitor.hxx"
#include "UTF8.hxx"
#include "internal.hxx"
#include "macrofile.hxx"
#include "visitor_common.hxx"

extern "C"
{
    #include "FileExist.h"
}

#ifdef _MSC_VER
#include <windows.h>
#include <shlwapi.h>
#endif

static bool same_file(const std::string& left, const std::string& right)
{
    std::error_code error;
    bool equivalent = std::filesystem::equivalent(left, right, error);
    return error ? left == right : equivalent;
}

static bool is_scinotes_temporary_file(const std::string& filename)
{
    return std::filesystem::path(filename).filename().string().rfind("LOAD_INTO_SCILAB-", 0) == 0;
}

static std::string macro_source_file(std::string filename)
{
    if(filename.size() >= 4 && filename.rfind(".bin") == filename.size() - 4)
    {
        filename.replace(filename.size() - 4, 4, ".sci");
    }

    return filename;
}
static json document_symbol(const DocumentSymbolEntry& symbol)
{
    int kind = 13;
    switch (symbol.role)
    {
        case DocumentSymbolRole::Function: kind = 12; break;
        case DocumentSymbolRole::Class: kind = 5; break;
        case DocumentSymbolRole::Method: kind = 6; break;
        case DocumentSymbolRole::Constructor: kind = 9; break;
        case DocumentSymbolRole::Property: kind = 7; break;
        case DocumentSymbolRole::Enumeration: kind = 10; break;
    }

    json result = {
        { "kind", kind },
        { "name", symbol.name },
        { "range", document_range(symbol.range) },
        { "selectionRange", document_range(symbol.selectionRange) }
    };
    if (!symbol.children.empty())
    {
        result["children"] = json::array();
        for (const DocumentSymbolEntry& child : symbol.children)
        {
            result["children"].push_back(document_symbol(child));
        }
    }
    return result;
}

static void collect_document_definitions(const std::vector<DocumentSymbolEntry>& symbols,
    const std::string& name, const std::string& uri, json& locations)
{
    for (const DocumentSymbolEntry& symbol : symbols)
    {
        collect_document_definitions(symbol.children, name, uri, locations);
        if (symbol.name == name)
        {
            locations.push_back({
                { "uri", uri },
                { "range", document_range(symbol.selectionRange) }
            });
        }
    }
}

// A macro declared inside another one is local to it: it is a callable of the
// analysis but never a document symbol. Only the document holding it can
// navigate to it.
static void collect_local_macro_definitions(const std::vector<DocumentCallableEntry>& callables,
    const std::string& name, const std::string& uri, json& locations)
{
    for (const DocumentCallableEntry& callable : callables)
    {
        if (callable.name == name)
        {
            locations.push_back({
                { "uri", uri },
                { "range", document_range(callable.selectionRange) }
            });
        }
    }
}

static std::string uri_to_local(const std::string& uri)
{
#ifdef _MSC_VER
    std::string filename = "";
    char path[MAX_PATH];
    DWORD cch = ARRAYSIZE(path);
    HRESULT hr = PathCreateFromUrlA(uri.data(), path, &cch, NULL);
    if (SUCCEEDED(hr))
    {
        filename = path + 1; // +1: remove "\" at the beginin
    }
#else
    std::string tmp = uri;
    tmp.erase(0, 7); // remove "file://"
    char* o = curl_easy_unescape(NULL, tmp.data(), tmp.size(), NULL);
    std::string filename(o);
    curl_free(o);
#endif

    return filename;
}

static std::string local_to_uri(const std::string& filename)
{
#ifdef _MSC_VER
    std::string target;
    char path[MAX_PATH];
    DWORD cch = ARRAYSIZE(path);
    HRESULT hr = UrlCreateFromPathA(filename.data(), path, &cch, NULL);
    if (SUCCEEDED(hr))
    {
        target = path;
    }
    return target;
#else
    return "file://" + filename;
#endif
}

static json runtime_snapshot_definition(const RuntimeSnapshot& snapshot,
    const RuntimeSymbolSnapshot* symbol)
{
    const RuntimeMacroSnapshot* macro = symbol == nullptr
        ? nullptr : snapshot.macro(*symbol);
    if (symbol == nullptr || symbol->kind != RuntimeSymbolKind::Macro
            || macro == nullptr || macro->sourcePath.empty()
            || is_scinotes_temporary_file(macro->sourcePath)
            || (macro->sourcePath.size() >= 4
                && macro->sourcePath.rfind(".sci") == macro->sourcePath.size() - 4
                && !FileExist(macro->sourcePath.data())))
    {
        return nullptr;
    }

    std::string target = local_to_uri(macro->sourcePath);
    if (target.empty())
    {
        return nullptr;
    }
    json range = {
        {"start", {
            {"line", macro->sourceStartLine},
            {"character", macro->sourceStartColumn}
        }},
        {"end", {
            {"line", macro->sourceEndLine},
            {"character", macro->sourceEndColumn}
        }}
    };
    return {
        {"targetUri", target},
        {"targetRange", range},
        {"targetSelectionRange", range}
    };
}

void LspEndpoint::handleDocumentSymbol(const json& json_msg)
{
    json resp = response(json_msg);
    std::string uri = json_msg["params"]["textDocument"]["uri"];

    const DocumentAnalysisCacheEntry* cached = findDocumentAnalysis(uri);
    if (cached == nullptr)
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    json symbols = json::array();
    for (const DocumentSymbolEntry& symbol : cached->analysis.symbols)
    {
        symbols.push_back(document_symbol(symbol));
    }
    resp["result"] = symbols;
    sendMessage(resp.dump());
}

void LspEndpoint::handleFoldingRange(const json& json_msg)
{
    json resp = response(json_msg);
    std::string uri = json_msg["params"]["textDocument"]["uri"];
    const std::vector<std::string>* found = findDocument(uri);
    if (found == nullptr)
    {
        resp["error"] = {
            { "code", InvalidParams },
            { "message", "File not found" }
        };
        sendMessage(resp.dump());
        return;
    }

    const DocumentAnalysisCacheEntry* cached = findDocumentAnalysis(uri);
    if (cached == nullptr)
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    json ranges = json::array();
    for (const DocumentRegionEntry& region : cached->analysis.regions)
    {
        // Standard clients ignore this extension; SciNotes uses it to derive
        // local line numbers only from AST function regions.
        ranges.push_back({
            { "startLine", region.range.firstLine },
            { "startCharacter", region.range.firstColumn },
            { "endLine", region.range.lastLine },
            { "endCharacter", region.range.lastColumn },
            { "scilabRole", region.role == DocumentRegionRole::Function ? "function" : "block" }
        });
    }
    resp["result"] = ranges;
    sendMessage(resp.dump());
}

void LspEndpoint::handleDefinition(const json& json_msg)
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

    std::string word = getWord(found->at(line_pos), char_pos);
    if(word.empty())
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    // Prefer declarations from the AST. This works before the
    // document has been executed and covers macros, constructors, and
    // class methods in the current source file.
    json locations = json::array();
    std::string runtimeFieldExpression;
    const DocumentAnalysisCacheEntry* cached = findDocumentAnalysis(uri);
    if (cached != nullptr)
    {
        const DocumentAnalysis& analysis = cached->analysis;

        // When the AST resolves the selected name, only a macro role
        // may navigate to a macro declaration. A field is the sole
        // exception: its value may itself be a macro in the current
        // Scilab context.
        for (const SyntaxTokenHint& token : analysis.syntaxTokens)
        {
            if (token.line == static_cast<int>(line_pos)
                    && token.start <= static_cast<int>(char_pos)
                    && static_cast<int>(char_pos) < token.start + token.length
                    && token.role != SyntaxRole::Macro)
            {
                if (token.role == SyntaxRole::Property)
                {
                    runtimeFieldExpression = token.runtimeSafe ? token.runtimeExpression : std::string();
                }

                if (runtimeFieldExpression.empty())
                {
                    resp["result"] = nullptr;
                    sendMessage(resp.dump());
                    return;
                }
            }
        }

        if (runtimeFieldExpression.empty())
        {
            collect_document_definitions(analysis.symbols, word, uri, locations);
            if (locations.empty())
            {
                collect_local_macro_definitions(analysis.callables, word, uri, locations);
            }
        }
    }

    // Search the other open documents before falling back to the Scilab
    // runtime context. This enables navigation without executing them.
    std::vector<std::string> openedUris;
    const std::vector<std::string> visibleUris = visibleDocumentUris();
    openedUris.reserve(visibleUris.size());
    for (const std::string& openedUri : visibleUris)
    {
        if (openedUri != uri)
        {
            openedUris.push_back(openedUri);
        }
    }
    std::sort(openedUris.begin(), openedUris.end());

    for (const std::string& openedUri : openedUris)
    {
        if (!runtimeFieldExpression.empty())
        {
            break;
        }

        const DocumentAnalysisCacheEntry* openedAnalysis = findDocumentAnalysis(openedUri);
        if (openedAnalysis != nullptr)
        {
            collect_document_definitions(openedAnalysis->analysis.symbols,
                word, openedUri, locations);
        }
    }

    if (!locations.empty())
    {
        resp["result"] = locations.size() == 1 ? locations[0] : locations;
        sendMessage(resp.dump());
        return;
    }

    RuntimeTryLockGuard runtimeGuard;
    if (!runtimeGuard.ownsLock())
    {
        const RuntimeSnapshot& snapshot = runtimeSnapshot();
        resp["result"] = runtime_snapshot_definition(snapshot, snapshot.find(word));
        sendMessage(resp.dump());
        return;
    }

    symbol::Context* pCtx = symbol::Context::getInstance();
    std::wstring wword = scilab::UTF8::toWide(word);
    symbol::Symbol w(wword);
    types::InternalType* pIT = nullptr;
    if (runtimeFieldExpression.empty())
    {
        pIT = pCtx->get(w);
    }
    else
    {
        if (!eval(runtimeFieldExpression, &pIT).empty())
        {
            pIT = nullptr;
        }
    }

    // look for submacro
    if (pIT == nullptr && runtimeFieldExpression.empty())
    {
        std::string local_uri = uri_to_local(uri);
        std::list<std::wstring> macrolist;
        pCtx->getMacrosName(macrolist);
        for (auto it : macrolist)
        {
            symbol::Symbol w(it);
            types::InternalType* pITCurrentMacro = pCtx->get(w);
            if (pITCurrentMacro == nullptr)
            {
                continue;
            }

            types::Macro* pCurrentMacro = nullptr;
            if(pITCurrentMacro->isMacroFile())
            {
                types::MacroFile* pMF = pITCurrentMacro->getAs<types::MacroFile>();
                if(pMF == nullptr)
                {
                    continue;
                }

                std::string filename = macro_source_file(scilab::UTF8::toUTF8(pMF->getPath()));
                if(same_file(filename, local_uri) == false)
                {
                    continue;
                }

                try
                {
                    pCurrentMacro = pMF->getMacro();
                }
                catch(...)
                {
                    continue;
                }
            }
            else if(pITCurrentMacro->isMacro())
            {
                pCurrentMacro = pITCurrentMacro->getAs<types::Macro>();
                if(pCurrentMacro == nullptr)
                {
                    continue;
                }

                std::string filename = macro_source_file(scilab::UTF8::toUTF8(pCurrentMacro->getFileName()));
                if(same_file(filename, local_uri) == false)
                {
                    continue;
                }
            }
            else
            {
                continue;
            }

            // check for sub macros
            if(pCurrentMacro == nullptr)
            {
                continue;
            }

            for(const auto& sub : pCurrentMacro->getSubMacros())
            {
                if(sub.first->getSymbol().getName() == wword)
                {
                    pIT = sub.second;
                    break;
                }
            }

            break;
        }
    }

    if (pIT == nullptr)
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    types::Macro* pM = nullptr;
    switch(pIT->getType())
    {
        case types::InternalType::ScilabMacroFile:
        {
            types::MacroFile* pMF = pIT->getAs<types::MacroFile>();
            if(pMF != nullptr)
            {
                try
                {
                    pM = pMF->getMacro();
                }
                catch(...)
                {
                    pM = nullptr;
                }
            }
            break;
        }
        case types::InternalType::ScilabMacro:
        {
            pM = pIT->getAs<types::Macro>();
            break;
        }
        default:
        {
            resp["result"] = nullptr;
            sendMessage(resp.dump());
            return;
        }
    }

    if(pM == nullptr)
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    std::string filename = macro_source_file(scilab::UTF8::toUTF8(pM->getFileName()));
    if (is_scinotes_temporary_file(filename))
    {
        json editorLocations = json::array();
        std::string macroName = scilab::UTF8::toUTF8(pM->getName());
        for (const std::string& openedUri : visibleDocumentUris())
        {
            const DocumentAnalysisCacheEntry* openedAnalysis = findDocumentAnalysis(openedUri);
            if (openedAnalysis != nullptr)
            {
                collect_document_definitions(openedAnalysis->analysis.symbols, macroName,
                    openedUri, editorLocations);
            }
        }

        if (!editorLocations.empty())
        {
            resp["result"] = editorLocations.size() == 1 ? editorLocations[0] : editorLocations;
            sendMessage(resp.dump());
            return;
        }
    }

    if (filename.size() >= 4 && filename.rfind(".sci") == filename.size() - 4 && !FileExist(filename.data()))
    {
        resp["result"] = nullptr;
        sendMessage(resp.dump());
        return;
    }

    std::string target = local_to_uri(filename);

    json bodyLocation = convertLocation(pM->getBody()->getLocation());
    resp["result"] = {
        { "targetUri", target },
        { "targetRange", bodyLocation },
        { "targetSelectionRange", bodyLocation }
    };

    sendMessage(resp.dump());
    return;
}
