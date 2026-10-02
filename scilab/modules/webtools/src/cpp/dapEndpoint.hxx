/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __DAP_ENDPOINT_HXX__
#define __DAP_ENDPOINT_HXX__

#include <sstream>
#include <string>

#include "tcpServer.hxx"
#include "wsServer.hxx"
#include "lsp_dap_tools.hxx"
#include "internal.hxx"
#include "abstractdebugger.hxx"
#include "UTF8.hxx"

enum VAR_LIST_KIND {
    LOCAL,
    GLOBAL,
    VISIBLE,
    SCOPE
};

class DapEndpoint
{
public:
    // avoid the use of assignation of this singleton
    DapEndpoint(const DapEndpoint &) = delete;
    DapEndpoint& operator=(const DapEndpoint &) = delete;
    DapEndpoint(DapEndpoint &&) = delete;
    DapEndpoint & operator=(DapEndpoint &&) = delete;

    // thread safe singleton
    static DapEndpoint& getInstance()
    {
        static DapEndpoint endpoint;
        return endpoint;
    }

    /*
     * Called when Scilab stops, before the quit scripts run: remove the
     * breakpoints and the debugger this endpoint registered, so that neither
     * outlives the endpoint, and stop accepting clients.
     */
    void shutdown();
    void startTcp(int _iPort, bool spawned = false);
    void startWs(int _iPort, const std::string& token = "", bool spawned = false);
    void output(const std::string text);
    void error(const std::string text);

    void onStop(int bp_idx, const std::string& fileOrFuncName = "");
    void execDebuggerDone();

    void setRunning(bool _bRunning);
    bool isRunning();

    json getVariableList(VAR_LIST_KIND kind, int _iLevel = -1);
    json getVariable(size_t _iRef, const std::string& valueField = "value");
    json getContainerValues(size_t _iRef);
    json getVariableValues(size_t _iRef);

private:
    DapEndpoint();
    ~DapEndpoint();

    void setSpawned(bool _spawned);
    void onConnected(std::shared_ptr<CppServer::Asio::TCPSession>& session);
    void onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession>& session);
    void onMessage(const std::vector<uint8_t>& msg);
    void onError(int code, const std::string& category, const std::string& message);
    void onWsConnected(const std::shared_ptr<SciWsSession>& session);
    void onWsDisconnected(const std::shared_ptr<SciWsSession>& session);
    void onWsMessage(const std::vector<uint8_t>& msg);
    void onWsError(int code, const std::string& category, const std::string& message);
    void manageMessage(const std::string& message);
    void sendMessage(const std::string& msg);
    void clearVariables();

    std::shared_ptr<SciTcpServer> m_server;
    std::shared_ptr<SciWsServer> m_ws_server;
    std::shared_ptr<CppServer::Asio::Service> m_service;
    std::shared_ptr<SciTcpSession> m_session;
    std::shared_ptr<SciWsSession> m_ws_session;

    bool m_launched;
    bool m_attached;
    bool m_stop_on_entry;
    bool m_keep_alive;
    bool m_is_running;
    bool m_stopped_on_error;
    bool m_execution_done;
    bool m_debug_mode;
    bool m_spawned;
    std::string m_incoming;
    std::string m_file_to_execute;
    std::string m_working_directory;
    int m_sequence;
    int m_iPort;
    std::vector<std::pair<std::string, types::InternalType*>> m_variables;

public:
    enum Command {
        undefined, // unmanaged command => default switch case
        initialize,
        attach,
        launch,
        setBreakpoints,
        configurationDone,
        threads,
        continue_,
        pause_,
        next,
        stepin,
        stepout,
        stackTrace,
        scopes,
        variables,
        evaluate,
        completions,
        disconnect
    };
};

class Debugger : public debugger::AbstractDebugger
{
public:

    Debugger() {}
    ~Debugger() {}

    virtual void onStop(int index){
        // std::cout << "!!!!! debugger onStop " << index << std::endl;
        DapEndpoint::getInstance().setRunning(false);
        DapEndpoint::getInstance().onStop(index);
    };
    virtual void onExecution(){
        DapEndpoint::getInstance().setRunning(true);
        // std::cout << "!!!!! debugger onExecution " << std::endl;
    };
    virtual void onPrint(const std::string& /*variable*/){};
    virtual void onShow(int /*bp*/){};
    virtual void onResume(){
        DapEndpoint::getInstance().setRunning(true);
        // std::cout << "!!!!! debugger onResume " << std::endl;
    };
    virtual void onAbort(){};
    virtual void onErrorInFile(const std::wstring& filemane){
        // std::cout << "!!!!! debugger onErrorInFile " << std::endl;
        DapEndpoint::getInstance().setRunning(false);
        std::filesystem::path p(filemane);
        DapEndpoint::getInstance().onStop(-1, p.filename().string());
    };
    virtual void onErrorInScript(const std::wstring& funcname){
        DapEndpoint::getInstance().setRunning(false);
        // std::cout << "!!!!! debugger onErrorInScript " << std::endl;
        DapEndpoint::getInstance().onStop(-1, scilab::UTF8::toUTF8(funcname));
    };
    virtual void onQuit(){};
    virtual void updateBreakpoints(){};
    virtual void onExecutionReleased(){
        // std::cout << "!!!!! debugger onExecutionReleased " << std::endl;
        DapEndpoint::getInstance().setRunning(false);
        DapEndpoint::getInstance().execDebuggerDone();
    };
};

#endif /* !__DAP_ENDPOINT_HXX__ */
