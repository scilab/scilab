/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifdef _MSC_VER
// asio, included through tcpSession.hxx, defines NOMINMAX as 1 while the Scilab core
// headers define it empty: define it first, identically to the core headers, so that
// neither definition is a redefinition of the other (C4005).
#define NOMINMAX
#endif

#include <algorithm>
#include <regex>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <set>

#include "tcpSession.hxx"
#include "dapEndpoint.hxx"

#include "debugmanager.hxx"
#include "breakpoint.hxx"
#include "debuggervisitor.hxx"
#include "evalVisitor.hxx"
#include "scilabWrite.hxx"
#include "visitor_common.hxx"
#include "cell.hxx"
#include "tlist.hxx"
#include "json.hxx"

#include "json_rpc.hxx"
#include "lsp_dap_tools.hxx"
#include "runtimeaccess.hxx"
#include "runner.hxx"

#ifdef _MSC_VER
#include "windows_tools.hxx"
#endif

// #define DAP_LOG

extern "C"
{
    // set command
    #include "storeCommand.h"
    #include "parsecommand.h"

    // scilab scichdir / isdir
    #include "scicurdir.h"
    #include "isdir.h"
    #include "fullpath.h"
    #include "FileExist.h"

    //history
    #include "HistoryManager.h"
}

static void DapOutput(const char *text)
{
    DapEndpoint::getInstance().output(text);
}
static void DapError(const char *text)
{
    DapEndpoint::getInstance().error(text);
}

static std::map<std::string, DapEndpoint::Command> s_Command = {
    { "initialize", DapEndpoint::initialize },
    { "attach", DapEndpoint::attach },
    { "launch", DapEndpoint::launch },
    { "setBreakpoints", DapEndpoint::setBreakpoints },
    { "configurationDone", DapEndpoint::configurationDone },
    { "threads", DapEndpoint::threads },
    { "continue", DapEndpoint::continue_ },
    { "next", DapEndpoint::next },
    { "pause", DapEndpoint::pause_ },
    { "stepIn", DapEndpoint::stepin },
    { "stepOut", DapEndpoint::stepout },
    { "stackTrace", DapEndpoint::stackTrace },
    { "scopes", DapEndpoint::scopes },
    { "variables", DapEndpoint::variables },
    { "evaluate", DapEndpoint::evaluate },
    { "completions", DapEndpoint::completions },
    { "terminate", DapEndpoint::terminate_ },
    { "disconnect", DapEndpoint::disconnect },
    { "scilabExecFile", DapEndpoint::scilabExecFile },
    { "scilab/getFullValue", DapEndpoint::scilabGetFullValue }
};

static std::set<std::string> s_UserVariables = {
    "home","PWD","%tk","%pvm","MSDOS","%F",
    "%T","%f","%t","%e","%pi","%modalWarning",
    "%nan","%inf","SCI","WSCI","SCIHOME","TMPDIR",
    "%gui","%fftw","%helps","%eps","%io","%i",
    "demolist","%z","%s","$","%toolboxes",
    "%toolboxes_dir","TICTOC","%helps_modules",
    "%_atoms_cache","%chars",
    "evoid", // Constant for external object
    "jvoid", // Constant for external object Java (jims)
    "jnull", // Constant for external object Java (jims)
    "enull"  // Constant for external object
};

static json response(const json _query);
static json event(const std::string& _command);

static bool isHitCount(const std::string& text)
{
    // Short enough for std::stoi never to overflow.
    return text.empty() == false && text.size() <= 9
        && std::all_of(text.begin(), text.end(), [](unsigned char c) { return std::isdigit(c) != 0; });
}

// The lines DebuggerVisitor can stop on: where a statement of a block starts.
static void collectStatementLines(const ast::Exp* exp, std::set<int>& lines)
{
    if(exp == nullptr)
    {
        return;
    }

    if(exp->isSeqExp())
    {
        for(const ast::Exp* statement : exp->getExps())
        {
            if(statement && statement->isCommentExp() == false && statement->isArgumentsExp() == false)
            {
                lines.insert(statement->getLocation().first_line);
            }
        }
    }

    for(const ast::Exp* child : exp->getExps())
    {
        collectStatementLines(child, lines);
    }
}

// Sets *known to false when the file cannot be read or does not parse cleanly.
static std::set<int> readStatementLines(const std::string& filename, bool* known)
{
    std::set<int> lines;
    *known = false;

    std::ifstream file(std::filesystem::u8path(filename), std::ios::binary);
    if(file.is_open() == false)
    {
        return lines;
    }

    std::vector<std::string> content;
    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty() == false && line.back() == '\r')
        {
            line.pop_back();
        }

        content.push_back(line);
    }

    ast::Exp* tree = nullptr;
    std::string error;
    // A recovered tree may miss statements past the error: trust complete ones only.
    if(parseDocumentWithRecoveryWithoutRuntime(content, &tree, &error) && tree && error.empty())
    {
        if(tree->isSeqExp() == false)
        {
            lines.insert(tree->getLocation().first_line);
        }

        collectStatementLines(tree, lines);
        *known = true;
    }

    delete tree;
    return lines;
}

DapEndpoint::DapEndpoint(): m_server(nullptr),
    m_standard_session_owner(nullptr), m_session_owner(nullptr),
    m_session_configured(false),
    m_next_breakpoint_id(1),
    m_launched(false), m_attached(false),
    m_stop_on_entry(false), m_keep_alive(false),
    m_is_running(false), m_stopped_on_error(false),
    m_execution_done(false),m_debug_mode(true),
    m_spawned(false), m_file_to_execute(""),
    m_iPort(-1),
    m_next_variable_ref(1)
{
    ConfigVariable::setFlushStream(false);
    m_service = std::make_shared<CppServer::Asio::Service>();
    m_service->Start();
}

DapEndpoint::~DapEndpoint()
{
    // Stopping twice is an assertion failure in CppServer, and shutdown() has
    // usually stopped both already when Scilab exits normally.
    if(m_server && m_server->IsStarted())
    {
        m_server->Stop();
    }
    if(m_ws_server && m_ws_server->IsStarted())
    {
        m_ws_server->Stop();
    }

#ifndef _MSC_VER
    // TODO: Freeze at exit under windows
    m_service->Stop();
#endif
}

void DapEndpoint::setSpawned(bool _spawned)
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

void DapEndpoint::startTcp(int _iPort, bool _spawned)
{
    m_iPort = _iPort;
    setSpawned(_spawned);
    m_ws_server = nullptr;
    m_clients.clear();
    m_server = std::make_shared<SciTcpServer>(m_service, m_iPort);
    m_server->setOnConnected(std::bind(&DapEndpoint::onConnected, this, std::placeholders::_1));
    m_server->setOnDisconnected(std::bind(&DapEndpoint::onDisconnected, this, std::placeholders::_1));
    m_server->setOnError(std::bind(&DapEndpoint::onError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_server->Start();
    while(m_server->IsStarted() == false)
    {
        CppCommon::Thread::Yield();
    }

#ifdef DAP_LOG
    std::cout << "[DapServer] started on port " << _iPort;
#endif
}

void DapEndpoint::startWs(int _iPort, const std::string& token, bool _spawned)
{
    m_iPort = _iPort;
    setSpawned(_spawned);
    m_server = nullptr;
    m_clients.clear();

    m_ws_server = std::make_shared<SciWsServer>(m_service, m_iPort);
    m_ws_server->setPath("/dap");
    m_ws_server->setToken(token);
    m_ws_server->setOnWsConnected(std::bind(&DapEndpoint::onWsConnected, this, std::placeholders::_1));
    m_ws_server->setOnWsDisconnected(std::bind(&DapEndpoint::onWsDisconnected, this, std::placeholders::_1));
    m_ws_server->setOnError(std::bind(&DapEndpoint::onWsError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_ws_server->Start();
    while(m_ws_server->IsStarted() == false)
    {
        CppCommon::Thread::Yield();
    }

#ifdef DAP_LOG
    std::cout << "[DapServer] WS started on port " << _iPort;
#endif
}

void DapEndpoint::onConnected(std::shared_ptr<CppServer::Asio::TCPSession>& session)
{
    // manage new client connection
#ifdef DAP_LOG
    std::cout << "[DapServer] Client connection "<< session->id() << std::endl;
#endif
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    std::shared_ptr<SciTcpSession> tcpSession = std::dynamic_pointer_cast<SciTcpSession>(session);
    auto client = std::make_shared<ClientContext>();
    client->transport = ClientContext::Transport::Tcp;
    client->tcpSession = tcpSession;
    m_clients.insert(client);

    std::weak_ptr<ClientContext> weak_client(client);
    tcpSession->setOnMessage([this, weak_client](const std::vector<uint8_t>& msg) {
        if(auto locked = weak_client.lock())
        {
            onTcpMessage(locked, msg);
        }
    });

    if(m_spawned)
    {
        // redirect execution output in a stringstream
        setScilabOutputStreamMethod(DapOutput);
        setScilabErrorStreamMethod(DapError);
    }
}

void DapEndpoint::onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession>& session)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] Client disconnection "<< session->id() << std::endl;
#endif
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    bool ownerLeft = false;
    std::shared_ptr<SciTcpSession> tcpSession = std::dynamic_pointer_cast<SciTcpSession>(session);
    for(auto client = m_clients.begin(); client != m_clients.end(); ++client)
    {
        if((*client)->transport == ClientContext::Transport::Tcp && (*client)->tcpSession == tcpSession)
        {
            ownerLeft = m_session_owner == client->get();
            dropClientBreakpoints(**client);
            if(m_standard_session_owner == client->get())
            {
                m_standard_session_owner = nullptr;
            }
            m_clients.erase(client);
            break;
        }
    }

    if(ownerLeft)
    {
        releaseSessionOwner();
    }

    updateDebugHold();
}

void DapEndpoint::onError(int code, const std::string& category, const std::string& message)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] Error code "<< code <<": " << category << std::endl << message << std::endl;
#endif
    // server error
}

