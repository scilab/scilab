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

#include <regex>

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

#ifdef _MSC_VER
#include "windows_tools.hxx"
#endif

// #define DAP_LOG

extern "C"
{
    // set command
    #include "storeCommand.h"

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
    { "disconnect", DapEndpoint::disconnect }
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

static json request(const std::string& _command, int _seq);
static json response(const json _query);
static json event(const std::string& _command, int _seq);

DapEndpoint::DapEndpoint(): m_server(nullptr),
    m_launched(false), m_attached(false),
    m_stop_on_entry(false), m_keep_alive(false),
    m_is_running(false), m_stopped_on_error(false),
    m_execution_done(false),m_debug_mode(true),
    m_spawned(false),
    m_incoming(""), m_file_to_execute(""),
    m_sequence(1), m_iPort(-1)
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

void DapEndpoint::shutdown()
{
    // No new client. Only a started server can be stopped: CppServer asserts
    // otherwise, and one of the two transports is usually unused.
    if(m_server && m_server->IsStarted())
    {
        m_server->Stop();
    }

    if(m_ws_server && m_ws_server->IsStarted())
    {
        m_ws_server->Stop();
    }

    // Same teardown as a disconnect, without aborting the execution: Scilab is
    // already leaving its run loop.
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    manager->removeAllBreakPoints();
    if(manager->getDebugger("DAP"))
    {
        delete manager->getDebugger("DAP");
        manager->removeDebugger("DAP");
    }

    clearVariables();
    m_session = nullptr;
    m_ws_session = nullptr;
    m_launched = false;
    m_attached = false;
    m_is_running = false;
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
    m_ws_session = nullptr;
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
    m_session = nullptr;

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
    m_session = std::dynamic_pointer_cast<SciTcpSession>(session);
    m_session->setOnMessage(std::bind(&DapEndpoint::onMessage, this, std::placeholders::_1));

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

    m_ws_session = session;
    m_ws_session->setOnMessage(std::bind(&DapEndpoint::onWsMessage, this, std::placeholders::_1));

    // redirect execution output in a stringstream
    setScilabOutputStreamMethod(DapOutput);
    setScilabErrorStreamMethod(DapError);
}

void DapEndpoint::onWsDisconnected(const std::shared_ptr<SciWsSession>& /*session*/)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] WS client disconnected" << std::endl;
#endif
}

void DapEndpoint::onWsError(int code, const std::string& category, const std::string& message)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] WS error code "<< code <<": " << category << std::endl << message << std::endl;
#endif
}

