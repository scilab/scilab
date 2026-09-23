/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// Handles LSP document synchronization and keeps the in-memory source copy
// used by the other language features up to date.

#include <regex>
#include <set>

#include "lspEndpoint.hxx"
#include "lspDiagnostics.hxx"
#include "lsp_dap_tools.hxx"
#include "exp.hxx"

using json = nlohmann::json;

static std::vector<std::string> text_to_lines(const std::string& text)
{
    const std::regex lineBreak(".*(\r\n|\n)");
    auto begin = std::sregex_iterator(text.begin(), text.end(), lineBreak);
    auto end = std::sregex_iterator();
    size_t lineCount = std::distance(begin, end);
    std::vector<std::string> lines;

    if (lineCount == 0)
    {
        lines.push_back(text);
        return lines;
    }

    lines.reserve(lineCount);
    std::smatch match;
    for (auto it = begin; it != end; ++it)
    {
        match = *it;
        lines.push_back(match.str().substr(0, match.str().length() - 1));
    }

    lines.push_back(match.suffix().str());
    return lines;
}

void LspEndpoint::indexDocument(const std::string& uri, const std::string& text, int version)
{
    m_service->Post([this, uri, text, version]() {
        m_indexedDocuments.files[uri] = text_to_lines(text);
        RuntimeTryLockGuard runtimeGuard;
        if (!runtimeGuard.ownsLock())
        {
            m_indexedDocuments.pendingAnalysisVersions[uri] = version;
        }
        else
        {
            m_indexedDocuments.pendingAnalysisVersions.erase(uri);
        }
        if (updateDocumentAnalysis(m_indexedDocuments, uri, version,
                runtimeGuard.ownsLock()))
        {
            refreshSemanticTokens(true);
        }
    });
}

void LspEndpoint::removeIndexedDocument(const std::string& uri)
{
    m_service->Post([this, uri]() {
        m_indexedDocuments.files.erase(uri);
        m_indexedDocuments.analyses.erase(uri);
        m_indexedDocuments.pendingAnalysisVersions.erase(uri);
        refreshSemanticTokens(true);
    });
}

static void collect_exported_macros(const std::vector<DocumentSymbolEntry>& symbols,
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
        collect_exported_macros(symbol.children, names);
    }
}

LspEndpoint::DocumentState& LspEndpoint::currentDocumentState()
{
    return s_currentDocumentState ? *s_currentDocumentState : m_tcpDocumentState;
}

const std::vector<std::string>* LspEndpoint::findDocument(const std::string& uri) const
{
    if (s_currentDocumentState)
    {
        auto sessionDocument = s_currentDocumentState->files.find(uri);
        if (sessionDocument != s_currentDocumentState->files.end())
        {
            return &sessionDocument->second;
        }
    }

    auto indexedDocument = m_indexedDocuments.files.find(uri);
    return indexedDocument == m_indexedDocuments.files.end() ? nullptr : &indexedDocument->second;
}

const LspEndpoint::DocumentAnalysisCacheEntry* LspEndpoint::findDocumentAnalysis(
    const std::string& uri) const
{
    if (s_currentDocumentState)
    {
        auto sessionAnalysis = s_currentDocumentState->analyses.find(uri);
        if (sessionAnalysis != s_currentDocumentState->analyses.end())
        {
            return &sessionAnalysis->second;
        }
    }

    auto indexedAnalysis = m_indexedDocuments.analyses.find(uri);
    return indexedAnalysis == m_indexedDocuments.analyses.end() ? nullptr : &indexedAnalysis->second;
}

std::vector<std::string> LspEndpoint::visibleDocumentUris() const
{
    std::set<std::string> uris;
    for (const auto& document : m_indexedDocuments.files)
    {
        uris.insert(document.first);
    }
    if (s_currentDocumentState)
    {
        for (const auto& document : s_currentDocumentState->files)
        {
            uris.insert(document.first);
        }
    }
    return {uris.begin(), uris.end()};
}

bool LspEndpoint::updateDocumentAnalysis(DocumentState& state, const std::string& uri,
    int version, bool useRuntimeContext)
{
    std::set<std::string> previousMacros;
    auto previous = state.analyses.find(uri);
    if (previous != state.analyses.end())
    {
        collect_exported_macros(previous->second.analysis.symbols, previousMacros);
    }

    DocumentAnalysisCacheEntry entry;
    entry.version = version;
    ast::Exp* tree = nullptr;
    if (useRuntimeContext)
    {
        parseDocumentWithRecovery(state.files.at(uri), &tree, &entry.parseError);
    }
    else
    {
        parseDocumentWithRecoveryWithoutRuntime(state.files.at(uri), &tree,
            &entry.parseError);
    }
    if (tree != nullptr)
    {
        entry.analysis = analyzeDocument(tree, state.files.at(uri));
        delete tree;
    }
    state.analyses[uri] = std::move(entry);

    std::set<std::string> currentMacros;
    collect_exported_macros(state.analyses.at(uri).analysis.symbols, currentMacros);
    return currentMacros != previousMacros;
}

