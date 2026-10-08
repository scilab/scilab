package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Async assistant requests: each one runs in a background thread through
 * the {@link AcpClient} of the chosen agent, and its {@link TaskState} is
 * polled by the panel (events, live tokens, final result).
 */
public final class AgentProcess {

    private static final Logger LOG = Logger.getLogger(AgentProcess.class.getName());
    private static final AtomicInteger ID_GEN = new AtomicInteger(0);
    private static final Map<Integer, TaskState> TASKS = new ConcurrentHashMap<>();

    /** Technical log path (TMPDIR/assistant.log), resolved on first use;
     *  null outside a Scilab session (standalone harness): no log. */
    private static volatile String logFilePath = null;
    private static volatile boolean logFileResolved = false;

    static String logFile() {
        if (!logFileResolved) {
            synchronized (AgentProcess.class) {
                if (!logFileResolved) {
                    try {
                        File tmp = org.scilab.modules.commons.ScilabConstants.TMPDIR;
                        if (tmp != null && tmp.isDirectory()) {
                            logFilePath = new File(tmp, "assistant.log").getAbsolutePath();
                        }
                    } catch (Throwable t) {
                        // no Scilab around: keep logging off
                    }
                    logFileResolved = true;
                }
            }
        }
        return logFilePath;
    }

    /** Set the log path without asking Scilab (standalone test harness:
     *  ScilabConstants cannot load outside a Scilab session). */
    static synchronized void setLogFile(String path) {
        logFilePath = path;
        logFileResolved = true;
    }

    private AgentProcess() {
    }

    // -- Public API --

    /**
     * Start a new assistant request asynchronously.
     *
     * @param agent        the ACP agent to use
     * @param prompt       the user prompt
     * @param model        model chosen in the panel ("" for the agent's default)
     * @param effort       effort level chosen in the panel ("" for the default)
     * @param systemPrompt system prompt (empty string for none)
     * @param maxTurns     max tool calls in one request
     * @param resumeSessionId session ID to resume (empty string = new conversation)
     * @param images       base64 data URLs, or null
     * @param workingDir   directory the agent works in; null/empty inherits the
     *                     JVM cwd. Keeps the agent from roaming the Scilab
     *                     source/install tree.
     * @param history      the conversation as shown in the panel, given to
     *                     the agent when its own session cannot be resumed;
     *                     null when not needed
     * @return task ID for polling
     */
    static int start(AcpAgent agent, String prompt, String model,
                     String effort, String systemPrompt, int maxTurns,
                     String resumeSessionId, List<String> images,
                     String workingDir, String history) {
        int id = ID_GEN.incrementAndGet();
        TaskState task = new TaskState();
        TASKS.put(id, task);

        AskContext ctx = context(workingDir);
        ctx.prompt = prompt;
        ctx.model = model != null ? model : "";
        ctx.effort = effort != null ? effort : "";
        ctx.systemPrompt = systemPrompt;
        ctx.maxTurns = maxTurns;
        ctx.resumeSessionId = resumeSessionId;
        ctx.images = images;
        ctx.history = history;

        Thread t = new Thread(() -> run(id, task, agent, ctx));
        t.setDaemon(true);
        t.setName("assistant-task-" + id);
        t.start();

        return id;
    }

    /**
     * Live check of an agent for the settings page (models, efforts,
     * version...). Starts the agent when needed: call from a background
     * thread only.
     */
    static String probe(AcpAgent agent, String workingDir) {
        return AcpClient.forAgent(agent).probe(context(workingDir)).toString();
    }

    /**
     * Sign in to an agent from the settings page (may open a browser page
     * and wait for the user): call from a background thread only.
     * @return null when done, else the error
     */
    static String signIn(AcpAgent agent, String methodId, String workingDir) {
        return AcpClient.forAgent(agent).signInFromSettings(methodId, context(workingDir));
    }

    /** The user's answer to a question of a running request (permission,
     *  sign-in method). */
    static void answer(int id, String questionId, String value) {
        TaskState task = TASKS.get(id);
        java.util.function.BiConsumer<String, String> hook = task != null ? task.answerHook : null;
        if (hook != null) {
            hook.accept(questionId, value);
        }
    }

