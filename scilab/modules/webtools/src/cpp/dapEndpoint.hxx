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

#include <atomic>
#include <sstream>
#include <string>
#include <map>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
    SCOPE,
    // Not a scope of its own: one group per frame of the call stack, so the
    // same name can be read in several scopes without changing frame.
    CALLSTACK
};

namespace debugger
{
struct Breakpoint;
}

class DapEndpoint
{
    struct ClientContext
    {
        enum class Transport
        {
            Tcp,
            WebSocket
        };

        Transport transport;
        int sequence = 1;
        bool supportsProgressReporting = false;
        bool supportsInvalidatedEvent = false;
        bool initialized = false;
        bool sessionActive = false;
        bool sharedSession = false;
        // Scilab stays in debug mode as long as this client is attached.
        bool holdsDebugMode = false;
        std::string logicalSessionId;
        std::string incoming;
        std::shared_ptr<SciTcpSession> tcpSession;
        std::shared_ptr<SciWsSession> wsSession;
    };

    /*
     * Breakpoints belong to the client that asked for them, and Scilab runs the
     * union: a line stays installed while any client still wants it, so a
     * second editor joining never removes the lines of the first.
     */
    struct DesiredBreakpoint
    {
        int line = 0;
        std::string condition;
        std::string hitCondition;

        bool operator==(const DesiredBreakpoint& other) const
        {
            return line == other.line
                && condition == other.condition
                && hitCondition == other.hitCondition;
        }
    };

    struct InstalledBreakpoint
    {
        DesiredBreakpoint desired;
        // Null when Scilab already owned an identical breakpoint.
        debugger::Breakpoint* bp = nullptr;
        int id = 0;
        bool verified = false;
    };


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

    void onStop(int bp_idx, const std::string& fileOrFuncName = "", bool executionDone = false);
    /* Hand a client that just joined the stop the session is already on. */
    void replayStoppedState(ClientContext& client);
    /* And the breakpoints the others installed before it arrived. */
    void replayBreakpoints(ClientContext& client);
    /* Take one client out of the session without touching the others. */
    void detachClient(ClientContext& client);
    /* Install the union of what every client wants for one source. */
    void resolveBreakpoints(const std::string& filename, const ClientContext* requester);
    /* Forget what one client wanted, and let the others keep theirs. */
    void dropClientBreakpoints(const ClientContext& client);
    /* Tell the other clients about a line they did not ask for themselves. */
    void notifyBreakpointChange(const std::string& reason, const std::string& filename,
                                const InstalledBreakpoint& entry, const ClientContext* except);
    /* The owner is gone: pass the session on, or close it. */
    void releaseSessionOwner();
    /* Whether this client may end the session for everybody. */
    bool canEndSession(const ClientContext& client) const;
    void onResume();
    void executionStarted();
    void execDebuggerDone();
    // Tell the clients whether Scilab is in debug mode while nothing runs.
    void publishDebugMode();
    void sendOutput(const std::string& category, const std::string& text);
    void flushPendingOutput();

    void setRunning(bool _bRunning);
    bool isRunning();

    std::vector<int> getVariableList(VAR_LIST_KIND kind, int _iLevel = -1);
    /* One group per frame below the current one, for the call stack group. */
    std::vector<int> getCallStackScopes();
    json getVariable(int _iRef, const std::string& valueField = "value");
    std::vector<int> getContainerValues(int _iRef);
    /* The children of a scope group or a container, built once per stop. */
    const std::vector<int>& getChildren(int _iRef);
    /* Store a value; the store owns the reference the caller increased. */
    int addVariable(const std::string& name, const std::string& evaluateName, types::InternalType* value);
    int addScope(const std::string& name, VAR_LIST_KIND kind, int level);
    void getFullValue(json& resp, const json& args);
    void evaluateForHover(json& resp, const json& args);

private:
    DapEndpoint();
    ~DapEndpoint();

    void setSpawned(bool _spawned);
    void onConnected(std::shared_ptr<CppServer::Asio::TCPSession>& session);
    void onDisconnected(std::shared_ptr<CppServer::Asio::TCPSession>& session);
    void onTcpMessage(const std::shared_ptr<ClientContext>& client, const std::vector<uint8_t>& msg);
    void onError(int code, const std::string& category, const std::string& message);
    void onWsConnected(const std::shared_ptr<SciWsSession>& session);
    void onWsDisconnected(const std::shared_ptr<SciWsSession>& session);
    void onWsMessage(const std::shared_ptr<ClientContext>& client, const std::vector<uint8_t>& msg);
    void onWsError(int code, const std::string& category, const std::string& message);
    void manageMessage(ClientContext& client, const std::string& message);
    void sendMessage(ClientContext& client, const std::string& msg);
    void broadcastMessage(const std::string& msg, const ClientContext* except = nullptr);
    void clearBreakpoints();
    void clearVariables();
    void ensureDebugger();
    // Whether Scilab cannot start a run now, debugged or not.
    bool isBusy(bool debug);
    // Tell the debugger whether a client keeps Scilab in debug mode.
    void updateDebugHold();