void DapEndpoint::onWsConnected(const std::shared_ptr<SciWsSession>& session)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] WS client connected" << std::endl;
#endif

    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    auto client = std::make_shared<ClientContext>();
    client->transport = ClientContext::Transport::WebSocket;
    client->wsSession = session;
    m_clients.insert(client);

    std::weak_ptr<ClientContext> weak_client(client);
    session->setOnMessage([this, weak_client](const std::vector<uint8_t>& msg) {
        if(auto locked = weak_client.lock())
        {
            onWsMessage(locked, msg);
        }
    });

    if(m_spawned)
    {
        // redirect execution output in a stringstream
        setScilabOutputStreamMethod(DapOutput);
        setScilabErrorStreamMethod(DapError);
    }
}

void DapEndpoint::onWsDisconnected(const std::shared_ptr<SciWsSession>& session)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] WS client disconnected" << std::endl;
#endif
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    bool ownerLeft = false;
    for(auto client = m_clients.begin(); client != m_clients.end(); ++client)
    {
        if((*client)->transport == ClientContext::Transport::WebSocket && (*client)->wsSession == session)
        {
            ownerLeft = m_session_owner == client->get();
            dropClientBreakpoints(**client);
            if(m_standard_session_owner == client->get())
            {
                m_standard_session_owner = nullptr;
            }
            m_clients.erase(client);
            break;
        }
    }

    if(ownerLeft)
    {
        releaseSessionOwner();
    }

    updateDebugHold();
}

void DapEndpoint::onWsError(int code, const std::string& category, const std::string& message)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] WS error code "<< code <<": " << category << std::endl << message << std::endl;
#endif
}

void DapEndpoint::onTcpMessage(const std::shared_ptr<ClientContext>& client, const std::vector<uint8_t>& data)
{
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    std::string msg((char*)data.data(), data.size());
#ifdef DAP_LOG
    std::cout << "[DapServer] Receive message: " << msg << std::endl;
#endif
    client->incoming.append(msg);
    // Data string format: "Content-Length: xxx\r\n\r\nJSON"
    while(client->incoming.length())
    {
        // Decode JSON RPC message
        std::string json_string;
        size_t iSize = json_rpc_decode(client->incoming, json_string);
        if(iSize == 0)
        {
            // Data not fully sent, wait for the next onMessage
            break;
        }

        // Manage message
        manageMessage(*client, json_string);

        // Remove message from the orignal string to continue on the next one
        client->incoming.erase(0, iSize);
    }

    if(m_debug_mode_to_publish)
    {
        publishDebugMode();
    }
}

void DapEndpoint::onWsMessage(const std::shared_ptr<ClientContext>& client, const std::vector<uint8_t>& data)
{
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    std::string msg((char*)data.data(), data.size());
#ifdef DAP_LOG
    std::cout << "[DapServer] Receive WS message: " << msg << std::endl;
#endif
    manageMessage(*client, msg);
    if(m_debug_mode_to_publish)
    {
        publishDebugMode();
    }
}

