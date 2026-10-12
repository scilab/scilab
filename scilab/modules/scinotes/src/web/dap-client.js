/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

/*
 * DAP-over-WebSocket client joining the shared SciNotes debug session.
 *
 * It owns the transport, the standard handshake, the session and execution
 * control states, and reconnection. It knows nothing of the page showing the
 * session: an editor or a debug view reacts through the hooks and the events
 * it is given, which are dispatched after the client has updated its own state.
 */

/* Texts of the client itself; a page hands over the translated ones. */
var DAP_CLIENT_TEXTS = {
    stopped: "Execution paused",
    stoppedOnError: "Execution paused on an error",
    pausedOnBreakpoint: "Paused on breakpoint",
    paused: "Execution paused",
    running: "Execution running",
    completed: "Execution completed",
    connectionLost: "Connection to the debugger lost",
    connectionFailed: "Connection to the debugger failed",
    noResponse: "The debugger did not respond to %s"
};

/*
 * @param options
 *   url, sessionId              the endpoint and the logical session to join
 *   clientID, clientName        sent with initialize
 *   text(key)                   translated text, DAP_CLIENT_TEXTS otherwise
 *   debug                       object filled with a diagnostic snapshot
 *   beforeConfigurationDone()   the page's breakpoints, as a promise
 *   onSessionState(state)       before the control state is published
 *   onControlState(values)      what the controls may do now
 */