bool LspEndpoint::processPendingDocumentAnalyses(DocumentState& state,
    std::vector<std::string>* diagnostics)
{
    bool exportedMacrosChanged = false;
    for (auto pending = state.pendingAnalysisVersions.begin();
            pending != state.pendingAnalysisVersions.end();)
    {
        const std::string uri = pending->first;
        const int version = pending->second;
        pending = state.pendingAnalysisVersions.erase(pending);
        if (state.files.find(uri) == state.files.end())
        {
            continue;
        }

        exportedMacrosChanged = updateDocumentAnalysis(state, uri, version)
            || exportedMacrosChanged;
        if (diagnostics != nullptr)
        {
            const DocumentAnalysisCacheEntry& cached = state.analyses.at(uri);
            diagnostics->push_back(syntax_diagnostics_notification(uri,
                state.files.at(uri), cached.analysis, cached.parseError,
                &m_runtimeSnapshot).dump());
        }
    }
    return exportedMacrosChanged;
}

void LspEndpoint::processPendingDocumentAnalyses()
{
    RuntimeTryLockGuard runtimeGuard;
    if (!runtimeGuard.ownsLock())
    {
        return;
    }

    m_runtimeSnapshot.capture();

    processPendingDocumentAnalyses(m_indexedDocuments);

    std::vector<std::string> diagnostics;
    processPendingDocumentAnalyses(m_tcpDocumentState, &diagnostics);
    for (const std::string& message : diagnostics)
    {
        sendMessage(message);
    }

    for (const std::shared_ptr<SciWsSession>& session : m_ws_sessions)
    {
        auto state = m_wsDocumentStates.find(session.get());
        if (state == m_wsDocumentStates.end())
        {
            continue;
        }

        diagnostics.clear();
        processPendingDocumentAnalyses(state->second, &diagnostics);
        for (const std::string& message : diagnostics)
        {
            session->SendTextMessage(message);
        }
    }
}

const RuntimeSnapshot& LspEndpoint::runtimeSnapshot() const
{
    return m_runtimeSnapshot;
}

void LspEndpoint::refreshSemanticTokens(bool broadcast)
{
    const std::string message = json{
        { "jsonrpc", "2.0" },
        { "id", m_sequence++ },
        { "method", "workspace/semanticTokens/refresh" },
        { "params", json::object() }
    }.dump();
    if (broadcast)
    {
        broadcastMessage(message);
    }
    else
    {
        sendMessage(message);
    }
}

void LspEndpoint::handleDidOpen(const json& json_msg)
{
    json textDoc = json_msg["params"]["textDocument"];
    if(textDoc["languageId"] == "scilab")
    {
        std::string uri = textDoc["uri"];
        DocumentState& state = currentDocumentState();
        state.files[uri] = text_to_lines(textDoc["text"]);
        const int version = textDoc.value("version", 0);
        RuntimeTryLockGuard runtimeGuard;
        if (!runtimeGuard.ownsLock())
        {
            state.pendingAnalysisVersions[uri] = version;
        }
        else
        {
            state.pendingAnalysisVersions.erase(uri);
        }
        bool exportedMacrosChanged = updateDocumentAnalysis(state, uri, version,
            runtimeGuard.ownsLock());
        const DocumentAnalysisCacheEntry& cached = state.analyses.at(uri);
        sendMessage(syntax_diagnostics_notification(uri, state.files.at(uri),
            cached.analysis, cached.parseError, &m_runtimeSnapshot).dump());
        if (exportedMacrosChanged)
        {
            refreshSemanticTokens();
        }
    }
}

void LspEndpoint::handleDidChange(const json& json_msg)
{
    std::string filename = json_msg["params"]["textDocument"]["uri"];
    std::string text = json_msg["params"]["contentChanges"][0]["text"];
    DocumentState& state = currentDocumentState();
    state.files[filename] = text_to_lines(text);
    const int version = json_msg["params"]["textDocument"].value("version", 0);
    RuntimeTryLockGuard runtimeGuard;
    if (!runtimeGuard.ownsLock())
    {
        state.pendingAnalysisVersions[filename] = version;
    }
    else
    {
        state.pendingAnalysisVersions.erase(filename);
    }
    bool exportedMacrosChanged = updateDocumentAnalysis(state, filename, version,
        runtimeGuard.ownsLock());
    const DocumentAnalysisCacheEntry& cached = state.analyses.at(filename);
    sendMessage(syntax_diagnostics_notification(filename, state.files.at(filename),
        cached.analysis, cached.parseError, &m_runtimeSnapshot).dump());
    if (exportedMacrosChanged)
    {
        refreshSemanticTokens();
    }
}

void LspEndpoint::handleDidClose(const json& json_msg)
{
    json textDoc = json_msg["params"]["textDocument"];
    std::string uri = textDoc["uri"];
    DocumentState& state = currentDocumentState();
    bool exportedMacrosChanged = false;
    auto cached = state.analyses.find(uri);
    if (cached != state.analyses.end())
    {
        std::set<std::string> macros;
        collect_exported_macros(cached->second.analysis.symbols, macros);
        exportedMacrosChanged = !macros.empty();
    }
    state.files.erase(uri);
    state.analyses.erase(uri);
    state.pendingAnalysisVersions.erase(uri);
    sendMessage(json{
        { "jsonrpc", "2.0" },
        { "method", "textDocument/publishDiagnostics" },
        { "params", {
            { "uri", uri },
            { "diagnostics", json::array() }
        }}
    }.dump());
    if (exportedMacrosChanged)
    {
        refreshSemanticTokens();
    }
}

void LspEndpoint::handleDidSave(const json& /* json_msg */)
{
}