void DapEndpoint::manageMessage(ClientContext& client, const std::string& msg)
{
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

#ifdef DAP_LOG
    std::cout << std::setw(4) << json_msg << std::endl;
#endif

    try
    {
        if(json_msg.contains("command") == false || json_msg["command"].is_string() == false)
        {
            std::cout << "invalid DAP message: missing/invalid 'command'" << std::endl;
            return;
        }

        auto cmd_it = s_Command.find(json_msg["command"].get<std::string>());
        if(cmd_it == s_Command.end())
        {
            json resp = response(json_msg);
            std::cout << "unmanaged command: " << json_msg["command"] << std::endl;
            resp["success"] = false;
            resp["message"] = "cancelled"; // value predefined in dap spec
            sendMessage(client, resp.dump());
            return;
        }

        switch(cmd_it->second)
        {
        case initialize:
        {
            const json& arguments = json_msg.value("arguments", json::object());
            client.supportsProgressReporting = arguments.value("supportsProgressReporting", false);
            client.supportsInvalidatedEvent = arguments.value("supportsInvalidatedEvent", false);
            client.initialized = true;

            json resp = response(json_msg);
            resp["body"] = {
                { "supportsConfigurationDoneRequest", true },
                { "supportsCompletionsRequest", true },
                { "supportsConditionalBreakpoints", true },
                { "supportsHitConditionalBreakpoints", true },
                { "supportsTerminateRequest", true },
                { "supportsEvaluateForHovers", true },
                { "completionTriggerCharacters", {"."} }
            };

            // "supportsFunctionBreakpoints": true,
            // "supportsDataBreakpoints": true

            sendMessage(client, resp.dump());

            json initialized = event("initialized");
            sendMessage(client, initialized.dump());
        }
        break;
        case attach:
        {
            json resp = response(json_msg);
            const bool configureSession = m_attached == false && m_launched == false;
            if(joinSession(client, json_msg.value("arguments", json::object()), false, resp) == false)
            {
                sendMessage(client, resp.dump());
                break;
            }

            if(configureSession)
            {
                ensureDebugger();
                m_attached = true;
                m_file_to_execute = "";
                m_working_directory = "";
                m_keep_alive = true;
                if(json_msg["arguments"].contains("stopOnEntry"))
                {
                    m_stop_on_entry = json_msg["arguments"]["stopOnEntry"];
                }

                if(json_msg["arguments"].contains("file"))
                {
                    std::string filename = json_msg["arguments"]["file"];
                    char* f = get_full_path(filename.data());
                    m_file_to_execute = f;
                    FREE(f);
                }

                if(json_msg["arguments"].contains("workingDirectory"))
                {
                    m_working_directory = json_msg["arguments"]["workingDirectory"];
                }
            }

            /*
             * Only the client that opened the session chooses how it ends: a
             * participant joining a running debug session must not be able to
             * decide that Scilab quits, or that it survives, behind its back.
             * Read outside the configuration block on purpose: a stop on error
             * clears the flag, and the owner re-attaching has to set it again.
             */
            if(m_session_owner == &client)
            {
                m_keep_alive = json_msg.value("arguments", json::object()).value("keepAlive", true);
            }

            // Attaching is the explicit "start debugging" of a standard client.
            // SciNotes attaches when it opens: its debug runs do it instead.
            if(client.sharedSession == false)
            {
                client.holdsDebugMode = true;
                updateDebugHold();
            }

            resp["body"]["scilabRunning"] = m_is_running;
            resp["body"]["scilabExecutionDone"] = m_execution_done;
            resp["body"]["scilabDebugMode"] = debugger::DebuggerManager::isDebugModeHeld();
            sendMessage(client, resp.dump());
        }
        break;
        case launch:
        {
            json resp = response(json_msg);
            if(joinSession(client, json_msg.value("arguments", json::object()), true, resp) == false)
            {
                sendMessage(client, resp.dump());
                break;
            }

            ensureDebugger();
            m_launched = true;
            m_session_owner = &client;
            m_file_to_execute = "";
            m_working_directory = "";
            m_keep_alive = false;
            if(json_msg["arguments"].contains("noDebug"))
            {
                m_debug_mode = json_msg["arguments"]["noDebug"] == false;
            }

            if(json_msg["arguments"].contains("keepAlive"))
            {
                m_keep_alive = json_msg["arguments"]["keepAlive"];
            }

            if(json_msg["arguments"].contains("stopOnEntry"))
            {
                m_stop_on_entry = json_msg["arguments"]["stopOnEntry"];
            }

            if(json_msg["arguments"].contains("file"))
            {
                std::string filename = json_msg["arguments"]["file"];
                char* f = get_full_path(filename.data());
                m_file_to_execute = f;
                FREE(f);
            }

            if(json_msg["arguments"].contains("workingDirectory"))
            {
                m_working_directory = json_msg["arguments"]["workingDirectory"];
            }

            // Scilab outliving its debug run stays in debug mode until the session ends.
            client.holdsDebugMode = m_debug_mode && m_keep_alive;
            updateDebugHold();

            sendMessage(client, resp.dump());
        }
        break;
        case setBreakpoints:
        {
            json resp = response(json_msg);
            resp["body"]["breakpoints"] = json::array({});
            std::string filename = json_msg["arguments"]["source"]["path"];
            char* f = get_full_path(filename.data());
            filename = f;
            FREE(f);

            /*
             * This replaces the list of this client for this source, and only
             * that one. Scilab then runs the union of every client's list, so
             * a second editor joining the session cannot remove the lines of
             * the first, and each of them keeps removing its own.
             */
            bool linesKnown = false;
            const std::set<int> statementLines = readStatementLines(filename, &linesKnown);

            // One entry per requested breakpoint, in order: a refused one keeps
            // its line and carries the reason instead of joining the list.
            // scilabReason names the reason for a client that translates it.
            struct Requested
            {
                DesiredBreakpoint wanted;
                std::string reason;
                std::string message;
            };
            std::vector<Requested> requested;
            std::vector<DesiredBreakpoint> desired;
            for(const auto& breakpoint : json_msg["arguments"]["breakpoints"])
            {
                DesiredBreakpoint wanted;
                wanted.line = breakpoint.value("line", 0);
                wanted.condition = breakpoint.value("condition", std::string());
                if(breakpoint.contains("hitCondition") && breakpoint["hitCondition"].is_string())
                {
                    wanted.hitCondition = breakpoint["hitCondition"];
                }

                std::string reason;
                std::string refused;
                if(wanted.hitCondition.empty() == false && isHitCount(wanted.hitCondition) == false)
                {
                    reason = "hitCount";
                    refused = "The hit count must be a whole number: the breakpoint stops from that hit on.";
                }
                else if(linesKnown)
                {
                    // Scilab only stops where a statement starts: move the
                    // breakpoint there, as the stop would happen anyway.
                    const auto next = statementLines.lower_bound(wanted.line);
                    if(next == statementLines.end())
                    {
                        reason = "noCode";
                        refused = "No code runs on or after this line.";
                    }
                    else
                    {
                        wanted.line = *next;
                    }
                }

                if(refused.empty())
                {
                    desired.push_back(wanted);
                }

                requested.push_back({wanted, reason, refused});
            }

            auto& perSource = m_desired_breakpoints[&client];
            if(desired.empty())
            {
                perSource.erase(filename);
                if(perSource.empty())
                {
                    m_desired_breakpoints.erase(&client);
                }
            }
            else
            {
                perSource[filename] = desired;
            }

            resolveBreakpoints(filename, &client);

            // The response describes the list of its requester, one entry per
            // requested breakpoint and in the same order, as DAP requires.
            const auto installed = m_breakpoints.find(filename);
            const bool sourceExists = FileExist(filename.data());
            for(const auto& request : requested)
            {
                const DesiredBreakpoint& wanted = request.wanted;
                if(request.message.empty() == false)
                {
                    resp["body"]["breakpoints"].push_back({
                        {"verified", false},
                        {"line", wanted.line},
                        {"reason", "failed"},
                        {"message", request.message},
                        {"scilabReason", request.reason}
                    });
                    continue;
                }

                bool answered = false;
                if(installed != m_breakpoints.end())
                {
                    for(const auto& entry : installed->second)
                    {
                        if(entry.desired == wanted && entry.verified)
                        {
                            resp["body"]["breakpoints"].push_back({
                                {"id", entry.id},
                                {"verified", true},
                                {"line", entry.desired.line}
                            });
                            answered = true;
                            break;
                        }
                    }
                }

                if(answered == false)
                {
                    json refused = {{"verified", false}, {"line", wanted.line}, {"reason", "failed"}};
                    if(sourceExists == false)
                    {
                        refused["message"] = "The file does not exist on disk.";
                        refused["scilabReason"] = "noFile";
                    }

                    resp["body"]["breakpoints"].push_back(refused);
                }
            }

            sendMessage(client, resp.dump());
        }
        break;
        case configurationDone:
        {
            json resp = response(json_msg);

            if(client.sessionActive == false)
            {
                resp["success"] = false;
                resp["message"] = "cancelled";
                sendMessage(client, resp.dump());
                break;
            }

            /*
             * A participant joining a configured session sends its own
             * configurationDone: answer it, hand it the stop the session may
             * already be on, and above all do not configure or run anything a
             * second time.
             */
            if(m_session_configured)
            {
                sendMessage(client, resp.dump());
                replayBreakpoints(client);
                replayStoppedState(client);
                break;
            }
            // cd to working directory
            if (m_working_directory.empty() == false)
            {
                if(isdir(m_working_directory.c_str()) == false)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = {
                        {"format", std::string("Unable to open workspace directory: ") + m_working_directory}
                    };
                    sendMessage(client, resp.dump());
                    return;
                }

                int ierr = scichdir(m_working_directory.data());
                if(ierr)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = {
                        {"format", std::string("Failed to set PWD in ") + m_working_directory}
                    };
                    sendMessage(client, resp.dump());
                    return;
                }
            }

            m_session_configured = true;

            /*
             * The file of an attach configuration only runs when Scilab has
             * nothing else going on: joining a session that is already running
             * or stopped means watching it, not starting a second run.
             */
            if(m_is_running || debugger::DebuggerManager::getInstance()->isInterrupted())
            {
                m_file_to_execute.clear();
            }

            if(m_file_to_execute.empty() == false)
            {
                sendProgressStart("Scilab", "Scilab debug is running");

                debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
                if(m_stop_on_entry && m_debug_mode)
                {
                    // fake breakpoint to stop a the first line of the executed file
                    debugger::Breakpoint* bp = new debugger::Breakpoint();
                    bp->setFileName(m_file_to_execute);
                    bp->setFileLine(-1); // -1: ephemeral bp, stop at entry in the file then the bp is deleted
                    manager->addBreakPoint(bp);
                    // reset the flag for "attach" case or F5 after F10 will stop on entry
                    m_stop_on_entry = false;
                }

                std::string cmd = "exec(\""+m_file_to_execute+"\");";
                // Consumed: a later configurationDone must not replay it.
                m_file_to_execute.clear();
                char* error = startRun(cmd, m_debug_mode);
                if(error)
                {
                    FREE(error);
                }
            }

            sendMessage(client, resp.dump());
            replayBreakpoints(client);
            replayStoppedState(client);
        }
        break;
        /*
         * Run a file in the Scilab this session is attached to. Not a launch:
         * launch owns the process it starts, and is refused once a session is
         * attached. The run is debugged unless "debug" is false; either way
         * the session is told a run is going on.
         */
        case scilabExecFile:
        {
            json resp = response(json_msg);
            const json args = json_msg.value("arguments", json::object());
            const std::string file = args.value("file", std::string());

            if(client.sessionActive == false || m_session_configured == false)
            {
                resp["success"] = false;
                resp["message"] = "cancelled";
                resp["body"]["error"]["format"] = "no configured debug session on this connection";
            }
            else if(file.empty())
            {
                resp["success"] = false;
                resp["message"] = "cancelled";
                resp["body"]["error"]["format"] = "scilabExecFile requires a file";
            }
            else if(isBusy(args.value("debug", true)))
            {
                resp["success"] = false;
                resp["message"] = "cancelled";
                resp["body"]["error"]["format"] = "Scilab is busy";
            }
            else
            {
                // A debug run of SciNotes opens its debug session: Scilab stays
                // in debug mode after it. A run without the debugger closes it.
                if(client.sharedSession)
                {
                    m_scinotes_holds_debug = args.value("debug", true);
                    updateDebugHold();
                }

                // A double quote inside a Scilab string literal is doubled.
                std::string quoted;
                for(const char c : file)
                {
                    quoted += c;
                    if(c == '"')
                    {
                        quoted += c;
                    }
                }

                const bool echo = args.value("echo", false);
                const std::string cmd = "exec(\"" + quoted + "\", " + (echo ? "1" : "-1") + ");";
                char* error = startRun(cmd, args.value("debug", true));
                if(error)
                {
                    resp["success"] = false;
                    resp["message"] = "cancelled";
                    resp["body"]["error"]["format"] = error;
                    FREE(error);
                }
            }

            sendMessage(client, resp.dump());
        }
        break;
        case threads:
        {
            json resp = response(json_msg);
            resp["body"]["threads"] = json::array({});
            resp["body"]["threads"].push_back(json::object({ {"id", 1} , {"name", "scilab"} }));
            sendMessage(client, resp.dump());
        }
        break;
        case stackTrace:
        {
            json resp = response(json_msg);
            //debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
            if(m_is_running)
            {
                resp["success"] = false;
                resp["message"] = "notStopped"; // value predefined in dap spec
                sendMessage(client, resp.dump());
                return;
            }

            resp["body"]["stackFrames"] = json::array({});
            debugger::DebuggerManager::CallStack cs = debugger::DebuggerManager::getInstance()->getCallStack();

            // set the stack trace
            int frameIndex = 0;
            int emittedFrames = 0;
            int startFrame = json_msg.value("arguments", json::object()).value("startFrame", 0);
            int levels = json_msg.value("arguments", json::object()).value("levels", 0);
            startFrame = std::max(0, startFrame);
            levels = std::max(0, levels);
            resp["body"]["totalFrames"] = cs.stack.size();
            for(const auto& stackRow : cs.stack)
            {
                if(frameIndex++ < startFrame)
                {
                    continue;
                }

                if(levels > 0 && emittedFrames >= levels)
                {
                    break;
                }

                json obj = json::object({
                    {"id", stackRow.scope},
                    {"name", stackRow.functionName},
                    {"line", 0},
                    {"column", 0}
                });

                // A command run between two instructions is no function: what
                // is above it was called from there, not by what is below.
                // Nothing is above it when the stop is in the command itself.
                if(stackRow.functionName == "<console>" || stackRow.functionName == "<callback>")
                {
                    const bool console = stackRow.functionName == "<console>";
                    if(&stackRow == &cs.stack.front())
                    {
                        obj["name"] = console ? "in a console command" : "in a callback";
                    }
                    else
                    {
                        obj["name"] = console ? "called from the console" : "called by a callback";
                    }
                    obj["presentationHint"] = "label";
                }

                if(stackRow.hasFile)
                {
                    /*
                     * Scilab reports the path it recorded when the file was
                     * executed. Clients key their sources on the resolved path,
                     * the one setBreakpoints is stored under, so resolve it
                     * here too rather than letting each client guess.
                     */
                    std::string resolved = stackRow.fileName;
                    char* full = get_full_path(resolved.data());
                    if(full)
                    {
                        resolved = full;
                        FREE(full);
                    }

                    obj["source"] = {{ "path", resolved }};
                    obj["line"] = stackRow.fileLine;
                    obj["column"] = stackRow.column;
                }

                resp["body"]["stackFrames"].push_back(obj);
                emittedFrames++;
            }

            // keep alive, send information as if we stopped on the last line of the executed file. (variable list)
            if(cs.stack.empty() && startFrame == 0 && m_keep_alive && m_file_to_execute.empty() == false)
            {
                std::ifstream myfile(m_file_to_execute);
                auto count = std::count_if(std::istreambuf_iterator<char>{myfile}, {}, [](char c) { return c == '\n'; });

                // count + 2: set more than the number of line to go to the last executed file line without highlight it.
                json obj = json::object({
                    {"id", 1},
                    {"name", "exec"},
                    {"line", count + 2},
                    {"column", 0},
                    {"source", {{ "path", m_file_to_execute }}}
                });

                resp["body"]["stackFrames"].push_back(obj);
                resp["body"]["totalFrames"] = 1;
            }

            sendMessage(client, resp.dump());
        }
        break;
        case scopes:
        {
            json resp = response(json_msg);
            //debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
            if(m_is_running)
            {
                resp["success"] = false;
                resp["message"] = "notStopped"; // value predefined in dap spec
                sendMessage(client, resp.dump());
                return;
            }

            /*
             * The frame id is the Scilab scope level of that frame. Every frame
             * shows the same four groups, so the view does not change shape
             * when another frame is selected. Only "local" follows the frame:
             * "global" and "visible" read the whole context, and "call stack"
             * holds one group per frame, the selected one included, which is
             * how the same name is compared across scopes without selecting
             * each frame in turn.
             */
            const json args = json_msg.value("arguments", json::object());
            const int frameId = args.value("frameId", -1);
            debugger::DebuggerManager::CallStack cs = debugger::DebuggerManager::getInstance()->getCallStack();
            // Row 0 is the frame execution is stopped in. A missing or unknown
            // frameId therefore falls back to it.
            size_t selectedRow = 0;
            for(size_t row = 1; row < cs.stack.size(); ++row)
            {
                if(cs.stack[row].scope == frameId && cs.stack[row].scope != cs.stack[0].scope)
                {
                    selectedRow = row;
                    break;
                }
            }

            const bool stoppedFrame = (selectedRow == 0);
            const int key = stoppedFrame ? -1 : frameId;
            auto cached = m_frame_scopes.find(key);
            if(cached == m_frame_scopes.end())
            {
                std::vector<int> groups;
                /*
                 * getLocalVars() keeps the names whose innermost definition is
                 * at that level, getScopedVars() the value seen at it. Reading
                 * another frame needs the second: the first hides a name that a
                 * deeper scope defines again, the one that frame is opened for.
                 */
                groups.push_back(stoppedFrame ? addScope("local", LOCAL, -1)
                                              : addScope("local", SCOPE, frameId));
                groups.push_back(addScope("global", GLOBAL, -1));
                groups.push_back(addScope("visible", VISIBLE, -1));
                if(cs.stack.empty() == false)
                {
                    groups.push_back(addScope("call stack", CALLSTACK, -1));
                }
                cached = m_frame_scopes.emplace(key, groups).first;
            }

            resp["body"]["scopes"] = json::array();
            for(int ref : cached->second)
            {
                json scope = {{"name", m_variables[ref].name}, {"expensive", false}, {"variablesReference", ref}};
                if(m_variables[ref].name == "local")
                {
                    scope["presentationHint"] = "locals";
                }
                resp["body"]["scopes"].push_back(scope);
            }
            sendMessage(client, resp.dump());
        }
        break;
        case variables:
        {
            json resp = response(json_msg);
            //debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
            if(m_is_running)
            {
                resp["success"] = false;
                resp["message"] = "notStopped"; // value predefined in dap spec
                sendMessage(client, resp.dump());
                return;
            }

            const int iRef = json_msg["arguments"].value("variablesReference", 0);
            if(m_variables.find(iRef) == m_variables.end())
            {
                // Zero, or a reference from a stop that is over.
                resp["success"] = false;
                resp["message"] = "cancelled"; // value predefined in dap spec
                sendMessage(client, resp.dump());
                return;
            }

            resp["body"]["variables"] = json::array();
            for(int child : getChildren(iRef))
            {
                resp["body"]["variables"].push_back(getVariable(child));
            }

            sendMessage(client, resp.dump());
        }
        break;
        case continue_:
        {
            if(m_execution_done)
            {
                m_keep_alive = false;
            }

            if(m_stopped_on_error)
            {
                clearVariables();
                debugger::DebuggerManager::getInstance()->abort();
                // The abort leaves the error stop: nothing else would clear the
                // flag, and every later resume would abort instead of running.
                m_stopped_on_error = false;
                json resp = response(json_msg);
                sendMessage(client, resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(client, resp.dump());

            sendProgressStart("Scilab", "Scilab debug is running");
        }
        break;
        case next:
        {
            if(m_stopped_on_error)
            {
                clearVariables();
                debugger::DebuggerManager::getInstance()->abort();
                // The abort leaves the error stop: nothing else would clear the
                // flag, and every later resume would abort instead of running.
                m_stopped_on_error = false;
                json resp = response(json_msg);
                sendMessage(client, resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->setStepNext();
            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(client, resp.dump());

            sendProgressStart("Scilab", "Scilab debug is running");
        }
        break;
        case pause_:
        {
            json resp = response(json_msg);
            if(debugger::DebuggerManager::isDebugging())
            {
                debugger::DebuggerManager::getInstance()->requestPause();
            }
            else
            {
                // Only a debugged execution can be paused; a request kept
                // pending would stop the next one.
                resp["success"] = false;
                resp["message"] = "cancelled";
                resp["body"]["error"]["format"] = "No debugged execution to pause";
            }

            sendMessage(client, resp.dump());
        }
        break;
        case stepin:
        {
            if(m_stopped_on_error)
            {
                clearVariables();
                debugger::DebuggerManager::getInstance()->abort();
                // The abort leaves the error stop: nothing else would clear the
                // flag, and every later resume would abort instead of running.
                m_stopped_on_error = false;
                json resp = response(json_msg);
                sendMessage(client, resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->setStepIn();
            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(client, resp.dump());

            sendProgressStart("Scilab", "Scilab debug is running");
        }
        break;
        case stepout:
        {
            if(m_stopped_on_error)
            {
                clearVariables();
                debugger::DebuggerManager::getInstance()->abort();
                // The abort leaves the error stop: nothing else would clear the
                // flag, and every later resume would abort instead of running.
                m_stopped_on_error = false;
                json resp = response(json_msg);
                sendMessage(client, resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->setStepOut();
            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(client, resp.dump());

            sendProgressStart("Scilab", "Scilab debug is running");
        }
        break;
        case evaluate:
        {
            bool reloadVariables = false;
            json resp = response(json_msg);
            std::string context = json_msg["arguments"]["context"];

            // Former shape of scilab/getFullValue, kept while clients move to it.
            if(context == "fullValue")
            {
                getFullValue(resp, json_msg["arguments"]);
                sendMessage(client, resp.dump());
                break;
            }

            std::string command = json_msg["arguments"]["expression"];
            if(command == "")
            {
                sendMessage(client, resp.dump());
                return;
            }

            if(context == "hover")
            {
                evaluateForHover(resp, json_msg["arguments"]);
                sendMessage(client, resp.dump());
                break;
            }

            // not yet managed context: 'clipboard' | 'variables'
            if(context == "watch")
            {
                types::InternalType* pIT = nullptr;
                std::string error = eval(command, &pIT);
                // parsing or execution error
                if(error.empty() == false)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = { {"format", "invalid expression" } };
                    sendMessage(client, resp.dump());
                    return;
                }

                if (pIT == nullptr)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = { {"format", "invalid read only execution" } };
                    sendMessage(client, resp.dump());
                    return;
                }

                pIT->IncreaseRef();
                resp["body"] = getVariable(addVariable(command, command, pIT), "result");
            }
            else if(context == "repl") // console: Real Eval Print Loop
            {
                if(m_is_running)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = { {"format", "Scilab is running..."} };
                    sendMessage(client, resp.dump());
                    return;
                }

                // force reload variables and watches for custom evaluate request
                if(json_msg["arguments"].contains("reload"))
                {
                    reloadVariables = json_msg["arguments"]["reload"];
                }

                clearVariables();

                sendProgressStart("Scilab", "Scilab debug is running");

                // Announced from here: the interpreter would need the lock this
                // thread holds while it waits. During a stop nothing is.
                if(debugger::DebuggerManager::getInstance()->isInterrupted() == false)
                {
                    executionStarted();
                }

                // Never debugged: this thread waits for the line under the lock
                // a stop would need to be announced.
                StoreNormalConsoleCommand(command.data(), 1);

                if(reloadVariables == false)
                {
                    appendLineToScilabHistory(command.data());
                }

                sendProgressEnd("Scilab");
            }

            if(reloadVariables)
            {
                json reload = event("invalidated");
                reload["body"] = R"({"areas": ["variables","watches"]})"_json;
                broadcastMessage(reload.dump());
            }

            sendMessage(client, resp.dump());
        }
        break;
        case scilabGetFullValue:
        {
            json resp = response(json_msg);
            getFullValue(resp, json_msg.value("arguments", json::object()));
            sendMessage(client, resp.dump());
        }
        break;
        case completions:
        {
            json resp = response(json_msg);
            std::string text = json_msg["arguments"]["text"];
            int iColumn = json_msg["arguments"]["column"];

            std::string token;
            std::string exp;
            switch(completion_info(text.substr(0, iColumn - 1), token, exp))
            {
                case FIELD:
                {
                    json result = getFieldCompletion(token, exp);
                    if(result.is_array())
                    {
                        resp["body"]["targets"] = result;
                        break;
                    }
                }
                case STRING:
                {
                    resp["body"]["targets"] = getFileCompletion(token);
                    break;
                }
                case DICT:
                {
                    resp["body"]["targets"] = getDictCompletion(token);
                    break;
                }
                default:
                {
                    resp["body"]["targets"] = json::array({});
                }
            }

            sendMessage(client, resp.dump());
        }
        break;
        case terminate_:
        {
            json resp = response(json_msg);
            if(client.sessionActive == false)
            {
                resp["success"] = false;
                resp["message"] = "cancelled";
                sendMessage(client, resp.dump());
                break;
            }

            if(canEndSession(client) == false)
            {
                // Terminating is the owner's call: a participant asking for it
                // would stop a run it did not start, for everybody.
                resp["success"] = false;
                resp["message"] = "cancelled";
                resp["body"]["error"]["format"] = "only the session owner can terminate it";
                sendMessage(client, resp.dump());
                break;
            }

            sendMessage(client, resp.dump());
            endSession(m_launched);
        }
        break;
        case disconnect:
        {
            json resp = response(json_msg);
            sendMessage(client, resp.dump());
            if(client.sessionActive == false)
            {
                break;
            }

            if(canEndSession(client))
            {
                endSession(m_launched);
            }
            else
            {
                // A participant leaves alone: the debug session goes on for the
                // others, and Scilab is none of its business.
                detachClient(client);
            }
        }
        break;
        default:
        {
            std::cout << "unmanaged command: " << json_msg["command"] << std::endl;
        }
        }
    }
    catch(const json::exception& e)
    {
        std::cout << "json exception " << e.id << ": " << e.what() << std::endl;
    }
}

void DapEndpoint::onStop(int bp_idx, const std::string& fileOrFuncName, bool executionDone)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] execution stop:" << std::endl;
#endif

    json stopped = event("stopped");
    if(bp_idx < 0)
    {
        stopped["body"] = json::object({
            {"threadId", 1},
            {"allThreadsStopped", true}
        });

        if(fileOrFuncName.empty())
        {
            /*
             * A run reaching its end keeps Scilab alive and stops on nothing,
             * exactly like a pause. Only the caller can tell them apart:
             * m_execution_done is reset when an execution starts, and an
             * attached session never sees that moment, since SciNotes runs its
             * file through the command queue instead of the debugger.
             */
            stopped["body"]["reason"] = "pause";
            stopped["body"]["description"] = executionDone ? "Execution completed" : "Execution paused";
            if(executionDone)
            {
                stopped["body"]["scilabExecutionDone"] = true;
            }
        }
        else
        {
            stopped["body"]["reason"] = "exception";
            stopped["body"]["description"] = "Stopped on Error";
            stopped["body"]["text"] = scilab::UTF8::toUTF8(ConfigVariable::getLastErrorMessage());
            // on error, close debug session after continue; SciNotes keeps its own
            if(m_session_owner == nullptr || m_session_owner->sharedSession == false)
            {
                m_keep_alive = false;
            }
            m_stopped_on_error = true;
        }
    }
    else
    {
        stopped["body"] = json::object({
            {"reason", "breakpoint"},
            {"threadId", 1},
            {"description", "Paused on breakpoint"},
            {"allThreadsStopped", true}
        });
    }

    /*
     * Broadcast a local copy: the member can be cleared from another thread -
     * a client resuming or leaving - while this call is still reading it.
     */
    const std::string stoppedEvent = stopped.dump();
    m_last_stopped_event = stoppedEvent;
    broadcastMessage(stoppedEvent);

    sendProgressEnd("Scilab");
}

/*
 * A client joining a session that is already stopped receives no event of its
 * own: the stop happened before it was there. Hand it the last one, so its
 * view of the debuggee matches the others' instead of showing a running
 * program. DAP expects that from an adapter attaching to a suspended debuggee.
 */
void DapEndpoint::replayStoppedState(ClientContext& client)
{
    if(m_last_stopped_event.empty() || client.sessionActive == false)
    {
        return;
    }

    if(debugger::DebuggerManager::getInstance()->isInterrupted() == false)
    {
        return;
    }

    const std::string stoppedEvent = m_last_stopped_event;
    sendMessage(client, stoppedEvent);
}

void DapEndpoint::onResume()
{
    // Called from the interpreter thread when a stop ends: the variable store
    // is shared with the requests of the clients.
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    clearVariables();
    m_last_stopped_event.clear();
    m_is_running = true;
    m_execution_done = false;
    m_stopped_on_error = false;

    json continued = event("continued");
    continued["body"] = {
        {"threadId", 1},
        {"allThreadsContinued", true}
    };
    broadcastMessage(continued.dump());
}

void DapEndpoint::executionStarted()
{
    m_execution_done = false;
    m_stopped_on_error = false;
    if(m_is_running == false)
    {
        onResume();
    }
}

/*
 * One group per frame of the call stack, named after its source position, the
 * running one included: the point of the group is to read one name at every
 * level side by side.
 */
std::vector<int> DapEndpoint::getCallStackScopes()
{
    std::vector<int> groups;
    debugger::DebuggerManager::CallStack cs = debugger::DebuggerManager::getInstance()->getCallStack();
    for(const auto& stackRow : cs.stack)
    {
        if(stackRow.hasFile)
        {
            const std::string filename = std::filesystem::path(stackRow.fileName).filename().string();
            groups.push_back(addScope(filename + ":" + std::to_string(stackRow.fileLine), SCOPE, stackRow.scope));
        }
    }

    return groups;
}

std::vector<int> DapEndpoint::getVariableList(VAR_LIST_KIND kind, int iLevel)
{
    symbol::Context* ctx = symbol::Context::getInstance();

    std::list<symbol::Variable*> lstVars;

    if(iLevel == -1)
    {
        iLevel = ctx->getScopeLevel();
    }

    switch(kind)
    {
        case LOCAL:   ctx->getLocalVars(lstVars); break;
        case GLOBAL:  ctx->getGlobalVars(lstVars); break;
        case VISIBLE: ctx->getVarsToVariableBrowser(lstVars); break;
        case SCOPE:   ctx->getScopedVars(lstVars, iLevel); break;
        default: {}
    }

    std::vector<int> refs;

    // for each local variable get information
    for (auto v : lstVars)
    {
        // skip user variables
        std::string name = scilab::UTF8::toUTF8(v->getSymbol().getName());
        if (s_UserVariables.find(name) != s_UserVariables.end())
        {
            continue;
        }

        // return remove global variables from local group
        // but if the variable is defined locally
        if(v->isGlobalVisible(iLevel) && kind == LOCAL)
        {
            continue;
        }

        types::InternalType* pIT = nullptr;
        if((v->isGlobal() && (kind == GLOBAL)) ||
           v->isGlobalVisible(iLevel))
        {
            pIT = v->getGlobalValue();
        }
        else
        {
            //get top level value
            symbol::ScopedVariable* sv = v->top();
            //get value
            pIT = sv->m_pIT;
        }

        if (pIT->isLibrary())
        {
            continue;
        }

        pIT->IncreaseRef();
        // A name read in an outer frame means something else in the current one.
        refs.push_back(addVariable(name, kind == SCOPE ? std::string() : name, pIT));
    }

    if(iLevel > 0 && kind == SCOPE)
    {
        for(auto v : lstVars)
        {
            delete v;
        }
    }

    return refs;
}

std::vector<int> DapEndpoint::getContainerValues(int _iRef)
{
    std::vector<int> content;
    const auto found = m_variables.find(_iRef);
    if(found == m_variables.end())
    {
        return content;
    }

    const std::string name = found->second.name;
    const std::string evaluateName = found->second.evaluateName;
    types::InternalType* pIT = found->second.value;
    // "name(2)" or "name{2}" index the parent, any other name is one of its fields.
    auto childEvaluateName = [&](const std::string& childName) -> std::string
    {
        if(evaluateName.empty())
        {
            return std::string();
        }
        if(childName.size() > name.size() && childName.compare(0, name.size(), name) == 0
           && (childName[name.size()] == '(' || childName[name.size()] == '{'))
        {
            return evaluateName + childName.substr(name.size());
        }
        return evaluateName + "." + childName;
    };

    if(pIT == nullptr)
    {
        return content;
    }

    switch(pIT->getType())
    {
        case types::InternalType::ScilabList:
        {
            types::List* pList = pIT->getAs<types::List>();
            for(int i = 0; i < pList->getSize(); ++i)
            {
                std::string var_name = name + "(" + std::to_string(i+1) + ")";
                types::InternalType* pITChildren = pList->get(i);
                pITChildren->IncreaseRef();
                content.push_back(addVariable(var_name, childEvaluateName(var_name), pITChildren));
            }

            break;
        }
        case types::InternalType::ScilabCell:
        {
            types::Cell* pCell = pIT->getAs<types::Cell>();
            for(int i = 0; i < pCell->getSize(); ++i)
            {
                std::string var_name = name + "{" + std::to_string(i+1) + "}";
                types::InternalType* pITChildren = pCell->get(i);
                pITChildren->IncreaseRef();
                content.push_back(addVariable(var_name, childEvaluateName(var_name), pITChildren));
            }

            break;
        }
        case types::InternalType::ScilabSingleStruct:
        {
            types::SingleStruct* pSStr = pIT->getAs<types::SingleStruct>();
            types::String* pFields = pSStr->getFieldNames();
            for(int i = 0; i < pFields->getSize(); ++i)
            {
                std::string var_name = scilab::UTF8::toUTF8(pFields->get(i));
                types::InternalType* pITChildren = pSStr->get(pFields->get(i));
                pITChildren->IncreaseRef();
                content.push_back(addVariable(var_name, childEvaluateName(var_name), pITChildren));
            }
            break;
        }
        case types::InternalType::ScilabStruct:
        {
            types::Struct* pStr = pIT->getAs<types::Struct>();
            if(pStr->isScalar())
            {
                types::String* pFields = pStr->getFieldNames();
                types::SingleStruct* pSStr = pStr->get(0);
                for(int i = 0; i < pFields->getSize(); ++i)
                {
                    std::string var_name = scilab::UTF8::toUTF8(pFields->get(i));
                    types::InternalType* pITChildren = pSStr->get(pFields->get(i));
                    pITChildren->IncreaseRef();
                    content.push_back(addVariable(var_name, childEvaluateName(var_name), pITChildren));
                }
            }
            else
            {
                int* piIndexes = new int[pStr->getDims()];
                for (int i = 0; i < pStr->getSize(); ++i)
                {
                    pStr->getIndexes(i, piIndexes);
                    std::string var_name = name + "(" + std::to_string(piIndexes[0]+1);
                    for(int idx = 1; idx < pStr->getDims(); ++idx)
                    {
                        var_name += ", "+std::to_string(piIndexes[idx]+1);
                    }
                    var_name += ")";

                    types::InternalType* pITChildren = pStr->get(i);
                    pITChildren->IncreaseRef();
                    content.push_back(addVariable(var_name, childEvaluateName(var_name), pITChildren));
                }
                delete[] piIndexes;
            }
            break;
        }
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            types::TList* pList = pIT->getAs<types::TList>();
            types::GenericType* pFields = nullptr;
            int firstFieldIdx = 0;
            types::Callable::ReturnValue Ret = types::Callable::Error;

            // try calling overload to get fieldnames
            types::typed_list in;
            types::typed_list out;
            std::wstring stType = pList->getShortTypeStr();
            std::wstring wstrFuncName = L"%" + stType + L"_fieldnames";

            pList->IncreaseRef();
            in.push_back(pList);
            try
            {
                Ret = Overload::call(wstrFuncName.c_str(), in, 1, out, false, false);
            }
            catch(const std::exception&)
            {
                // error in overload, continue to get original fieldnames
            }

            pList->DecreaseRef();
            if(Ret == types::Callable::OK && out.size() == 1)
            {
                if(out[0]->isString() || out[0]->isDouble())
                {
                    pFields = out[0]->getAs<types::GenericType>();
                }
                else
                {
                    out[0]->killMe();
                }
            }

            if(pFields == nullptr)
            {
                pFields = pList->getFieldNames();
                // first field is the list type
                firstFieldIdx = 1;
            }

            if(pFields == nullptr)
            {
                // no fields or type not managed
                break;
            }

            for(int i = firstFieldIdx; i < pFields->getSize(); ++i)
            {
                types::typed_list in;
                types::typed_list out;
                types::optional_list opt;

                types::Double* pDblIdx = new types::Double(i+1);
                in.push_back(pDblIdx);
                // can be a string, double, int
                types::InternalType* pField = pFields->extract(&in);
                pDblIdx->killMe();
                in.clear();
                if(pField == nullptr)
                {
                    continue;
                }

                in.push_back(pField);
                pField->IncreaseRef();
                //int iRet = false;
                try
                {
                    /*iRet =*/ pList->invoke(in, opt, 1, out, ast::CommentExp(Location(), new std::wstring(L"")));
                    if(out.size() != 1)
                    {
                        throw;
                    }
                    out[0]->IncreaseRef();
                }
                catch(const std::exception&)
                {
                    pField->DecreaseRef();
                    pField->killMe();
                    continue;
                }

                std::string var_name = "";
                if(pField->isString())
                {
                    var_name = scilab::UTF8::toUTF8(pField->getAs<types::String>()->get(0));
                }
                else
                {
                    var_name = name + "(" + std::to_string(i+1) + ")";
                }

                pField->DecreaseRef();
                pField->killMe();

                content.push_back(addVariable(var_name, childEvaluateName(var_name), out[0]));
            }

            pFields->killMe();
            break;
        }
        case types::InternalType::ScilabObject:
        {
            types::Object* pObj = pIT->getAs<types::Object>();
            types::String* pFields = pObj->getFields();
            for(int i = 0; i < pFields->getSize(); ++i)
            {
                std::string var_name = scilab::UTF8::toUTF8(pFields->get(i));
                types::InternalType* pITChildren = nullptr;
                pObj->extract(pFields->get(i), pITChildren);
                pITChildren->IncreaseRef();
                content.push_back(addVariable(var_name, childEvaluateName(var_name), pITChildren));
            }

            break;
        }
        default: {}
    }

    return content;
}