    /** A request context with the working directory and the MCP endpoints. */
    private static AskContext context(String workingDir) {
        AskContext ctx = new AskContext();
        ctx.workingDir = workingDir;
        try {
            if (McpSseServer.isRunning()) {
                int port = McpSseServer.getPort();
                ctx.mcpSseUrl = "http://127.0.0.1:" + port + "/sse";
                ctx.mcpHttpUrl = "http://127.0.0.1:" + port + "/mcp";
            }
        } catch (Exception e) {
            // no MCP
        }
        return ctx;
    }

    /** Check if a task has completed. */
    static boolean isComplete(int id) {
        TaskState task = TASKS.get(id);
        return task != null && task.complete;
    }

    /** Get the text result of a completed task. Returns "" if not complete. */
    static String getResult(int id) {
        TaskState task = TASKS.get(id);
        if (task == null) {
            return "";
        }
        return task.result;
    }

    /** Get the cost in USD of a completed task. */
    static double getCost(int id) {
        TaskState task = TASKS.get(id);
        if (task == null) {
            return 0.0;
        }
        return task.cost;
    }

    /** Input-side tokens (input + cache creation) of the last turn. */
    static long getTokensIn(int id) {
        TaskState task = TASKS.get(id);
        return task == null ? 0 : task.tokensIn;
    }

    /** Output tokens of the last turn. */
    static long getTokensOut(int id) {
        TaskState task = TASKS.get(id);
        return task == null ? 0 : task.tokensOut;
    }

    /** Output tokens generated so far in the running turn (live counter,
     *  estimated from the streamed characters: ~4 chars/token). */
    static long getLiveTokens(int id) {
        TaskState task = TASKS.get(id);
        return task == null ? 0 : task.liveEstChars / 4;
    }

    /** Get the error message (empty if no error). */
    static String getError(int id) {
        TaskState task = TASKS.get(id);
        if (task == null) {
            return "unknown task";
        }
        return task.isError ? task.errorMessage : "";
    }

    /** Get the session ID for conversation resume. */
    static String getSessionId(int id) {
        TaskState task = TASKS.get(id);
        if (task == null) {
            return "";
        }
        return task.sessionId != null ? task.sessionId : "";
    }

    /** Cancel a running task. */
    static void cancel(int id) {
        TaskState task = TASKS.get(id);
        Runnable hook = task != null ? task.cancelHook : null;
        if (hook != null) {
            hook.run();
        }
    }

    /** Stop every agent (SciNotes closing). */
    static void cancelAll() {
        AcpClient.shutdownAll();
    }

    /** Drain pending activity events (returns JSON array string, clears the list). */
    static String drainEvents(int id) {
        TaskState task = TASKS.get(id);
        if (task == null) {
            return "[]";
        }
        synchronized (task.pendingEvents) {
            if (task.pendingEvents.isEmpty()) {
                return "[]";
            }
            StringBuilder sb = new StringBuilder("[");
            for (int i = 0; i < task.pendingEvents.size(); i++) {
                if (i > 0) {
                    sb.append(",");
                }
                sb.append(task.pendingEvents.get(i));
            }
            sb.append("]");
            task.pendingEvents.clear();
            return sb.toString();
        }
    }

    /** Clean up a completed task. */
    static void cleanup(int id) {
        TASKS.remove(id);
    }

    private static void run(int id, TaskState task, AcpAgent agent, AskContext ctx) {
        try {
            logAppend(logFile(), "[start] task=" + id + " agent=" + agent.id + " prompt=" + ctx.prompt);
            AcpClient.forAgent(agent).runTurn(id, task, ctx);
            logAppend(logFile(), "[done] task=" + id + " cost=" + task.cost + " error=" + task.errorMessage);
        } catch (Exception e) {
            LOG.log(Level.SEVERE, "Assistant request error", e);
            task.isError = true;
            task.errorMessage = String.valueOf(e.getMessage());
            logAppend(logFile(), "[error] task=" + id + " " + e);
        } finally {
            task.complete = true;
        }
    }

    // -- Logging --

    static void logAppend(String logFile, String entry) {
        if (logFile == null || logFile.isEmpty()) {
            return;
        }
        try (PrintWriter pw = new PrintWriter(new FileWriter(logFile, true))) {
            pw.println(entry);
        } catch (Exception e) {
            // silent
        }
    }
}
