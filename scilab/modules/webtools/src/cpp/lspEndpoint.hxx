/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __LSP_ENDPOINT_HXX__
#define __LSP_ENDPOINT_HXX__

#include <atomic>
#include <unordered_set>
#include <unordered_map>

#include "tcpServer.hxx"
#include "wsServer.hxx"
#include "internal.hxx"
#include "nlohmann_json.hxx"
#include "documentAnalysisVisitor.hxx"
#include "runtimeaccess.hxx"
#include "runtimeSnapshot.hxx"
//#include "output/SLintScilabResult.hxx"

class LspEndpoint
{
public:
    // avoid the use of assignation of this singleton
    LspEndpoint(const LspEndpoint &) = delete;
    LspEndpoint& operator=(const LspEndpoint &) = delete;
    LspEndpoint(LspEndpoint &&) = delete;
    LspEndpoint & operator=(LspEndpoint &&) = delete;

    // thread safe singleton
    static LspEndpoint& getInstance()
    {
        static LspEndpoint endpoint;
        return endpoint;
    }

    void startTcp(int port, bool _spawned = false);
    void startWs(int port, const std::string& token = "", bool _spawned = false);
    void indexDocument(const std::string& uri, const std::string& text, int version);
    void removeIndexedDocument(const std::string& uri);

private:
    LspEndpoint();
    ~LspEndpoint();

    void onTcpConnected(std::shared_ptr<CppServer::Asio::TCPSession>& session);
    void onTcpDisconnected(std::shared_ptr<CppServer::Asio::TCPSession>& session);
    void onTcpMessage(const std::vector<uint8_t>& msg);
    void onTcpError(int code, const std::string& category, const std::string& message);
    void onWsConnected(const std::shared_ptr<SciWsSession>& session);
    void onWsDisconnected(const std::shared_ptr<SciWsSession>& session);
    void onWsMessage(const std::shared_ptr<SciWsSession>& session, const std::vector<uint8_t>& msg);
    void onWsError(int code, const std::string& category, const std::string& message);
    void manageMessage(const std::string& message, const std::shared_ptr<SciWsSession>& response_session = nullptr);
    static void onRuntimeStable();
    void sendMessage(const std::string& msg);
    void broadcastMessage(const std::string& msg);
    void handleDidOpen(const nlohmann::json& message);
    void handleDidChange(const nlohmann::json& message);
    void handleDidClose(const nlohmann::json& message);
    void handleDidSave(const nlohmann::json& message);
    void handleCompletion(const nlohmann::json& message);
    void handleCompletionResolve(const nlohmann::json& message);
    void handleSignatureHelp(const nlohmann::json& message);
    void handleHover(const nlohmann::json& message);
    void handleDocumentSymbol(const nlohmann::json& message);
    void handleFoldingRange(const nlohmann::json& message);
    void handleDefinition(const nlohmann::json& message);
    void handleSemanticTokens(const nlohmann::json& message);
    void handleFormatting(const nlohmann::json& message);
    struct DocumentAnalysisCacheEntry
    {
        int version = 0;
        std::string parseError;
        DocumentAnalysis analysis;
    };
    struct DocumentState
    {
        std::unordered_map<std::string, std::vector<std::string>> files;
        std::unordered_map<std::string, DocumentAnalysisCacheEntry> analyses;
        std::unordered_map<std::string, int> pendingAnalysisVersions;
    };
    bool updateDocumentAnalysis(DocumentState& state, const std::string& uri, int version,
        bool useRuntimeContext = true);
    bool processPendingDocumentAnalyses(DocumentState& state,
        std::vector<std::string>* diagnostics = nullptr);
    void processPendingDocumentAnalyses();
    void refreshSemanticTokens(bool broadcast = false);
    const std::vector<std::string>* findDocument(const std::string& uri) const;
    const DocumentAnalysisCacheEntry* findDocumentAnalysis(const std::string& uri) const;
    std::vector<std::string> visibleDocumentUris() const;
    DocumentState& currentDocumentState();
    const RuntimeSnapshot& runtimeSnapshot() const;

    std::shared_ptr<SciTcpServer> m_tcp_server;
    std::shared_ptr<SciWsServer> m_ws_server;
    std::shared_ptr<CppServer::Asio::Service> m_service;
    std::atomic<bool> m_stopping{false};
    std::shared_ptr<SciTcpSession> m_tcp_session;
    std::unordered_set<std::shared_ptr<SciWsSession>> m_ws_sessions;
    DocumentState m_tcpDocumentState;
    std::unordered_map<const SciWsSession*, DocumentState> m_wsDocumentStates;
    static thread_local DocumentState* s_currentDocumentState;
    bool m_linter;
    bool m_spawned;
    int m_iPort;
    int m_sequence;
    std::string m_incoming;
    std::string m_workdir;
    std::wstring m_linter_conf;
    RuntimeSnapshot m_runtimeSnapshot;
    //slint::SLintScilabOut* m_linter_results;

    // Documents pre-indexed by the host (SciNotes) remain a shared, read-only
    // fallback. Unsaved LSP contents live in the current session state above.
    DocumentState m_indexedDocuments;
public:
    enum Method {
        undefined, // unmanaged command => default switch case
        initialize,
        initialized,
        shutdown,
        exit,
        textDocument_didOpen,
        textDocument_didChange,
        textDocument_didClose,
        textDocument_didSave,
        textDocument_hover,
        // textDocument_diagnostic,
        textDocument_completion,
        textDocument_documentSymbol,
        textDocument_foldingRange,
        textDocument_definition,
        textDocument_signatureHelp,
        textDocument_semanticTokens_full,
        textDocument_formatting,
        textDocument_rangeFormatting,
        completionItem_resolve,
        workspace_executeCommand,
        workspace_didChangeConfiguration,
        workspace_diagnostic,
    };
};

#endif /* !__LSP_ENDPOINT_HXX__ */