int DapEndpoint::addVariable(const std::string& name, const std::string& evaluateName, types::InternalType* value)
{
    const int ref = m_next_variable_ref++;
    VariableEntry& entry = m_variables[ref];
    entry.name = name;
    entry.evaluateName = evaluateName;
    entry.value = value;
    return ref;
}

int DapEndpoint::addScope(const std::string& name, VAR_LIST_KIND kind, int level)
{
    const int ref = addVariable(name, std::string(), nullptr);
    m_variables[ref].scopeKind = kind;
    m_variables[ref].scopeLevel = level;
    return ref;
}

const std::vector<int>& DapEndpoint::getChildren(int _iRef)
{
    static const std::vector<int> none;
    const auto found = m_variables.find(_iRef);
    if(found == m_variables.end())
    {
        return none;
    }

    // Building adds entries: std::map keeps this one where it is meanwhile.
    VariableEntry& entry = found->second;
    if(entry.childrenBuilt == false)
    {
        entry.childrenBuilt = true;
        if(entry.value != nullptr)
        {
            entry.children = getContainerValues(_iRef);
        }
        else if(entry.scopeKind == CALLSTACK)
        {
            entry.children = getCallStackScopes();
        }
        else
        {
            entry.children = getVariableList(entry.scopeKind, entry.scopeLevel);
        }
    }

    return entry.children;
}