void DapEndpoint::onMessage(const std::vector<uint8_t>& data)
{
    std::string msg((char*)data.data(), data.size());
#ifdef DAP_LOG
    std::cout << "[DapServer] Receive message: " << msg << std::endl;
#endif
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

void DapEndpoint::onWsMessage(const std::vector<uint8_t>& data)
{
    std::string msg((char*)data.data(), data.size());
#ifdef DAP_LOG
    std::cout << "[DapServer] Receive WS message: " << msg << std::endl;
#endif
    manageMessage(msg);
}

void DapEndpoint::manageMessage(const std::string& msg)
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
            sendMessage(resp.dump());
            return;
        }

        switch(cmd_it->second)
        {
        case initialize:
        {
            m_sequence = 1;

            json resp = response(json_msg);
            resp["body"] = {
                { "supportsConfigurationDoneRequest", true },
                { "supportsCompletionsRequest", true },
                { "supportsConditionalBreakpoints", true },
                { "supportsHitConditionalBreakpoints", true },
                { "completionTriggerCharacters", {"."} },
                { "supportsProgressReporting", true }
            };

            // "supportsEvaluateForHovers": true,
            // "supportsFunctionBreakpoints": true,
            // "supportsDataBreakpoints": true

            sendMessage(resp.dump());

            debugger::DebuggerManager::getInstance()->addDebugger("DAP", new Debugger());

            json initialized = event("initialized", m_sequence++);
            sendMessage(initialized.dump());
        }
        break;
        case attach:
        {
            m_attached = true;
            m_file_to_execute = "";
            m_working_directory = "";
            m_keep_alive = true;
            json resp = response(json_msg);
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

            sendMessage(resp.dump());
        }
        break;
        case launch:
        {
            m_launched = true;
            m_file_to_execute = "";
            m_working_directory = "";
            m_keep_alive = false;
            json resp = response(json_msg);
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

            sendMessage(resp.dump());
        }
        break;
        case setBreakpoints:
        {
            json resp = response(json_msg);
            resp["body"]["breakpoints"] = json::array({});
            debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
            std::string filename = json_msg["arguments"]["source"]["path"];
            char* f = get_full_path(filename.data());
            filename = f;
            FREE(f);

            // first remove breakpoints from the file before add them again
            manager->lockBreakpoints();
            debugger::Breakpoints bps = manager->getAllBreakPoint();
            for(const auto& bp : bps)
            {
                if(bp->getFileName() == filename)
                {
                    manager->removeBreakPoint(bp);
                }
            }

            for(auto breakpoint : json_msg["arguments"]["breakpoints"])
            {
                if (!FileExist(filename.data()))
                {
                    resp["body"]["breakpoints"].push_back({ {"verified", false}, {"reason", "failed"} });
                    continue;
                }

                // Create Scilab breakpoint
                debugger::Breakpoint* bp = new debugger::Breakpoint();
                bp->setFileName(filename);
                bp->setFileLine(breakpoint["line"]);
                if(breakpoint.contains("condition"))
                {
                    bp->setCondition(breakpoint["condition"]);
                }

                if(breakpoint.contains("hitCondition"))
                {
                    try
                    {
                        std::string hitCond = breakpoint["hitCondition"];
                        bp->setHitCondition(std::stoi(hitCond));
                    }
                    catch (...)
                    {
                        std::cout << "Invalid breakpoint hit condition" << std::endl;
                    }
                }

                // Add the breakpoint
                manager->addBreakPoint(bp);

                // fill result response
                resp["body"]["breakpoints"].push_back({ {"verified", true} });
            }

            manager->unlockBreakpoints();
            sendMessage(resp.dump());
        }
        break;
        case configurationDone:
        {
            json resp = response(json_msg);

            // cd to working directory
            if (m_working_directory.empty() == false)
            {
                if(isdir(m_working_directory.c_str()) == false)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = {
                        {"format", std::string("Unable to open workspace directory: ") + m_working_directory}
                    };
                    sendMessage(resp.dump());
                    return;
                }

                int ierr = scichdir(m_working_directory.data());
                if(ierr)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = {
                        {"format", std::string("Failed to set PWD in ") + m_working_directory}
                    };
                    sendMessage(resp.dump());
                    return;
                }
            }

            if(m_debug_mode)
            {
                ConfigVariable::setDefaultVisitor(new ast::DebuggerVisitor());
            }
            else
            {
                ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());
            }

            ConfigVariable::setEnableDebug(m_debug_mode);
            if(m_file_to_execute.empty() == false)
            {
                json progress = event("progressStart", m_sequence++);
                progress["body"]["progressId"] = "Scilab";
                progress["body"]["title"] = "Scilab debug is running";
                sendMessage(progress.dump());

                debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
                if(m_stop_on_entry)
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
                manager->execute(cmd);
            }

            sendMessage(resp.dump());
        }
        break;
        case threads:
        {
            json resp = response(json_msg);
            resp["body"]["threads"] = json::array({});
            resp["body"]["threads"].push_back(json::object({ {"id", 1} , {"name", "scilab"} }));
            sendMessage(resp.dump());
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
                sendMessage(resp.dump());
                return;
            }

            resp["body"]["stackFrames"] = json::array({});
            debugger::DebuggerManager::CallStack cs = debugger::DebuggerManager::getInstance()->getCallStack();

            // set the stack trace
            int iter = 0;
            int max_row = json_msg["arguments"]["levels"];
            for(const auto& stackRow : cs.stack)
            {
                json obj = json::object({
                    {"id", stackRow.scope},
                    {"name", stackRow.functionName},
                    {"line", 0},
                    {"column", 0}
                });

                if(stackRow.hasFile)
                {
                    obj["source"] = {{ "path", stackRow.fileName }};
                    obj["line"] = stackRow.fileLine;
                    obj["column"] = stackRow.column;
                }

                resp["body"]["stackFrames"].push_back(obj);

                if(++iter > max_row)
                {
                    break;
                }
            }

            // keep alive, send information as if we stopped on the last line of the executed file. (variable list)
            if(resp["body"]["stackFrames"].empty() && m_keep_alive && m_file_to_execute.empty() == false)
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
            }

            sendMessage(resp.dump());
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
                sendMessage(resp.dump());
                return;
            }

            resp["body"]["scopes"] = json::array();
            // define scope group
            m_variables.emplace_back("__get_local_variables__", nullptr);
            resp["body"]["scopes"].push_back({{"name", "local"}, {"expensive", false}, {"variablesReference", m_variables.size()}});
            // define global group
            m_variables.emplace_back("__get_global_variables__", nullptr);
            resp["body"]["scopes"].push_back({{"name", "global"}, {"expensive", false}, {"variablesReference", m_variables.size()}});
            // define local group
            m_variables.emplace_back("__get_visible_variables__", nullptr);
            resp["body"]["scopes"].push_back({{"name", "visible"}, {"expensive", false}, {"variablesReference", m_variables.size()}});
            // define callstack group
            m_variables.emplace_back("__get_callstack_variables__", nullptr);
            resp["body"]["scopes"].push_back({{"name", "scope"}, {"expensive", false}, {"variablesReference", m_variables.size()}});
            sendMessage(resp.dump());
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
                sendMessage(resp.dump());
                return;
            }

            size_t iRef = json_msg["arguments"]["variablesReference"];
            if(iRef == 0 || iRef > m_variables.size())
            {
                resp["success"] = false;
                resp["message"] = "cancelled"; // value predefined in dap spec
                sendMessage(resp.dump());
                return;
            }

            const std::string& varname = m_variables[iRef-1].first;
            debugger::DebuggerManager::CallStack cs = debugger::DebuggerManager::getInstance()->getCallStack();
            if(varname == "__get_visible_variables__")
            {
                resp["body"]["variables"] = getVariableList(VISIBLE);
            }
            else if(varname == "__get_local_variables__")
            {
                resp["body"]["variables"] = getVariableList(LOCAL);
            }
            else if(varname == "__get_global_variables__")
            {
                resp["body"]["variables"] = getVariableList(GLOBAL);
            }
            else if(varname == "__get_callstack_variables__")
            {
                int current_scope = cs.stack.empty() ? 1 : cs.stack[0].scope;
                for(const auto& stackRow : cs.stack)
                {
                    // skip current scope because variables values are already available in other local and visible groups.
                    if(stackRow.hasFile && stackRow.scope != current_scope)
                    {
                        m_variables.emplace_back("__get_scope_variables__" + std::to_string(stackRow.scope), nullptr);
                        std::string filename = std::filesystem::path(stackRow.fileName).filename().string();
                        resp["body"]["variables"].push_back({{"name", filename + ":" + std::to_string(stackRow.fileLine)}, {"value", ""}, {"variablesReference", m_variables.size()}});
                    }
                }
            }
            else if (varname.find("__get_scope_variables__") != std::string::npos)
            {
                std::smatch level;
                if(std::regex_search(varname, level, std::regex("\\d+")))
                {
                    int iLevel = atoi(level[0].str().c_str());
                    resp["body"]["variables"] = getVariableList(SCOPE, iLevel);
                }
            }
            else
            {
                resp["body"]["variables"] = getContainerValues(iRef);
            }

            sendMessage(resp.dump());
        }
        break;
        case continue_:
        {
            clearVariables();
            if(m_execution_done)
            {
                m_keep_alive = false;
            }

            if(m_stopped_on_error)
            {
                debugger::DebuggerManager::getInstance()->abort();
                json resp = response(json_msg);
                sendMessage(resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(resp.dump());

            json progress = event("progressStart", m_sequence++);
            progress["body"]["progressId"] = "Scilab";
            progress["body"]["title"] = "Scilab debug is running";
            sendMessage(progress.dump());
        }
        break;
        case next:
        {
            clearVariables();
            if(m_stopped_on_error)
            {
                debugger::DebuggerManager::getInstance()->abort();
                json resp = response(json_msg);
                sendMessage(resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->setStepNext();
            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(resp.dump());

            json progress = event("progressStart", m_sequence++);
            progress["body"]["progressId"] = "Scilab";
            progress["body"]["title"] = "Scilab debug is running";
            sendMessage(progress.dump());
        }
        break;
        case pause_:
        {
            debugger::DebuggerManager::getInstance()->requestPause();
            json resp = response(json_msg);
            sendMessage(resp.dump());
        }
        break;
        case stepin:
        {
            clearVariables();
            if(m_stopped_on_error)
            {
                debugger::DebuggerManager::getInstance()->abort();
                json resp = response(json_msg);
                sendMessage(resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->setStepIn();
            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(resp.dump());

            json progress = event("progressStart", m_sequence++);
            progress["body"]["progressId"] = "Scilab";
            progress["body"]["title"] = "Scilab debug is running";
            sendMessage(progress.dump());
        }
        break;
        case stepout:
        {
            clearVariables();
            if(m_stopped_on_error)
            {
                debugger::DebuggerManager::getInstance()->abort();
                json resp = response(json_msg);
                sendMessage(resp.dump());
                return;
            }

            debugger::DebuggerManager::getInstance()->setStepOut();
            debugger::DebuggerManager::getInstance()->resume(false);
            json resp = response(json_msg);
            sendMessage(resp.dump());

            json progress = event("progressStart", m_sequence++);
            progress["body"]["progressId"] = "Scilab";
            progress["body"]["title"] = "Scilab debug is running";
            sendMessage(progress.dump());
        }
        break;
        case evaluate:
        {
            bool reloadVariables = false;
            json resp = response(json_msg);
            std::string context = json_msg["arguments"]["context"];

            // custom command to get full values of a variable
            if(context == "fullValue")
            {
                long long iSignedRef = json_msg["arguments"]["variablesReference"];
                size_t iRef = static_cast<size_t>(iSignedRef < 0 ? -iSignedRef : iSignedRef);
                if(iRef == 0 || iRef > m_variables.size())
                {
                    resp["success"] = false;
                    resp["message"] = "cancelled";
                    sendMessage(resp.dump());
                    return;
                }

                std::string var_name = m_variables[iRef-1].first;
                resp["body"]["name"] = var_name;

                json progress = event("progressStart", m_sequence++);
                progress["body"]["progressId"] = "Scilab_Variable";
                progress["body"]["title"] = "Loading Scilab variable: " + var_name;
                sendMessage(progress.dump());

                ast::ConstVisitor* current = ConfigVariable::getDefaultVisitor();
                ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());

                std::string format = json_msg["arguments"]["format"];
                if(format == "scilab")
                {
                    types::typed_list in;
                    types::typed_list out;
                    types::InternalType* pIT = m_variables[iRef-1].second;
                    pIT->IncreaseRef();
                    in.push_back(pIT);
                    types::String* pName = new types::String(m_variables[iRef-1].first.data());
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
                        resp["body"]["error"] == "Fail to open variable.";
                    }

                    pIT->DecreaseRef();
                    pName->DecreaseRef();
                    pName->killMe();
                }
                else if(format == "json")
                {
                    std::string err;
                    // toJSON(pIT, err, indent, allowNanAndInf);
                    std::string json_str = toJSON(m_variables[iRef-1].second, err, 4, false);
                    if(err.empty() == false)
                    {
                        ConfigVariable::setDefaultVisitor(current);
                        resp["body"]["error"] = err;
                        progress = event("progressEnd", m_sequence++);
                        progress["body"]["progressId"] = "Scilab_Variable";
                        sendMessage(progress.dump());
                        sendMessage(resp.dump());
                        return;
                    }

                    resp["body"]["result"] = json_str;
                }
                else
                {
                    resp["success"] = false;
                    resp["message"] = "cancelled";
                }

                ConfigVariable::setDefaultVisitor(current);

                sendMessage(resp.dump());

                progress = event("progressEnd", m_sequence++);
                progress["body"]["progressId"] = "Scilab_Variable";
                sendMessage(progress.dump());

                break;
            }

            std::string command = json_msg["arguments"]["expression"];
            if(command == "")
            {
                sendMessage(resp.dump());
                return;
            }

            // not yet managed context: 'hover' | 'clipboard' | 'variables'
            if(context == "watch")
            {
                types::InternalType* pIT = nullptr;
                std::string error = eval(command, &pIT);
                // parsing or execution error
                if(error.empty() == false)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = { {"format", "invalid expression" } };
                    sendMessage(resp.dump());
                    return;
                }

                if (pIT == nullptr)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = { {"format", "invalid read only execution" } };
                    sendMessage(resp.dump());
                    return;
                }

                pIT->IncreaseRef();
                m_variables.emplace_back(command, pIT);
                resp["body"] = getVariable(m_variables.size(), "result");
            }
            else if(context == "repl") // console: Real Eval Print Loop
            {
                if(m_is_running)
                {
                    resp["success"] = false;
                    resp["body"]["error"] = { {"format", "Scilab is running..."} };
                    sendMessage(resp.dump());
                    return;
                }

                // force reload variables and watches for custom evaluate request
                if(json_msg["arguments"].contains("reload"))
                {
                    reloadVariables = json_msg["arguments"]["reload"];
                }

                clearVariables();

                json progress = event("progressStart", m_sequence++);
                progress["body"]["progressId"] = "Scilab";
                progress["body"]["title"] = "Scilab debug is running";
                sendMessage(progress.dump());

                ast::ConstVisitor* current = ConfigVariable::getDefaultVisitor();
                ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());
                StoreConsoleCommand(command.data(), 1);
                ConfigVariable::setDefaultVisitor(current);

                if(reloadVariables == false)
                {
                    appendLineToScilabHistory(command.data());
                }

                progress = event("progressEnd", m_sequence++);
                progress["body"]["progressId"] = "Scilab";
                sendMessage(progress.dump());
            }

            if(reloadVariables)
            {
                json reload = event("invalidated", m_sequence++);
                reload["body"] = R"({"areas": ["variables","watches"]})"_json;
                sendMessage(reload.dump());
            }

            sendMessage(resp.dump());
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

            sendMessage(resp.dump());
        }
        break;
        case disconnect:
        {
            // skip messages sent when debugger stop
            // and ask for information after a disconnect message
            m_is_running = true;

            debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
            manager->removeAllBreakPoints();
            if(manager->getDebugger("DAP"))
            {
                delete manager->getDebugger("DAP");
                // remove debugger before abort to avoid send message to vscode
                manager->removeDebugger("DAP");
            }

            manager->abort();
            ConfigVariable::setEnableDebug(false);
            ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());

            if(m_launched)
            {
                StoreCommand("[_,__err__]=lasterror();exit(__err__);");
            }

            json resp = response(json_msg);
            sendMessage(resp.dump());
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

void DapEndpoint::onStop(int bp_idx, const std::string& fileOrFuncName)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] execution stop:" << std::endl;
#endif

    json stopped = event("stopped", m_sequence++);
    if(bp_idx < 0)
    {
        stopped["body"] = json::object({
            {"threadId", 1},
            {"allThreadsStopped", true}
        });

        if(fileOrFuncName.empty())
        {
            stopped["body"]["reason"] = "pause";
            stopped["body"]["description"] = "Execution paused";
        }
        else
        {
            stopped["body"]["reason"] = "exception";
            stopped["body"]["description"] = "Stopped on Error";
            stopped["body"]["text"] = scilab::UTF8::toUTF8(ConfigVariable::getLastErrorMessage());
            // on error, close debug session after continue
            m_keep_alive = false;
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

    sendMessage(stopped.dump());

    json progress = event("progressEnd", m_sequence++);
    progress["body"]["progressId"] = "Scilab";
    sendMessage(progress.dump());
}

json DapEndpoint::getVariableList(VAR_LIST_KIND kind, int iLevel)
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

    //array of var
    json var_array = json::array({});

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
        m_variables.emplace_back(name, pIT);
        json obj = getVariable(m_variables.size());
        var_array.push_back(obj);
    }

    if(iLevel > 0 && kind == SCOPE)
    {
        for(auto v : lstVars)
        {
            delete v;
        }
    }

    return var_array;
}

