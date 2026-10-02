/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <cctype>
#include <algorithm>
#include <regex>
#include <set>
#include <sstream>
#include <tuple>
#ifndef _MSC_VER
#include <curl/curl.h>
#endif

#include "lspEndpoint.hxx"
#include "lspProtocolUtils.hxx"
#include "json_rpc.hxx"
#include "json.hxx"
#include "lsp_dap_tools.hxx"
#include "documentAnalysisVisitor.hxx"

#include "struct.hxx"
#include "UTF8.hxx"
#include "SLint.hxx"
//#include "output/SLintXmlResult.hxx"
#include "config/XMLConfig.hxx"
#include "configvariable.hxx"
#include "visitor_common.hxx"
#include "macrofile.hxx"
#include "callable.hxx"

#ifdef _MSC_VER
#include "windows_tools.hxx"
#endif

extern "C"
{
    #include "version.h"
    #include "storeCommand.h"
    #include "FileExist.h"
    #include "expandPathVariable.h"
    #include "scicurdir.h"
    #include "sci_path.h"

    #ifdef _MSC_VER
    #include <shlwapi.h>
    #endif
}
// #define LSP_LOG

static thread_local std::shared_ptr<SciWsSession> s_response_ws_session = nullptr;
thread_local LspEndpoint::DocumentState* LspEndpoint::s_currentDocumentState = nullptr;

static std::map<std::string, LspEndpoint::Method> s_Method = {
    { "initialize", LspEndpoint::initialize },
    { "initialized", LspEndpoint::initialized },
    { "shutdown", LspEndpoint::shutdown },
    { "exit", LspEndpoint::exit },

    { "textDocument/didOpen", LspEndpoint::textDocument_didOpen },
    { "textDocument/didChange", LspEndpoint::textDocument_didChange },
    { "textDocument/didClose", LspEndpoint::textDocument_didClose },
    { "textDocument/didSave", LspEndpoint::textDocument_didSave },
    { "textDocument/hover", LspEndpoint::textDocument_hover },
    // { "textDocument/diagnostic", LspEndpoint::textDocument_diagnostic },
    { "textDocument/completion", LspEndpoint::textDocument_completion },
    { "textDocument/documentSymbol", LspEndpoint::textDocument_documentSymbol },
    { "textDocument/foldingRange", LspEndpoint::textDocument_foldingRange },
    { "textDocument/definition", LspEndpoint::textDocument_definition },
    { "textDocument/signatureHelp", LspEndpoint::textDocument_signatureHelp },
    { "textDocument/semanticTokens/full", LspEndpoint::textDocument_semanticTokens_full },
    { "textDocument/formatting", LspEndpoint::textDocument_formatting },
    { "textDocument/rangeFormatting", LspEndpoint::textDocument_rangeFormatting },

    { "completionItem/resolve", LspEndpoint::completionItem_resolve },

    { "workspace/executeCommand", LspEndpoint::workspace_executeCommand },
    { "workspace/didChangeConfiguration", LspEndpoint::workspace_didChangeConfiguration }
};

LspEndpoint::LspEndpoint():
    m_linter(false),
    m_spawned(false),
    m_iPort(-1), m_sequence(1),
    m_incoming(""), m_workdir(""),
    m_linter_conf(L"")
    //m_linter_results(nullptr)
{
    ConfigVariable::setFlushStream(false);
    m_service = std::make_shared<CppServer::Asio::Service>();
    m_service->Start();
    RuntimeAccess_addStableCallback(&LspEndpoint::onRuntimeStable);
}

LspEndpoint::~LspEndpoint()
{
    m_stopping.store(true, std::memory_order_release);
    RuntimeAccess_removeStableCallback(&LspEndpoint::onRuntimeStable);

    // if(m_linter_results)
    // {
    //     delete m_linter_results;
    // }

    if (m_tcp_server)
    {
        m_tcp_server->Stop();
    }

    if (m_ws_server)
    {
        m_ws_server->Stop();
    }

#ifndef _MSC_VER
    // TODO: Freeze at exit under windows
    m_service->Stop();
#endif
}