json DapEndpoint::getVariable(int _iRef, const std::string& valueField)
{
    const auto found = m_variables.find(_iRef);
    if(found == m_variables.end())
    {
        return json::object();
    }

    if(found->second.value == nullptr)
    {
        // A group: one frame of the call stack, expandable like a container.
        return json::object({{"name", found->second.name}, {valueField, ""}, {"variablesReference", _iRef}});
    }

    const std::string name = found->second.name;
    types::InternalType* pIT = found->second.value;

    std::string type = scilab::UTF8::toUTF8(pIT->getTypeStr());
    json obj = json::object({
        {"name", name},
        // Only a value with children gets a reference, as DAP requires.
        {"variablesReference", 0},
        {"type", type},
        {valueField, type}
    });
    if(found->second.evaluateName.empty() == false)
    {
        obj["evaluateName"] = found->second.evaluateName;
    }

    switch(pIT->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            obj["type"] = "double"; // instead of constant
        }
        case types::InternalType::ScilabInt8:
        case types::InternalType::ScilabInt16:
        case types::InternalType::ScilabInt32:
        case types::InternalType::ScilabInt64:
        case types::InternalType::ScilabUInt8:
        case types::InternalType::ScilabUInt16:
        case types::InternalType::ScilabUInt32:
        case types::InternalType::ScilabUInt64:
        case types::InternalType::ScilabBool:
        case types::InternalType::ScilabString:
        {
            obj[valueField] = getVariableDisplayValue(pIT);
            break;
        }
        case types::InternalType::ScilabStruct:
        {
            obj[valueField] = getStructuredVariableSummary(pIT, false);
            if(obj[valueField] != "empty struct")
            {
                obj["variablesReference"] = _iRef;
            }
            break;
        }
        case types::InternalType::ScilabSingleStruct:
        {
            obj[valueField] = getStructuredVariableSummary(pIT, false);
            obj["variablesReference"] = _iRef;
            break;
        }
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            obj[valueField] = getStructuredVariableSummary(pIT, false);
            obj["variablesReference"] = _iRef;
            break;
        }
        case types::InternalType::ScilabList:
        {
            obj[valueField] = getStructuredVariableSummary(pIT, false);
            if(obj[valueField] != "empty list")
            {
                obj["variablesReference"] = _iRef;
            }
            break;
        }
        case types::InternalType::ScilabCell:
        {
            obj[valueField] = getStructuredVariableSummary(pIT, false);
            if(obj[valueField] != "empty cell")
            {
                obj["variablesReference"] = _iRef;
            }
            break;
        }
        case types::InternalType::ScilabFunction:
        {
            obj[valueField] = scilab::UTF8::toUTF8(pIT->getAs<types::Function>()->getName());
            break;
        }
        case types::InternalType::ScilabMacro:
        case types::InternalType::ScilabMacroFile:
        {
            std::wostringstream os;
            pIT->toString(os);
            pIT->clearPrintState();
            std::string tmp(scilab::UTF8::toUTF8(os.str()));
            tmp.insert(tmp.find("=") + 1, name);
            obj[valueField] = tmp;
            break;
        }
        case types::InternalType::ScilabObject:
        {
            obj["variablesReference"] = _iRef;
            break;
        }
        default: {};
    }

    return obj;
}