json DapEndpoint::getContainerValues(size_t _iRef)
{
    json content = json::array({});
    if(_iRef == 0 || _iRef > m_variables.size())
    {
        return content;
    }

    std::string name = m_variables[_iRef-1].first;
    types::InternalType* pIT = m_variables[_iRef-1].second;

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
                m_variables.emplace_back(var_name, pITChildren);
                content.push_back(getVariable(m_variables.size()));
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
                m_variables.emplace_back(var_name, pITChildren);
                content.push_back(getVariable(m_variables.size()));
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
                m_variables.emplace_back(var_name, pITChildren);
                content.push_back(getVariable(m_variables.size()));
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
                    m_variables.emplace_back(var_name, pITChildren);
                    content.push_back(getVariable(m_variables.size()));
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
                    m_variables.emplace_back(var_name, pITChildren);
                    content.push_back(getVariable(m_variables.size()));
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

            // Use exec visitor to avoid stopping in overloads when displaying variables
            ast::ConstVisitor* current = ConfigVariable::getDefaultVisitor();
            ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());

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
                ConfigVariable::setDefaultVisitor(current);
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

                m_variables.emplace_back(var_name, out[0]);
                content.push_back(getVariable(m_variables.size()));
            }

            pFields->killMe();
            ConfigVariable::setDefaultVisitor(current);
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
                m_variables.emplace_back(var_name, pITChildren);
                content.push_back(getVariable(m_variables.size()));
            }

            break;
        }
        default: {}
    }

    return content;
}