function createDapClient(options) {
    var socket = null;
    var sequence = 1;
    // Counts the stops and resumes: a stack answer belongs to one stop only.
    var runStateGeneration = 0;
    var pendingRequests = {};
    var eventListeners = {};
    var initializeAnswered = false;
    var initializedEventReceived = false;
    var attached = false;
    var configured = false;
    var disposed = false;
    var reconnectTimer = null;
    var reconnectDelay = 250;
    var reconnectAttempts = 0;
    var maxReconnectAttempts = 120;
    var sessionState = "disconnected";
    // Whether Scilab stays in debug mode while nothing runs.
    var debugMode = false;
    var stopReason = null;
    var pendingControl = null;
    var controlTimer = null;
    var stateMessage = "";
    var debug = options.debug || {};

    updateDebug({ status: sessionState });

    function text(key) {
        return options.text && options.text(key) || DAP_CLIENT_TEXTS[key] || "";
    }

    function updateDebug(values) {
        Object.keys(values).forEach(function(name) {
            debug[name] = values[name];
        });
    }

    /*
     * Told by the reason of the stop, not by its description: that one is the
     * adapter's English text. An error keeps Scilab's own message.
     */
    function stoppedMessage(body) {
        var reason = body && body.reason;
        if (reason === "exception") {
            return body.text || text("stoppedOnError");
        }
        if (reason === "breakpoint") {
            return text("pausedOnBreakpoint");
        }
        return text(reason === "pause" ? "paused" : "stopped");
    }

    function publishControlState() {
        var ready = configured && attached && !pendingControl;
        var stopped = sessionState === "stopped";
        var running = sessionState === "running";
        var exception = stopReason === "exception";
        var values = {
            state: sessionState,
            pending: pendingControl || "",
            // A stop is debug mode whoever started the run.
            debugMode: debugMode || stopped,
            stoppedOnError: stopped && exception,
            canContinue: ready && stopped,
            canPause: ready && running && debugMode,
            canStepOver: ready && stopped && !exception,
            canStepIn: ready && stopped && !exception,
            canStepOut: ready && stopped && !exception,
            canDisconnect: ready && (debugMode || stopped),
            /*
             * Whether Scilab is free to start a run. Deliberately not tied to
             * the session being ready: without a session SciNotes still runs
             * files its own way, and its controls must stay usable. A debug
             * run needs Scilab at rest; a run without the debugger is only
             * kept out of a debugged execution, running or stopped.
             */
            canDebug: !running && !stopped,
            canExecute: !stopped && !(running && debugMode),
            sessionReady: ready,
            message: stateMessage
        };
        updateDebug({
            status: sessionState,
            pendingControl: pendingControl,
            controlAvailability: values
        });
        if (options.onControlState) {
            options.onControlState(values);
        }
    }

    function setSessionState(nextState, message) {
        sessionState = nextState;
        stateMessage = message || "";
        if (nextState !== "stopped") {
            stopReason = null;
            updateDebug({ stopReason: null, stopDescription: null, stopText: null });
        }
        if (options.onSessionState) {
            options.onSessionState(nextState);
        }
        publishControlState();
    }

    function finishControlTransition(error) {
        if (controlTimer) {
            clearTimeout(controlTimer);
            controlTimer = null;
        }
        pendingControl = null;
        if (error) {
            stateMessage = error.message || String(error);
            updateDebug({ lastError: stateMessage });
        }
        publishControlState();
    }

    function controlArguments(command) {
        if (command === "disconnect") {
            return { terminateDebuggee: false };
        }
        return { threadId: 1, singleThread: false };
    }

    function isControlEnabled(command) {
        if (!configured || !attached || pendingControl) {
            return false;
        }
        if (command === "disconnect") {
            return debugMode || sessionState === "stopped";
        }
        if (command === "pause") {
            return sessionState === "running" && debugMode;
        }
        if (command === "continue") {
            return sessionState === "stopped";
        }
        return sessionState === "stopped"
            && stopReason !== "exception"
            && (command === "next" || command === "stepIn" || command === "stepOut");
    }

    function runControl(command) {
        if (!isControlEnabled(command)) {
            return false;
        }

        pendingControl = command;
        stateMessage = "";
        publishControlState();
        controlTimer = setTimeout(function() {
            controlTimer = null;
            if (pendingControl === command) {
                finishControlTransition(new Error(text("noResponse").replace("%s", command)));
            }
        }, 5000);
        sendRequest(command, controlArguments(command)).catch(function(error) {
            if (pendingControl === command) {
                finishControlTransition(error);
            }
        });
        return true;
    }

    function rejectPending(reason) {
        Object.keys(pendingRequests).forEach(function(id) {
            pendingRequests[id].reject(new Error(reason));
            delete pendingRequests[id];
        });
    }

    function sendRequest(command, args) {
        return new Promise(function(resolve, reject) {
            if (!socket || socket.readyState !== WebSocket.OPEN) {
                reject(new Error("DAP socket is not open"));
                return;
            }

            var requestSequence = sequence++;
            pendingRequests[requestSequence] = {
                command: command,
                resolve: resolve,
                reject: reject
            };
            socket.send(JSON.stringify({
                seq: requestSequence,
                type: "request",
                command: command,
                arguments: args || {}
            }));
        });
    }

    /*
     * Run a file in the attached Scilab, in debug mode or not. The response only
     * says the request was accepted: the run itself is followed through
     * `continued`, `stopped` and `terminated`, like any other execution of the
     * session.
     */
    function execFile(sourcePath, echo, debug) {
        if (!configured || !attached || !sourcePath) {
            return false;
        }

        sendRequest("scilabExecFile", { file: sourcePath, echo: echo === true, debug: debug === true })
            .catch(function(error) {
                updateDebug({ lastError: "scilabExecFile: " + error.message });
                stateMessage = error.message;
                publishControlState();
            });
        return true;
    }

    function emitEvent(message) {
        var listeners = eventListeners[message.event] || [];
        listeners.slice().forEach(function(listener) {
            listener(message.body || {});
        });
    }

    function completeHandshake() {
        if (!initializeAnswered || !initializedEventReceived || attached || disposed) {
            return;
        }

        attached = true;
        var attachedState = {};
        setSessionState("configuring");
        sendRequest("attach", {
            scilabSessionId: options.sessionId,
            keepAlive: true
        }).then(function(body) {
            attachedState = body || {};
            /*
             * Ours before configurationDone: the session then knows which lines
             * belong to this client, and the ones it replays as installed are
             * really the other clients'.
             */
            return options.beforeConfigurationDone ? options.beforeConfigurationDone() : null;
        }).then(function() {
            return sendRequest("configurationDone", {});
        }).then(function() {
            configured = true;
            debugMode = attachedState.scilabDebugMode === true;
            reconnectAttempts = 0;
            reconnectDelay = 250;
            updateDebug({
                attached: true,
                configured: true,
                terminated: false,
                lastError: null,
                running: attachedState.scilabRunning === true,
                stopped: false
            });
            if (attachedState.scilabRunning === true) {
                setSessionState("running", text("running"));
            } else {
                setSessionState("idle");
            }
        }).catch(function(error) {
            if (disposed) {
                return;
            }
            attached = false;
            configured = false;
            updateDebug({ attached: false, configured: false, lastError: error.message });
            setSessionState("failed", error.message);
            if (socket) {
                socket.close();
            }
        });
    }

    function handleMessage(event) {
        var message;
        try {
            message = JSON.parse(event.data);
        } catch (error) {
            updateDebug({ lastError: "Invalid DAP JSON: " + error.message });
            return;
        }

        updateDebug({ lastInbound: message, lastUpdate: new Date().toISOString() });
        if (message.type === "response") {
            var pending = pendingRequests[message.request_seq];
            if (!pending) {
                return;
            }
            delete pendingRequests[message.request_seq];
            if (message.success === false) {
                pending.reject(new Error(message.body && message.body.error && message.body.error.format || message.message || "DAP request failed"));
            } else {
                pending.resolve(message.body || {});
            }
            return;
        }

        if (message.type === "event") {
            var body = message.body || {};
            if (message.event === "stopped" || message.event === "continued" || message.event === "terminated") {
                runStateGeneration++;
            }
            if (message.event === "initialized") {
                initializedEventReceived = true;
                completeHandshake();
            } else if (message.event === "stopped") {
                finishControlTransition();
                stopReason = body.scilabExecutionDone ? null : body.reason || null;
                updateDebug({
                    running: false,
                    stopped: !body.scilabExecutionDone,
                    stopReason: stopReason,
                    stopDescription: body.description || null,
                    stopText: body.text || null
                });
                if (body.scilabExecutionDone) {
                    setSessionState("idle", text("completed"));
                } else {
                    setSessionState("stopped", stoppedMessage(body));
                }
            } else if (message.event === "continued") {
                finishControlTransition();
                updateDebug({ running: true, stopped: false });
                setSessionState("running", text("running"));
            } else if (message.event === "scilabDebugMode") {
                debugMode = body.debugMode === true;
                publishControlState();
            } else if (message.event === "terminated") {
                finishControlTransition();
                attached = false;
                configured = false;
                debugMode = false;
                updateDebug({ running: false, stopped: false, attached: false, configured: false, terminated: true });
                setSessionState("disconnected");
            }
            emitEvent(message);
            // Stop ended the logical session, not this connection: join the next one.
            if (message.event === "terminated" && !disposed && socket) {
                completeHandshake();
            }
        }
    }

    function scheduleReconnect() {
        if (disposed || reconnectTimer || reconnectAttempts >= maxReconnectAttempts) {
            return;
        }
        reconnectAttempts++;
        reconnectTimer = setTimeout(function() {
            reconnectTimer = null;
            reconnectDelay = Math.min(reconnectDelay * 2, 2000);
            connect();
        }, reconnectDelay);
    }

    function connect() {
        if (disposed || !options.url || !options.sessionId) {
            return;
        }

        initializeAnswered = false;
        initializedEventReceived = false;
        attached = false;
        configured = false;
        setSessionState("connecting");
        socket = new WebSocket(options.url);
        socket.onopen = function() {
            setSessionState("configuring");
            updateDebug({ connected: true, attached: false, configured: false, reconnectAttempts: reconnectAttempts });
            sendRequest("initialize", {
                adapterID: "scilab",
                clientID: options.clientID,
                clientName: options.clientName,
                linesStartAt1: true,
                columnsStartAt1: true,
                pathFormat: "path",
                supportsProgressReporting: true,
                supportsInvalidatedEvent: true
            }).then(function(capabilities) {
                initializeAnswered = true;
                updateDebug({ capabilities: capabilities });
                completeHandshake();
            }).catch(function(error) {
                if (disposed) {
                    return;
                }
                updateDebug({ lastError: error.message });
                socket.close();
            });
        };
        socket.onmessage = handleMessage;
        socket.onerror = function() {
            if (disposed) {
                return;
            }
            attached = false;
            configured = false;
            finishControlTransition();
            updateDebug({ connected: false, attached: false, configured: false, lastError: "DAP WebSocket error" });
            setSessionState("failed", text("connectionFailed"));
        };
        socket.onclose = function() {
            socket = null;
            attached = false;
            configured = false;
            debugMode = false;
            finishControlTransition();
            rejectPending("DAP socket closed");
            updateDebug({ connected: false, attached: false, configured: false });
            setSessionState("disconnected", disposed ? "" : text("connectionLost"));
            scheduleReconnect();
        };
    }

    return {
        start: connect,
        request: sendRequest,
        command: runControl,
        execFile: execFile,
        publishControlState: publishControlState,
        /* setBreakpoints is accepted from attach on, before configurationDone. */
        isAttached: function() {
            return attached;
        },
        getSessionState: function() {
            return sessionState;
        },
        isDebugMode: function() {
            return debugMode;
        },
        getRunStateGeneration: function() {
            return runStateGeneration;
        },
        updateDebug: updateDebug,
        on: function(eventName, listener) {
            eventListeners[eventName] = eventListeners[eventName] || [];
            eventListeners[eventName].push(listener);
            return {
                dispose: function() {
                    var listeners = eventListeners[eventName] || [];
                    var index = listeners.indexOf(listener);
                    if (index >= 0) {
                        listeners.splice(index, 1);
                    }
                }
            };
        },
        dispose: function() {
            disposed = true;
            if (reconnectTimer) {
                clearTimeout(reconnectTimer);
                reconnectTimer = null;
            }
            rejectPending("DAP client disposed");
            finishControlTransition();
            attached = false;
            configured = false;
            setSessionState("disconnected");
            eventListeners = {};
            if (socket) {
                socket.close();
                socket = null;
            }
        }
    };
}