/*
 * The value under the mouse, or of the selection, read without executing
 * anything. While stopped, in the selected frame: the evaluator reads the scope
 * execution is stopped in, so another frame only resolves a plain name, as the
 * "local" group of that frame shows it. While idle, in the global scope. The
 * runtime read lock is only granted when the interpreter is idle or paused.
 */
void DapEndpoint::evaluateForHover(json& resp, const json& args)
{
    RuntimeTryLockGuard runtimeGuard;
    if(m_is_running || runtimeGuard.ownsLock() == false)
    {
        resp["success"] = false;
        resp["message"] = "notStopped"; // value predefined in dap spec
        return;
    }

    const std::string expression = args.value("expression", std::string());
    const int frameId = args.value("frameId", -1);
    debugger::DebuggerManager::CallStack cs = debugger::DebuggerManager::getInstance()->getCallStack();
    bool stoppedFrame = true;
    for(size_t row = 1; row < cs.stack.size(); ++row)
    {
        if(cs.stack[row].scope == frameId && cs.stack[row].scope != cs.stack[0].scope)
        {
            stoppedFrame = false;
            break;
        }
    }

    types::InternalType* pIT = nullptr;
    if(stoppedFrame)
    {
        if(eval(expression, &pIT).empty() == false)
        {
            pIT = nullptr;
        }
    }
    else if(symbol::Context::getInstance()->isValidVariableName(expression.data()))
    {
        pIT = symbol::Context::getInstance()->getAtLevel(symbol::Symbol(scilab::UTF8::toWide(expression)), frameId);
    }

    // A function is described by the language server, not given a value here.
    if(pIT == nullptr || pIT->isCallable())
    {
        resp["success"] = false;
        resp["body"]["error"] = { {"format", "no value"} };
        return;
    }

    pIT->IncreaseRef();
    resp["body"] = getVariable(addVariable(expression, expression, pIT), "result");
}

/*
 * The complete value of one variable, as Scilab code or as JSON. It is found by
 * its container and name, which is exact whatever the frame, else by its own
 * reference, else by evaluating the expression of a watch.
 */
void DapEndpoint::getFullValue(json& resp, const json& args)
{
    if(m_is_running)
    {
        resp["success"] = false;
        resp["message"] = "notStopped"; // value predefined in dap spec
        return;
    }

    int ref = 0;
    const int container = args.value("containerReference", 0);
    const std::string wanted = args.value("name", std::string());
    if(container > 0 && wanted.empty() == false)
    {
        for(int child : getChildren(container))
        {
            if(m_variables[child].name == wanted)
            {
                ref = child;
                break;
            }
        }
    }

    if(ref == 0)
    {
        // The former request sent the negative reference of a plain value.
        long long given = args.value("variablesReference", 0LL);
        given = given < 0 ? -given : given;
        if(m_variables.find(static_cast<int>(given)) != m_variables.end())
        {
            ref = static_cast<int>(given);
        }
    }

    if(ref == 0)
    {
        const std::string expression = args.value("expression", std::string());
        types::InternalType* pIT = nullptr;
        if(expression.empty() == false && eval(expression, &pIT).empty() && pIT != nullptr)
        {
            pIT->IncreaseRef();
            ref = addVariable(expression, expression, pIT);
        }
    }

    const auto found = m_variables.find(ref);
    if(found == m_variables.end() || found->second.value == nullptr)
    {
        resp["success"] = false;
        resp["message"] = "cancelled"; // value predefined in dap spec
        return;
    }

    // The expression, when there is one, names the value where it lives: saving
    // "s.field = ..." back updates the field, not a new variable.
    const std::string var_name = found->second.evaluateName.empty() ? found->second.name : found->second.evaluateName;
    types::InternalType* pIT = found->second.value;
    const std::string format = args.value("format", std::string("scilab"));
    resp["body"]["name"] = var_name;

    sendProgressStart("Scilab_Variable", "Loading Scilab variable: " + var_name);

    if(format == "scilab")
    {
        types::typed_list in;
        types::typed_list out;
        pIT->IncreaseRef();
        in.push_back(pIT);
        types::String* pName = new types::String(var_name.data());
        in.push_back(pName);
        pName->IncreaseRef();
        try
        {
            Overload::call(L"sci2exp", in, 1, out, false, false);
            types::String* pStrOut = out[0]->getAs<types::String>();
            std::string content = "";
            for(int i = 0; i < pStrOut->getSize(); i++)
            {
                content += scilab::UTF8::toUTF8(pStrOut->get(i));
                content += "\n";
            }

            if(pIT->isMacro())
            {
                content = var_name + " = " + content;
            }

            resp["body"]["result"] = content;
            pStrOut->killMe();
        }
        catch(const ast::InternalError& ie)
        {
            resp["body"]["error"] = scilab::UTF8::toUTF8(ie.GetErrorMessage());
        }
        catch(...)
        {
            resp["body"]["error"] = "Fail to open variable.";
        }

        pIT->DecreaseRef();
        pName->DecreaseRef();
        pName->killMe();
    }
    else if(format == "json")
    {
        std::string err;
        // toJSON(pIT, err, indent, allowNanAndInf);
        std::string json_str = toJSON(pIT, err, 4, false);
        if(err.empty() == false)
        {
            resp["body"]["error"] = err;
        }
        else
        {
            resp["body"]["result"] = json_str;
        }
    }
    else
    {
        resp["success"] = false;
        resp["message"] = "cancelled"; // value predefined in dap spec
    }

    sendProgressEnd("Scilab_Variable");
}