    // Start a run, debugged or not. Returns an error message to free, or null.
    char* startRun(const std::string& command, bool debug);
    bool joinSession(ClientContext& client, const json& arguments, bool launch, json& response);
    void endSession(bool terminateScilab);
    void sendProgressStart(const std::string& progressId, const std::string& title);
    void sendProgressEnd(const std::string& progressId);

    std::shared_ptr<SciTcpServer> m_server;
    std::shared_ptr<SciWsServer> m_ws_server;
    std::shared_ptr<CppServer::Asio::Service> m_service;
    std::unordered_set<std::shared_ptr<ClientContext>> m_clients;
    std::recursive_mutex m_clients_mutex;
    std::string m_active_session_id;
    ClientContext* m_standard_session_owner;
    /*
     * The client that opened the active session. It is the only one allowed to
     * choose keepAlive and to end the session; every other client of the same
     * session is a participant that can only detach itself.
     */
    ClientContext* m_session_owner;
    /* The stop the session is on, replayed to a client joining after it. */
    std::string m_last_stopped_event;
    bool m_session_configured;
    std::unordered_map<const ClientContext*,
        std::unordered_map<std::string, std::vector<DesiredBreakpoint>>> m_desired_breakpoints;
    std::unordered_map<std::string, std::vector<InstalledBreakpoint>> m_breakpoints;
    int m_next_breakpoint_id;

    bool m_launched;
    bool m_attached;
    bool m_stop_on_entry;
    bool m_keep_alive;
    bool m_is_running;
    bool m_stopped_on_error;
    bool m_execution_done;
    bool m_debug_mode;
    // The SciNotes session keeps Scilab in debug mode, from a debug run to Stop.
    bool m_scinotes_holds_debug = false;
    bool m_published_debug_mode = false;
    std::atomic<bool> m_debug_mode_to_publish{false};
    // Output written while a request was being handled, sent once it is.
    std::mutex m_pending_output_mutex;
    std::vector<std::string> m_pending_output;
    bool m_spawned;
    std::string m_file_to_execute;
    // The last file run, which a kept-alive session still shows once it is done.
    std::string m_executed_file;
    std::string m_working_directory;
    int m_iPort;

    /*
     * What the clients were shown of the current stop, shared by all of them.
     * A handle is never reused, so a reference kept from an earlier stop finds
     * nothing instead of another value. Cleared when execution resumes.
     */
    struct VariableEntry
    {
        std::string name;
        // The expression giving this value in the current frame, if there is one.
        std::string evaluateName;
        // Null for a scope group, whose children are the variables of a scope.
        types::InternalType* value = nullptr;
        VAR_LIST_KIND scopeKind = LOCAL;
        int scopeLevel = -1;
        bool childrenBuilt = false;
        std::vector<int> children;
    };
    std::map<int, VariableEntry> m_variables;
    // The scope groups of each frame asked for, by frame id.
    std::map<int, std::vector<int>> m_frame_scopes;
    int m_next_variable_ref;

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
        terminate_,
        disconnect,
        // Vendor request, ignored by standard clients: see manageMessage().
        scilabExecFile,
        scilabGetFullValue
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
        DapEndpoint::getInstance().executionStarted();
        // std::cout << "!!!!! debugger onExecution " << std::endl;
    };
    virtual void onPrint(const std::string& /*variable*/){};
    virtual void onShow(int /*bp*/){};
    virtual void onResume(){
        DapEndpoint::getInstance().onResume();
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
    virtual void onDebugMode(bool /*held*/){
        DapEndpoint::getInstance().publishDebugMode();
    };
    virtual void updateBreakpoints(){};
    virtual void onExecutionReleased(){
        // std::cout << "!!!!! debugger onExecutionReleased " << std::endl;
        DapEndpoint::getInstance().setRunning(false);
        DapEndpoint::getInstance().execDebuggerDone();
    };
};

#endif /* !__DAP_ENDPOINT_HXX__ */