json DapEndpoint::getVariableValues(size_t _iRef)
{
    if(_iRef == 0 || _iRef > m_variables.size())
    {
        return "";
    }

    types::InternalType* pIT = m_variables[_iRef-1].second;
    return getVariableDisplayValue(pIT);
}

json DapEndpoint::getVariable(size_t _iRef, const std::string& valueField)
{
    if(_iRef == 0 || _iRef > m_variables.size())
    {
        return json::array({});
    }

    std::string name = m_variables[_iRef-1].first;
    types::InternalType* pIT = m_variables[_iRef-1].second;

    std::string type = scilab::UTF8::toUTF8(pIT->getTypeStr());
    json obj = json::object({
        {"name", name},
        {"variablesReference", -static_cast<long long>(_iRef)},
        {"type", type},
        {valueField, type}
    });

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
            obj[valueField] = getVariableValues(_iRef);
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
            onStop(-1);
            m_execution_done = true;
        }
        else
        {
            json terminated = event("terminated", m_sequence++);
            sendMessage(terminated.dump());
        }
    }
}

void DapEndpoint::sendMessage(const std::string& msg)
{
#ifdef DAP_LOG
    std::cout << "[DapServer] Send message:" << std::endl << msg << std::endl;
#endif

    if (m_ws_session)
    {
        m_ws_session->SendTextMessage(msg);
        return;
    }

    if (m_session)
    {
        std::string encoded;
        json_rpc_encode(msg, encoded);
        m_session->Send(encoded);
        return;
    }
}

void DapEndpoint::output(const std::string text)
{
    json console = event("output", m_sequence++);
    console["body"] = {
        {"category", "stdout"},
        {"output", text}
    };
    sendMessage(console.dump());
}
void DapEndpoint::error(const std::string text)
{
    json console = event("output", m_sequence++);
    console["body"] = {
        {"category", "stderr"},
        {"output", text}
    };
    sendMessage(console.dump());
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
        if(it->second != nullptr)
        {
            it->second->DecreaseRef();
            // std::cout << it->first << " ("<< it->second->getRef() << ")" << std::endl;
            it->second->killMe();
        }
    }
    m_variables.clear();
}


/*** internal ***/
json request(const std::string& _command, int _seq)
{
    json obj = R"({"type": "request"})"_json;
    obj["seq"] = _seq;
    obj["command"] = _command;

    return obj;
}

json response(const json _query)
{
    json obj = R"({"type": "response"})"_json;
    obj["request_seq"] = _query["seq"];
    obj["command"] = _query["command"];
    obj["success"] = true;

    return obj;
}

json event(const std::string& _event, int _seq)
{
    json obj = R"({"type": "event"})"_json;
    obj["seq"] = _seq;
    obj["event"] = _event;

    return obj;
}