void DapEndpoint::execDebuggerDone()
{
#ifdef DAP_LOG
    std::cout << "[DapServer] execution done" << std::endl;
#endif

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    if(manager->isInterrupted() == false)
    {
        if(m_keep_alive)
        {
            m_execution_done = true;
            onStop(-1, "", true);
        }
        else if(ConfigVariable::getPauseLevel() == 0)
        {
            json terminated = event("terminated");
            broadcastMessage(terminated.dump());
        }
    }
}

char* DapEndpoint::startRun(const std::string& command, bool debug)
{
    if(debug)
    {
        return debugger::DebuggerManager::getInstance()->execute(command);
    }

    // Not a matter for the debugger: a plain command, that the session is
    // told about here. Its end is reported like the one of any command.
    char* error = checkCommand(command.data());
    if(error)
    {
        return error;
    }

    executionStarted();
    StoreCommand(command.data());
    return nullptr;
}

bool DapEndpoint::isBusy(bool debug)
{
    // a debugged execution, running or stopped
    if(debugger::DebuggerManager::isDebugging())
    {
        return true;
    }

    // A debug run needs Scilab at rest. A normal one is a command like any
    // other: it can be run from a pause, and waits behind what is running.
    return debug && (m_is_running || StaticRunner_isRunning());
}

void DapEndpoint::updateDebugHold()
{
    // The SciNotes windows hold debug mode together, as one session: any of
    // them takes or gives it, and it lasts until the last one leaves.
    bool sciNotesAttached = false;
    bool held = false;
    for(const auto& client : m_clients)
    {
        sciNotesAttached = sciNotesAttached || (client->sessionActive && client->sharedSession);
        held = held || (client->sessionActive && client->holdsDebugMode);
    }

    if(sciNotesAttached == false)
    {
        m_scinotes_holds_debug = false;
    }

    debugger::DebuggerManager::holdDebugMode(debugger::DebuggerManager::ClientHolder,
                                              held || m_scinotes_holds_debug);
    publishDebugMode();
}

void DapEndpoint::publishDebugMode()
{
    // Called by the interpreter thread too, when debug() takes or leaves
    // debug mode: never wait for a request being handled, it may be waiting
    // for the interpreter itself.
    std::unique_lock<std::recursive_mutex> lock(m_clients_mutex, std::try_to_lock);
    if(lock.owns_lock() == false)
    {
        m_debug_mode_to_publish = true;
        return;
    }

    m_debug_mode_to_publish = false;
    const bool held = debugger::DebuggerManager::isDebugModeHeld();
    if(held == m_published_debug_mode)
    {
        return;
    }

    m_published_debug_mode = held;
    json debugMode = event("scilabDebugMode");
    debugMode["body"] = { {"debugMode", held} };
    broadcastMessage(debugMode.dump());
}

void DapEndpoint::ensureDebugger()
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    if(manager->hasDebugger("DAP") == false)
    {
        manager->addDebugger("DAP", new Debugger());
    }
}

bool DapEndpoint::joinSession(ClientContext& client, const json& arguments, bool launch, json& resp)
{
    if(client.initialized == false)
    {
        resp["success"] = false;
        resp["message"] = "cancelled";
        resp["body"]["error"]["format"] = "initialize must complete before launch or attach";
        return false;
    }

    std::string requestedSessionId;
    if(arguments.contains("scilabSessionId") && arguments["scilabSessionId"].is_string())
    {
        requestedSessionId = arguments["scilabSessionId"].get<std::string>();
    }

    if(requestedSessionId.empty() == false
       && (launch || client.transport != ClientContext::Transport::WebSocket))
    {
        resp["success"] = false;
        resp["message"] = "cancelled";
        resp["body"]["error"]["format"] = "scilabSessionId is only supported for SciNotes WebSocket attach sessions";
        return false;
    }

    /*
     * One Scilab holds one debugger, so there is one debug session at a time -
     * but every client is welcome in the one that exists. Several SciNotes
     * views share it, a VS Code connection joins what SciNotes started, and the
     * panes of a SciNotes opened by a launch join that launch. Only a second
     * logical session, named by a different scilabSessionId, is refused.
     */
    if(requestedSessionId.empty() == false
       && m_active_session_id.empty() == false
       && m_active_session_id != requestedSessionId)
    {
        resp["success"] = false;
        resp["message"] = "cancelled";
        resp["body"]["error"]["format"] = "another debug session already controls Scilab";
        return false;
    }

    if(m_session_owner == nullptr)
    {
        m_active_session_id = requestedSessionId;
        m_session_owner = &client;
        m_standard_session_owner = requestedSessionId.empty() ? &client : nullptr;
        m_session_configured = false;
        m_launched = false;
        m_attached = false;
        m_stopped_on_error = false;
        m_execution_done = false;
    }
    else if(m_active_session_id.empty() && requestedSessionId.empty() == false)
    {
        // A view naming its session joins one that had no name: keep the name,
        // so its siblings recognise the session they belong to.
        m_active_session_id = requestedSessionId;
    }

    client.sharedSession = requestedSessionId.empty() == false;
    client.logicalSessionId = requestedSessionId;
    client.sessionActive = true;
    return true;
}

/*
 * A participant leaving takes nothing with it: the session belongs to the
 * client that opened it, and the others are only watching.
 */
void DapEndpoint::detachClient(ClientContext& client)
{
    dropClientBreakpoints(client);
    client.sessionActive = false;
    client.sharedSession = false;
    client.holdsDebugMode = false;
    client.logicalSessionId.clear();
    updateDebugHold();
}

/*
 * The owner left. A launch session dies with it, since the Scilab process was
 * started for it; an attach session is handed to another client still watching,
 * and only closes when nobody is left.
 */
void DapEndpoint::releaseSessionOwner()
{
    m_session_owner = nullptr;
    if(m_launched == false)
    {
        for(const auto& candidate : m_clients)
        {
            if(candidate->sessionActive)
            {
                m_session_owner = candidate.get();
                return;
            }
        }
    }

    endSession(m_launched);
}

/*
 * The SciNotes windows of one Scilab share a scilabSessionId: they are one
 * application, and any of them stops the run the others are showing. A client
 * of another application only ever leaves.
 */
bool DapEndpoint::canEndSession(const ClientContext& client) const
{
    if(m_session_owner == &client)
    {
        return true;
    }

    return m_session_owner != nullptr
           && client.sharedSession
           && m_session_owner->sharedSession
           && client.logicalSessionId == m_session_owner->logicalSessionId;
}

void DapEndpoint::shutdown()
{
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);

    // No new client, and no debugger callback once the module is gone. Only a
    // started server can be stopped: CppServer asserts otherwise, and one of
    // the two transports is usually unused.
    if(m_server && m_server->IsStarted())
    {
        m_server->Stop();
    }

    if(m_ws_server && m_ws_server->IsStarted())
    {
        m_ws_server->Stop();
    }

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    manager->lockBreakpoints();
    debugger::Breakpoints& allBreakpoints = manager->getAllBreakPoint();
    for(const auto& source : m_breakpoints)
    {
        for(const InstalledBreakpoint& entry : source.second)
        {
            if(entry.bp && std::find(allBreakpoints.begin(), allBreakpoints.end(), entry.bp) != allBreakpoints.end())
            {
                manager->removeBreakPoint(entry.bp);
            }
        }
    }
    m_breakpoints.clear();
    m_desired_breakpoints.clear();
    manager->unlockBreakpoints();

    manager->removeDebugger("DAP");

    clearVariables();

    for(const auto& client : m_clients)
    {
        client->sessionActive = false;
    }
    m_clients.clear();

    m_active_session_id.clear();
    m_standard_session_owner = nullptr;
    m_session_owner = nullptr;
    m_last_stopped_event.clear();
    m_session_configured = false;
    m_launched = false;
    m_attached = false;
    m_is_running = false;
}

void DapEndpoint::endSession(bool terminateScilab)
{
    // Ignore debugger callbacks while tearing the one global debug session down.
    m_is_running = true;
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    clearBreakpoints();
    manager->removeDebugger("DAP");
    manager->abort();
    clearVariables();

    json terminated = event("terminated");
    broadcastMessage(terminated.dump());
    for(const auto& client : m_clients)
    {
        client->holdsDebugMode = false;
        if(client->sessionActive)
        {
            client->sessionActive = false;
            client->sharedSession = false;
            client->logicalSessionId.clear();
        }
    }

    // ending the session is leaving debug mode, for what the clients hold of it
    m_scinotes_holds_debug = false;
    updateDebugHold();

    m_active_session_id.clear();
    m_standard_session_owner = nullptr;
    m_session_owner = nullptr;
    m_last_stopped_event.clear();
    m_session_configured = false;
    m_launched = false;
    m_attached = false;
    m_stop_on_entry = false;
    m_keep_alive = false;
    m_is_running = false;
    m_stopped_on_error = false;
    m_execution_done = false;
    m_debug_mode = true;
    m_file_to_execute.clear();
    m_working_directory.clear();

    if(terminateScilab)
    {
        StoreCommand("[_,__err__]=lasterror();exit(__err__);");
    }
}