void LspEndpoint::onRuntimeStable()
{
    LspEndpoint& endpoint = getInstance();
    if (endpoint.m_stopping.load(std::memory_order_acquire))
    {
        return;
    }

    endpoint.m_service->Post([&endpoint]() {
        if (endpoint.m_stopping.load(std::memory_order_acquire))
        {
            return;
        }

        endpoint.processPendingDocumentAnalyses();
        if (endpoint.m_stopping.load(std::memory_order_acquire))
        {
            return;
        }
        endpoint.refreshSemanticTokens(true);
    });
}

void LspEndpoint::setSpawned(bool _spawned)
{
    m_spawned = _spawned;
#ifdef _MSC_VER
    if (m_spawned)
    {
        // A Scilab spawned as a server must not be picked to open
        // a file through the file association.
        SetWindowLspDapTitle();
    }
#endif
}

void LspEndpoint::startTcp(int _iPort, bool _spawned)
{
    m_iPort = _iPort;
    setSpawned(_spawned);
    {
        RuntimeTryLockGuard runtimeGuard;
        if (runtimeGuard.ownsLock())
        {
            m_runtimeSnapshot.capture();
        }
    }
    m_incoming.clear();

    if (m_ws_server)
    {
        m_ws_server->Stop();
    }
    m_ws_server = nullptr;
    m_ws_sessions.clear();
    m_wsDocumentStates.clear();

    if (m_tcp_server)
    {
        m_tcp_server->Stop();
    }

    m_tcp_server = std::make_shared<SciTcpServer>(m_service, m_iPort);
    m_tcp_server->setOnConnected(std::bind(&LspEndpoint::onTcpConnected, this, std::placeholders::_1));
    m_tcp_server->setOnDisconnected(std::bind(&LspEndpoint::onTcpDisconnected, this, std::placeholders::_1));
    m_tcp_server->setOnError(std::bind(&LspEndpoint::onTcpError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_tcp_server->Start();
    while(m_tcp_server->IsStarted() == false)
    {
        CppCommon::Thread::Yield();
    }
}

void LspEndpoint::startWs(int _iPort, const std::string& token, bool _spawned)
{
    m_iPort = _iPort;
    setSpawned(_spawned);
    {
        RuntimeTryLockGuard runtimeGuard;
        if (runtimeGuard.ownsLock())
        {
            m_runtimeSnapshot.capture();
        }
    }
    if (m_ws_server)
    {
        m_ws_server->Stop();
    }
    if (m_tcp_server)
    {
        m_tcp_server->Stop();
    }
    m_tcp_server = nullptr;
    m_tcp_session = nullptr;
    m_incoming.clear();
    m_ws_sessions.clear();
    m_wsDocumentStates.clear();
    m_tcpDocumentState = DocumentState{};

    m_ws_server = std::make_shared<SciWsServer>(m_service, m_iPort);
    m_ws_server->setPath("/lsp");
    m_ws_server->setToken(token);
    m_ws_server->setOnWsConnected(std::bind(&LspEndpoint::onWsConnected, this, std::placeholders::_1));
    m_ws_server->setOnWsDisconnected(std::bind(&LspEndpoint::onWsDisconnected, this, std::placeholders::_1));
    m_ws_server->setOnError(std::bind(&LspEndpoint::onWsError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_ws_server->Start();
    while(m_ws_server->IsStarted() == false)
    {
        CppCommon::Thread::Yield();
    }

#ifdef LSP_LOG
    std::cout << "[LspClient] WS started on port " << _iPort << std::endl;
#endif
}

void LspEndpoint::onTcpConnected(std::shared_ptr<CppServer::Asio::TCPSession>& session)
{
#ifdef LSP_LOG
    std::cout << "[LspEndpoint] TCP client connected on port " << m_iPort << std::endl;
#endif
    m_tcp_session = std::dynamic_pointer_cast<SciTcpSession>(session);
    m_tcpDocumentState = DocumentState{};
    if (m_tcp_session)
    {
        m_tcp_session->setOnMessage(std::bind(&LspEndpoint::onTcpMessage, this, std::placeholders::_1));
    }
}

void LspEndpoint::onTcpDisconnected(std::shared_ptr<CppServer::Asio::TCPSession>& /*session*/)
{
#ifdef LSP_LOG
    std::cout << "[LspEndpoint] TCP client disconnected" << std::endl;
#endif
    m_tcp_session = nullptr;
    m_tcpDocumentState = DocumentState{};
}

void LspEndpoint::onWsConnected(const std::shared_ptr<SciWsSession>& session)
{
#ifdef LSP_LOG
    std::cout << "[LspClient] WS client connected" << std::endl;
#endif
    m_ws_sessions.insert(session);
    m_wsDocumentStates.emplace(session.get(), DocumentState{});

    std::weak_ptr<SciWsSession> weak_session(session);
    session->setOnMessage([this, weak_session](const std::vector<uint8_t>& msg) {
        if (auto locked = weak_session.lock())
        {
            onWsMessage(locked, msg);
        }
    });
}

void LspEndpoint::onWsDisconnected(const std::shared_ptr<SciWsSession>& session)
{
#ifdef LSP_LOG
    std::cout << "[LspClient] WS client disconnected" << std::endl;
#endif
    m_ws_sessions.erase(session);
    m_wsDocumentStates.erase(session.get());
}

void LspEndpoint::onWsError(int code, const std::string& category, const std::string& message)
{
#ifdef LSP_LOG
    std::cout << "[LspClient] WS Error code "<< code <<": " << category << std::endl << message << std::endl;
#endif
}

void LspEndpoint::onTcpError(int code, const std::string& category, const std::string& message)
{
#ifdef LSP_LOG
    std::cout << "[LspEndpoint] TCP error code "<< code <<": " << category << std::endl << message << std::endl;
#endif
}

void LspEndpoint::onTcpMessage(const std::vector<uint8_t>& data)
{
#ifdef LSP_LOG
    std::cout << "[LspEndpoint] Received message size: " << data.size() << " bytes" << std::endl;
#endif

    std::string msg((char*)data.data(), data.size());
    m_incoming.append(msg);
    // Data string format: "Content-Length: xxx\r\n\r\nJSON"
    while(m_incoming.length())
    {
        // Decode JSON RPC message
        std::string json_string;
        size_t iSize = json_rpc_decode(m_incoming, json_string);
        if(iSize == 0)
        {
            // Data not fully sent, wait for the next onMessage
            break;
        }

        // Manage message
        manageMessage(json_string);

        // Remove message from the orignal string to continue on the next one
        m_incoming.erase(0, iSize);
    }
}

void LspEndpoint::onWsMessage(const std::shared_ptr<SciWsSession>& session, const std::vector<uint8_t>& data)
{
    std::string msg((char*)data.data(), data.size());
#ifdef LSP_LOG
    std::cout << "[LspClient] Received WS message: " << msg << std::endl;
#endif
    manageMessage(msg, session);
}

void LspEndpoint::manageMessage(const std::string& msg, const std::shared_ptr<SciWsSession>& response_session)
{
    std::shared_ptr<SciWsSession> previous_response_session = s_response_ws_session;
    s_response_ws_session = response_session;
    struct ResponseSessionGuard
    {
        std::shared_ptr<SciWsSession>& current;
        std::shared_ptr<SciWsSession> previous;

        ~ResponseSessionGuard()
        {
            current = previous;
        }
    } guard{s_response_ws_session, previous_response_session};

    DocumentState* previous_document_state = s_currentDocumentState;
    s_currentDocumentState = response_session
        ? &m_wsDocumentStates[response_session.get()]
        : &m_tcpDocumentState;
    struct DocumentStateGuard
    {
        DocumentState*& current;
        DocumentState* previous;

        ~DocumentStateGuard()
        {
            current = previous;
        }
    } documentStateGuard{s_currentDocumentState, previous_document_state};

    json json_msg;
    try
    {
        json_msg = json::parse(msg);
    }
    catch(const json::parse_error& e)
    {
        std::cout << "exception " << e.id << std::endl;
        std::cout << e.what() << " at position " << e.byte << std::endl;
        return;
    }

#ifdef LSP_LOG
    std::cout << std::setw(4) << json_msg << std::endl;
#endif

    try
    {
        if(json_msg.contains("method") == false)
        {
            // response from vscode: not needed for now
            return;
        }

        if(json_msg["method"].is_string() == false)
        {
            std::cout << "invalid LSP message: missing/invalid 'method'" << std::endl;
            return;
        }

        auto method_it = s_Method.find(json_msg["method"].get<std::string>());
        if(method_it == s_Method.end())
        {
            std::cout << "unmanaged method: " << json_msg["method"] << std::endl;
            return;
        }

        LspEndpoint::Method method = method_it->second;

        switch(method)
        {
        case initialize:
        {
            json resp = response(json_msg);
            if(json_msg["params"]["rootPath"].is_null() == false)
            {
                m_workdir = json_msg["params"]["rootPath"];
                // cd to working directory
                int ierr = scichdir(m_workdir.data());
                if(ierr)
                {
                    resp["error"] = {
                        { "code", -32603 },
                        { "message", std::string("Failed to set PWD in ") + m_workdir }
                    };
                    sendMessage(resp.dump());
                    return;
                }
            }

            resp["result"]["capabilities"] = {
                { "textDocumentSync", 1 }, //none: 0, full: 1, Incremental: 2
                { "hoverProvider", true },
                { "completionProvider", {
                    { "resolveProvider", true }, // true: get more details on the selected item (completion/resolve)
                    { "triggerCharacters", {"."} } // dot also trigger the completion message
                }},
                // { "diagnosticProvider", true }, // Lint
                { "documentSymbolProvider", true },
                { "foldingRangeProvider", true },
                { "documentFormattingProvider", true },
                { "documentRangeFormattingProvider", true },
                { "definitionProvider", true },
                { "signatureHelpProvider", {
                    {"triggerCharacters", {"("} },
                    {"retriggerCharacters", {","} }
                }},
                { "semanticTokensProvider", {
                    { "legend", {
                        { "tokenTypes", {
                            "namespace", "type", "class", "enum", "interface", "struct", "typeParameter",
                            "parameter", "variable", "property", "enumMember", "event", "function",
                            "method", "macro", "keyword", "modifier", "comment", "string", "number",
                            "regexp", "operator", "decorator", "operatorSpecial", "functionKeyword"
                        }},
                        { "tokenModifiers", {
                            "declaration", "definition", "readonly", "static", "deprecated", "abstract",
                            "async", "modification", "documentation", "defaultLibrary", "scilabRuntime"
                        }}
                    }},
                    { "full", true },
                    { "range", false }
                }},
                { "executeCommandProvider", {
                    { "commands", { "scilab.execute" } }
                }}
            };

            resp["result"]["serverInfo"] = {
                { "name", "Scilab" },
                { "version", SCI_VERSION_STRING }
            };

            sendMessage(resp.dump());
        }
        break;
        case initialized:
        {
            // Retry after the protocol handshake in case the runtime was busy
            // when the TCP or WebSocket endpoint started.
            RuntimeTryLockGuard runtimeGuard;
            if (runtimeGuard.ownsLock())
            {
                m_runtimeSnapshot.capture();
            }
        }
        break;
        case shutdown:
        {
            json resp = response(json_msg);
            resp["result"] = nullptr;
            sendMessage(resp.dump());
        }
        break;
        case exit:
        {
            // Exit Scilab only when spawned by another process (ie: vscode)
            if(m_spawned)
            {
                StorePrioritaryCommand("exit");
            }
        }
        break;
        case textDocument_didOpen: handleDidOpen(json_msg); break;
        case textDocument_didChange: handleDidChange(json_msg); break;
        case textDocument_didClose: handleDidClose(json_msg); break;
        case textDocument_didSave: handleDidSave(json_msg); break;
        case textDocument_completion: handleCompletion(json_msg); break;
        case completionItem_resolve: handleCompletionResolve(json_msg); break;
        case textDocument_documentSymbol: handleDocumentSymbol(json_msg); break;
        case textDocument_foldingRange: handleFoldingRange(json_msg); break;
        case textDocument_definition: handleDefinition(json_msg); break;
        case textDocument_signatureHelp: handleSignatureHelp(json_msg); break;
        case textDocument_hover: handleHover(json_msg); break;
        case textDocument_semanticTokens_full: handleSemanticTokens(json_msg); break;
        case textDocument_formatting:
        case textDocument_rangeFormatting: handleFormatting(json_msg); break;
        case workspace_executeCommand:
        {
            json resp = response(json_msg);
            if (!json_msg.contains("params") || !json_msg["params"].is_object())
            {
                resp["error"] = {
                    { "code", InvalidParams },
                    { "message", "workspace/executeCommand expects an object in params" }
                };
                sendMessage(resp.dump());
                break;
            }

            const json& params = json_msg["params"];
            if (!params.contains("command")
                    || !params["command"].is_string()
                    || params["command"] != "scilab.execute")
            {
                resp["error"] = {
                    { "code", InvalidParams },
                    { "message", "Unsupported command: expected scilab.execute" }
                };
                sendMessage(resp.dump());
                break;
            }

            if (!params.contains("arguments")
                    || !params["arguments"].is_array()
                    || params["arguments"].empty()
                    || !params["arguments"][0].is_string())
            {
                resp["error"] = {
                    { "code", InvalidParams },
                    { "message", "scilab.execute expects a command string in arguments[0]" }
                };
                sendMessage(resp.dump());
                break;
            }

            const std::string cmd = params["arguments"][0];
            switch(StoreConsoleCommand(cmd.data(), 0))
            {
                case Succeded: resp["result"] = 0; break;
                case Failed: resp["result"] = 1; break;
                case NotAllControlClosed: resp["result"] = 2; break;
                default: resp["result"] = 3;
            }
            sendMessage(resp.dump());
        }
        break;
        case workspace_didChangeConfiguration:
        {
        }
        break;
        default:
        {
            std::cout << "unmanaged method: " << json_msg["method"] << std::endl;
        }
        }
    }
    catch(const json::exception& e)
    {
        std::cout << "json exception " << e.id << ": " << e.what() << std::endl;
    }

}


void LspEndpoint::sendMessage(const std::string& msg)
{
#ifdef LSP_LOG
    std::string_view str(msg);
    std::cout << "[LspClient] Send message:" << std::endl << str.substr(0, 1000) << std::endl;
#endif

    if (s_response_ws_session)
    {
        s_response_ws_session->SendTextMessage(msg);
        return;
    }

    // TCP mode
    if (m_tcp_session)
    {
        std::string encoded;
        json_rpc_encode(msg, encoded);
        m_tcp_session->Send(encoded);
    }
}

void LspEndpoint::broadcastMessage(const std::string& msg)
{
    // Notify every WS client when the LSP uses the WS transport.
    for (const std::shared_ptr<SciWsSession>& session : m_ws_sessions)
    {
        session->SendTextMessage(msg);
    }

    // Notify the TCP client when the LSP uses the TCP transport.
    if (m_tcp_session)
    {
        std::string encoded;
        json_rpc_encode(msg, encoded);
        m_tcp_session->Send(encoded);
    }
}