void DapEndpoint::clearBreakpoints()
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    manager->lockBreakpoints();
    debugger::Breakpoints& allBreakpoints = manager->getAllBreakPoint();
    for(const auto& source : m_breakpoints)
    {
        for(const InstalledBreakpoint& entry : source.second)
        {
            if(entry.bp && std::find(allBreakpoints.begin(), allBreakpoints.end(), entry.bp) != allBreakpoints.end())
            {
                manager->removeBreakPoint(entry.bp);
            }
        }
    }
    m_breakpoints.clear();
    m_desired_breakpoints.clear();
    manager->unlockBreakpoints();
}

/*
 * Install what the clients want for one source: the union of their lists. A
 * line is created when the first client asks for it and removed when the last
 * one drops it, and the other clients are told either way, since a breakpoint
 * appearing in their gutter is not something they asked for themselves.
 */
void DapEndpoint::resolveBreakpoints(const std::string& filename, const ClientContext* requester)
{
    std::vector<DesiredBreakpoint> wanted;
    for(const auto& perClient : m_desired_breakpoints)
    {
        const auto source = perClient.second.find(filename);
        if(source == perClient.second.end())
        {
            continue;
        }

        for(const DesiredBreakpoint& desired : source->second)
        {
            if(std::find(wanted.begin(), wanted.end(), desired) == wanted.end())
            {
                wanted.push_back(desired);
            }
        }
    }

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    manager->lockBreakpoints();
    debugger::Breakpoints& allBreakpoints = manager->getAllBreakPoint();
    std::vector<InstalledBreakpoint>& installed = m_breakpoints[filename];

    for(auto entry = installed.begin(); entry != installed.end(); )
    {
        if(std::find(wanted.begin(), wanted.end(), entry->desired) != wanted.end())
        {
            ++entry;
            continue;
        }

        if(entry->bp && std::find(allBreakpoints.begin(), allBreakpoints.end(), entry->bp) != allBreakpoints.end())
        {
            manager->removeBreakPoint(entry->bp);
        }

        if(entry->verified)
        {
            notifyBreakpointChange("removed", filename, *entry, requester);
        }
        entry = installed.erase(entry);
    }

    const bool sourceExists = FileExist(filename.data());
    for(const DesiredBreakpoint& desired : wanted)
    {
        bool known = false;
        for(const InstalledBreakpoint& entry : installed)
        {
            if(entry.desired == desired)
            {
                known = true;
                break;
            }
        }

        if(known)
        {
            continue;
        }

        InstalledBreakpoint entry;
        entry.desired = desired;
        entry.id = m_next_breakpoint_id++;
        if(sourceExists)
        {
            debugger::Breakpoint* bp = new debugger::Breakpoint();
            bp->setFileName(filename);
            bp->setFileLine(desired.line);
            if(desired.condition.empty() == false)
            {
                bp->setCondition(desired.condition);
            }

            if(desired.hitCondition.empty() == false)
            {
                // Checked by isHitCount() before it reached the list.
                bp->setHitCondition(std::stoi(desired.hitCondition));
            }

            if(manager->addBreakPoint(bp))
            {
                entry.bp = bp;
            }
            else
            {
                // An identical breakpoint already belongs to Scilab or another
                // debugger. It is effective, simply not ours to remove.
                delete bp;
            }

            entry.verified = true;
        }

        installed.push_back(entry);
        if(entry.verified)
        {
            notifyBreakpointChange("new", filename, entry, requester);
        }
    }

    if(installed.empty())
    {
        m_breakpoints.erase(filename);
    }

    manager->unlockBreakpoints();
}

/*
 * A client leaving takes its own lines with it, and only those: what another
 * client still wants stays installed.
 */
void DapEndpoint::dropClientBreakpoints(const ClientContext& client)
{
    const auto owned = m_desired_breakpoints.find(&client);
    if(owned == m_desired_breakpoints.end())
    {
        return;
    }

    std::vector<std::string> sources;
    for(const auto& source : owned->second)
    {
        sources.push_back(source.first);
    }

    m_desired_breakpoints.erase(owned);
    for(const std::string& filename : sources)
    {
        resolveBreakpoints(filename, &client);
    }
}

void DapEndpoint::notifyBreakpointChange(const std::string& reason, const std::string& filename,
                                         const InstalledBreakpoint& entry, const ClientContext* except)
{
    json changed = event("breakpoint");
    changed["body"]["reason"] = reason;
    changed["body"]["breakpoint"] = {
        {"id", entry.id},
        {"verified", entry.verified},
        {"line", entry.desired.line},
        {"source", {{ "path", filename }}}
    };

    broadcastMessage(changed.dump(), except);
}

/*
 * A client joining a session missed every breakpoint event that came before it.
 * Announce what is installed and does not belong to it, so its gutter shows the
 * lines that will actually stop the execution.
 */
void DapEndpoint::replayBreakpoints(ClientContext& client)
{
    if(client.sessionActive == false)
    {
        return;
    }

    const auto owned = m_desired_breakpoints.find(&client);
    for(const auto& source : m_breakpoints)
    {
        for(const InstalledBreakpoint& entry : source.second)
        {
            if(entry.verified == false)
            {
                continue;
            }

            if(owned != m_desired_breakpoints.end())
            {
                const auto wanted = owned->second.find(source.first);
                if(wanted != owned->second.end()
                   && std::find(wanted->second.begin(), wanted->second.end(), entry.desired) != wanted->second.end())
                {
                    // Its own line: it already knows about that one.
                    continue;
                }
            }

            json changed = event("breakpoint");
            changed["body"]["reason"] = "new";
            changed["body"]["breakpoint"] = {
                {"id", entry.id},
                {"verified", entry.verified},
                {"line", entry.desired.line},
                {"source", {{ "path", source.first }}}
            };
            sendMessage(client, changed.dump());
        }
    }
}

void DapEndpoint::sendProgressStart(const std::string& progressId, const std::string& title)
{
    json progress = event("progressStart");
    progress["body"]["progressId"] = progressId;
    progress["body"]["title"] = title;
    broadcastMessage(progress.dump());
}

void DapEndpoint::sendProgressEnd(const std::string& progressId)
{
    json progress = event("progressEnd");
    progress["body"]["progressId"] = progressId;
    broadcastMessage(progress.dump());
}

void DapEndpoint::sendMessage(ClientContext& client, const std::string& msg)
{
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
#ifdef DAP_LOG
    std::cout << "[DapServer] Send message:" << std::endl << msg << std::endl;
#endif

    // Never let a malformed message reach json::parse's exception: it would
    // travel up the interpreter thread and terminate Scilab.
    json payload = json::parse(msg, nullptr, false);
    if(payload.is_discarded())
    {
        std::cout << "[DapServer] dropped an unparsable message" << std::endl;
        return;
    }

    payload["seq"] = client.sequence++;
    if(client.transport == ClientContext::Transport::WebSocket)
    {
        if(client.wsSession)
        {
            client.wsSession->SendTextMessage(payload.dump());
        }
        return;
    }

    if(client.tcpSession)
    {
        std::string encoded;
        json_rpc_encode(payload.dump(), encoded);
        client.tcpSession->Send(encoded);
    }
}

void DapEndpoint::broadcastMessage(const std::string& msg, const ClientContext* except)
{
    std::lock_guard<std::recursive_mutex> lock(m_clients_mutex);
    json payload = json::parse(msg, nullptr, false);
    if(payload.is_discarded())
    {
        std::cout << "[DapServer] dropped an unparsable event" << std::endl;
        return;
    }

    const std::string eventName = payload.value("event", "");

    auto supportsEvent = [&eventName, except](const ClientContext& client) {
        if(client.sessionActive == false || &client == except)
        {
            return false;
        }
        if((eventName == "progressStart" || eventName == "progressUpdate" || eventName == "progressEnd") &&
           client.supportsProgressReporting == false)
        {
            return false;
        }
        if(eventName == "invalidated" && client.supportsInvalidatedEvent == false)
        {
            return false;
        }
        return true;
    };

    for(const auto& client : m_clients)
    {
        if(supportsEvent(*client))
        {
            sendMessage(*client, msg);
        }
    }
}

void DapEndpoint::output(const std::string text)
{
    json console = event("output");
    console["body"] = {
        {"category", "stdout"},
        {"output", text}
    };
    broadcastMessage(console.dump());
}
void DapEndpoint::error(const std::string text)
{
    json console = event("output");
    console["body"] = {
        {"category", "stderr"},
        {"output", text}
    };
    broadcastMessage(console.dump());
}

void DapEndpoint::setRunning(bool _bRunning)
{
    m_is_running = _bRunning;
}

bool DapEndpoint::isRunning()
{
    return m_is_running;
}

void DapEndpoint::clearVariables()
{
    // Kill internalType created using overloads.
    // When an internalType is killed, it will kill all its children,
    // because this list hold containers and its children killing containers before
    // childrens result to a double free on childrens.
    // Reverse the loop is used to call the killMe on childrens that are created always
    // after the holding container.
    // Childrens will not be killed here because of the ref > 0 but will be killed at parent killMe.
    for(auto it = m_variables.rbegin(); it != m_variables.rend(); ++it)
    {
        if(it->second.value != nullptr)
        {
            it->second.value->DecreaseRef();
            it->second.value->killMe();
        }
    }
    m_variables.clear();
    m_frame_scopes.clear();
}


/*** internal ***/
json response(const json _query)
{
    json obj = R"({"type": "response"})"_json;
    obj["request_seq"] = _query["seq"];
    obj["command"] = _query["command"];
    obj["success"] = true;

    return obj;
}

json event(const std::string& _event)
{
    json obj = R"({"type": "event"})"_json;
    obj["event"] = _event;

    return obj;
}
