package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

/**
 * The ACP client side of the assistant for one agent: keeps the agent
 * process and its session alive between turns, runs one turn into a
 * {@link TaskState} (the same state the polling API of {@link AgentProcess}
 * reads) and answers the agent's requests.
 *
 * Everything here follows the protocol only: no behaviour depends on which
 * agent is on the other side. An agent that implements ACP poorly is the
 * agent's problem.
 */
final class AcpClient implements AcpConnection.Handler {

    private static final Map<String, AcpClient> CLIENTS = new ConcurrentHashMap<>();

    /** An agent left unused this long is stopped (it restarts on demand). */
    private static final long IDLE_MS = 15 * 60_000;
    private static final java.util.concurrent.ScheduledExecutorService REAPER =
        java.util.concurrent.Executors.newSingleThreadScheduledExecutor(r -> {
            Thread t = new Thread(r, "acp-idle-reaper");
            t.setDaemon(true);
            return t;
        });

    static {
        REAPER.scheduleWithFixedDelay(AcpClient::stopIdleAgents, 1, 1, TimeUnit.MINUTES);
    }

    /** initialize can be slow the first time (npx download, big binaries). */
    private static final long INIT_TIMEOUT_MS = 180_000;
    private static final long SESSION_TIMEOUT_MS = 120_000;
    private static final long OPTION_TIMEOUT_MS = 30_000;
    /** After session/cancel, how long the agent gets to end the turn. */
    private static final long CANCEL_GRACE_MS = 5_000;
    /** ACP error code of session/new when the user must sign in first. */
    private static final int AUTH_REQUIRED = -32000;
    /** authenticate may open a browser page and wait for the user. */
    private static final long AUTH_TIMEOUT_MS = 300_000;
    /** Answer to a sign-in question: the user signed in by other means. */
    private static final String SIGNED_IN = "__signed_in";

    /** Catalogue entry, refreshed on every request (the user may edit it). */
    private volatile AcpAgent agent;
    private volatile AcpConnection conn;
    /** Command, environment and cwd the running process was started with. */
    private String connKey;
    private JsonObject agentCaps = new JsonObject();
    private JsonObject agentInfo = new JsonObject();
    private JsonArray authMethods = new JsonArray();
    /** Last time a turn or a probe used the agent. */
    private volatile long lastUsed;
    /** Last probe answer, given while a turn holds the client. */
    private volatile JsonObject lastProbe;

    private String sessionId;
    /** True while the session has run no turn yet (opened by a probe):
     *  the next new conversation takes it instead of opening another. */
    private boolean sessionFresh;
    private String sessionMcpKey;
    private JsonArray configOptions = new JsonArray();
    private JsonObject models;
    /** Cumulated cost the agent reported for the session (usage_update). */
    private double sessionCost;

    // -- State of the running turn (null task between turns) --
    private volatile TaskState task;
    private volatile boolean replaying;
    private volatile boolean cancelRequested;
    /** True while session/prompt runs: session/cancel can stop it. */
    private volatile boolean promptInFlight;
    private String logFile;
    /** Request id of the running turn, 0 for a probe. */
    private int turnId;
    private String lastMessageId;
    private double costAtTurnStart;
    private int toolCalls;
    private int maxToolCalls;
    private boolean stepLimitHit;
    /** The session of this turn was just created: it knows nothing of the
     *  conversation the panel shows. */
    private boolean newSessionThisTurn;
    private final StringBuilder thinking = new StringBuilder();
    /** Tool calls announced without their input yet: shown at the first update. */
    private final Map<String, JsonObject> unshownToolCalls = new java.util.HashMap<>();
    /** Questions to the user waiting for an answer, by question id. */
    private final Map<String, CompletableFuture<String>> questions = new ConcurrentHashMap<>();
    private final java.util.concurrent.atomic.AtomicInteger questionIds = new java.util.concurrent.atomic.AtomicInteger();

    private AcpClient(AcpAgent agent) {
        this.agent = agent;
    }

    /** The client of an agent, created on first use. When the catalogue
     *  entry changed, ensureConnection restarts the process. */
    static AcpClient forAgent(AcpAgent agent) {
        AcpClient c = CLIENTS.computeIfAbsent(agent.id, k -> new AcpClient(agent));
        c.agent = agent;
        return c;
    }

    /** Stop every agent process (SciNotes closing). */
    static void shutdownAll() {
        for (AcpClient c : CLIENTS.values()) {
            c.cancel();
            c.drop();
        }
        CLIENTS.clear();
    }

    /** Stop one agent and forget its client (agent removed by the user). */
    static void stop(String id) {
        AcpClient c = CLIENTS.remove(id);
        if (c != null) {
            c.cancel();
            c.drop();
        }
    }

    private static void stopIdleAgents() {
        long now = System.currentTimeMillis();
        for (AcpClient c : CLIENTS.values()) {
            if (c.task == null && c.conn != null && now - c.lastUsed > IDLE_MS) {
                AgentProcess.logAppend(c.logFile, "[acp] stopping idle agent " + c.agent.id);
                c.drop();
            }
        }
    }

    // -- Live check (settings page) -------------------------------------------

    /**
     * What the agent offers, for the settings page: starts it when needed
     * and opens a session (kept for the next new conversation, so the first
     * request does not wait for it).
     * @return {"id","found","path"?,"custom_path","version"?,"auth"?,"error"?,
     *          "models":[{"id","label"}]?,"current_model"?,"efforts":[{"id","label"}]?,
     *          "current_effort"?,
     *          "supports_images"?}
     */
    JsonObject probe(AskContext ctx) {
        if (task != null && lastProbe != null) {
            // A turn holds the client: what it offered last time
            return lastProbe;
        }
        synchronized (this) {
            JsonObject o = new JsonObject();
            o.addProperty("id", agent.id);
            String bin = agent.binary();
            String custom = agent.customPath();
            o.addProperty("found", bin != null);
            o.addProperty("custom_path", custom != null ? custom : "");
            if (bin == null) {
                if (custom != null) {
                    o.addProperty("error", "custom path not found: " + custom);
                }
                return o;
            }
            o.addProperty("path", bin);
            logFile = AgentProcess.logFile();
            turnId = 0;
            lastUsed = System.currentTimeMillis();
            try {
                ensureConnection(agent.commandLine(bin), ctx);
                String version = str(agentInfo, "version");
                if (version != null) {
                    String n = str(agentInfo, "title") != null ? str(agentInfo, "title") : str(agentInfo, "name");
                    o.addProperty("version", (n != null ? n + " " : "") + version);
                }
                if (sessionId == null || !sessionMcpKey.equals(mcpKey(ctx))) {
                    newSession(ctx);
                    sessionFresh = true;
                }
                o.addProperty("auth", "ready");
                JsonObject model = option("model");
                if (model != null) {
                    o.add("models", choices(model.get("options")));
                    o.addProperty("current_model", str(model, "currentValue"));
                } else if (models != null) {
                    JsonArray list = new JsonArray();
                    for (JsonElement e : models.getAsJsonArray("availableModels")) {
                        JsonObject m = e.getAsJsonObject();
                        JsonObject c = new JsonObject();
                        c.addProperty("id", str(m, "modelId"));
                        c.addProperty("label", str(m, "name") != null ? str(m, "name") : str(m, "modelId"));
                        list.add(c);
                    }
                    o.add("models", list);
                    o.addProperty("current_model", str(models, "currentModelId"));
                }
                JsonObject effort = option("thought_level");
                if (effort != null) {
                    o.add("efforts", choices(effort.get("options")));
                    o.addProperty("current_effort", str(effort, "currentValue"));
                }
                JsonObject pc = obj(agentCaps, "promptCapabilities");
                o.addProperty("supports_images", pc != null && pc.has("image") && pc.get("image").getAsBoolean());
            } catch (AcpConnection.AcpException e) {
                if (e.code == AUTH_REQUIRED) {
                    o.addProperty("auth", "sign-in required");
                    o.add("auth_methods", signInMethods());
                } else {
                    o.addProperty("error", e.describe());
                }
                if (e.code == 0) {
                    drop();
                }
            }
            lastProbe = o;
            return o;
        }
    }

    /** The values of a select option as [{"id","label"}], groups flattened. */
    private static JsonArray choices(JsonElement options) {
        JsonArray list = new JsonArray();
        if (options == null || !options.isJsonArray()) {
            return list;
        }
        for (JsonElement e : options.getAsJsonArray()) {
            if (!e.isJsonObject()) {
                continue;
            }
            JsonObject o = e.getAsJsonObject();
            if (o.has("options")) {
                list.addAll(choices(o.get("options")));
            } else if (str(o, "value") != null) {
                JsonObject c = new JsonObject();
                c.addProperty("id", str(o, "value"));
                c.addProperty("label", str(o, "name") != null ? str(o, "name") : str(o, "value"));
                list.add(c);
            }
        }
        return list;
    }

    // -- One turn -----------------------------------------------------------

    /** Run one request; fills the task, never throws. */
    synchronized void runTurn(int id, TaskState t, AskContext ctx) {
        logFile = AgentProcess.logFile();
        List<String> cmd = agent.commandLine();
        if (cmd == null) {
            t.isError = true;
            String custom = agent.customPath();
            t.errorMessage = custom != null
                ? agent.name + " not found at the path set in the settings: " + custom
                : agent.name + " is not installed (no " + String.join(" / ", agent.command)
                  + " program found). See the assistant help page to install it.";
            return;
        }
        task = t;
        turnId = id;
        cancelRequested = false;
        lastMessageId = null;
        toolCalls = 0;
        maxToolCalls = ctx.maxTurns > 0 ? ctx.maxTurns : Integer.MAX_VALUE;
        stepLimitHit = false;
        newSessionThisTurn = false;
        thinking.setLength(0);
        unshownToolCalls.clear();
        t.cancelHook = this::cancel;
        t.answerHook = this::answer;
        try {
            ensureConnection(cmd, ctx);
            ensureSession(ctx);
            if (cancelRequested) {
                return;
            }
            applyOptions(ctx);
            t.sessionId = sessionId;
            sessionFresh = false;
            lastUsed = System.currentTimeMillis();
            costAtTurnStart = sessionCost;

            JsonObject p = new JsonObject();
            p.addProperty("sessionId", sessionId);
            p.add("prompt", promptBlocks(ctx));
            promptInFlight = true;
            CompletableFuture<JsonElement> f = live().request("session/prompt", p);
            JsonObject res = waitForTurn(f);
            promptInFlight = false;
            flushThinking();
            endTurn(res);
        } catch (AcpConnection.AcpException e) {
            flushThinking();
            if (!cancelRequested) {
                t.isError = true;
                t.errorMessage = agent.name + ": " + e.describe();
            }
            if (e.code == 0) {
                // Transport failure: start afresh next time
                drop();
            }
            AgentProcess.logAppend(logFile, "[acp] turn failed: " + e.describe());
        } finally {
            promptInFlight = false;
            lastUsed = System.currentTimeMillis();
            t.cancelHook = null;
            t.answerHook = null;
            task = null;
            t.result = t.texts.toString();
        }
    }

    /** Wait for the end of session/prompt, however long the turn takes. */
    private JsonObject waitForTurn(CompletableFuture<JsonElement> f) throws AcpConnection.AcpException {
        long cancelledAt = 0;
        while (true) {
            try {
                JsonElement r = f.get(500, TimeUnit.MILLISECONDS);
                return r.isJsonObject() ? r.getAsJsonObject() : new JsonObject();
            } catch (java.util.concurrent.TimeoutException e) {
                if (cancelRequested) {
                    if (cancelledAt == 0) {
                        cancelledAt = System.currentTimeMillis();
                    } else if (System.currentTimeMillis() - cancelledAt > CANCEL_GRACE_MS) {
                        // The agent ignores session/cancel: stop it
                        drop();
                        throw new AcpConnection.AcpException(0, "cancelled", null);
                    }
                }
            } catch (java.util.concurrent.ExecutionException e) {
                if (e.getCause() instanceof AcpConnection.AcpException) {
                    throw (AcpConnection.AcpException) e.getCause();
                }
                throw new AcpConnection.AcpException(0, String.valueOf(e.getCause()), null);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new AcpConnection.AcpException(0, "interrupted", null);
            }
        }
    }

    private void endTurn(JsonObject res) {
        TaskState t = task;
        String stop = str(res, "stopReason");
        JsonObject usage = obj(res, "usage");
        if (usage != null) {
            // Cache reads left out, as before: the whole history is re-read
            // from cache on every turn and would dwarf the real input
            t.tokensIn = num(usage, "inputTokens") + num(usage, "cachedWriteTokens");
            t.tokensOut = num(usage, "outputTokens");
        }
        t.cost = Math.max(0, sessionCost - costAtTurnStart);
        if (stepLimitHit) {
            t.startNewMessage();
            t.texts.append("_Stopped: the step limit for one request was reached before "
                           + "the task was finished. Reply \"continue\" to resume._");
        } else if ("refusal".equals(stop)) {
            t.startNewMessage();
            t.texts.append("_The agent refused to continue this request._");
        } else if ("max_tokens".equals(stop)) {
            t.startNewMessage();
            t.texts.append("_Stopped: the answer reached the model's output limit._");
        } else if (!"cancelled".equals(stop) && t.texts.length() == 0 && !t.isError) {
            t.isError = true;
            t.errorMessage = agent.name + " produced no answer (stop reason: " + stop + ")";
        }
        AgentProcess.logAppend(logFile, "[acp] turn end stop=" + stop + " tools=" + toolCalls
                               + " cost=" + t.cost);
    }

    // -- Process and session ------------------------------------------------

    private void ensureConnection(List<String> cmd, AskContext ctx) throws AcpConnection.AcpException {
        String cwd = ctx.workingDir != null ? ctx.workingDir : "";
        String key = String.join("\u0001", cmd) + "\u0002" + agent.env + "\u0002" + cwd;
        if (conn != null && conn.isAlive() && key.equals(connKey)) {
            return;
        }
        drop();
        status("Starting " + agent.name + "...");
        try {
            conn = AcpConnection.start(cmd, cwd.isEmpty() ? null : new File(cwd), agent.env, this, logFile);
        } catch (java.io.IOException e) {
            throw new AcpConnection.AcpException(-1, "cannot start " + cmd.get(0) + " (" + e.getMessage() + ")", null);
        }
        connKey = key;
        SessionLogger.command(turnId, cmd, agent.env);

        JsonObject fs = new JsonObject();
        fs.addProperty("readTextFile", false);
        fs.addProperty("writeTextFile", false);
        JsonObject caps = new JsonObject();
        caps.add("fs", fs);
        caps.addProperty("terminal", false);
        JsonObject info = new JsonObject();
        info.addProperty("name", "scilab-scinotes");
        info.addProperty("title", "Scilab SciNotes assistant");
        info.addProperty("version", "1.0");
        JsonObject p = new JsonObject();
        p.addProperty("protocolVersion", 1);
        p.add("clientCapabilities", caps);
        p.add("clientInfo", info);
        JsonElement r = call("initialize", p, INIT_TIMEOUT_MS);
        JsonObject res = r.isJsonObject() ? r.getAsJsonObject() : new JsonObject();
        agentCaps = obj(res, "agentCapabilities") != null ? obj(res, "agentCapabilities") : new JsonObject();
        agentInfo = obj(res, "agentInfo") != null ? obj(res, "agentInfo") : new JsonObject();
        authMethods = res.has("authMethods") && res.get("authMethods").isJsonArray()
                      ? res.getAsJsonArray("authMethods") : new JsonArray();
        sessionId = null;
    }

    private static String mcpKey(AskContext ctx) {
        return ctx.mcpHttpUrl + "|" + ctx.mcpSseUrl;
    }

    private void ensureSession(AskContext ctx) throws AcpConnection.AcpException {
        String mcpKey = mcpKey(ctx);
        String wanted = ctx.resumeSessionId != null ? ctx.resumeSessionId : "";
        boolean sameMcp = sessionId != null && mcpKey.equals(sessionMcpKey);
        if (sameMcp && (wanted.isEmpty() ? sessionFresh : wanted.equals(sessionId))) {
            // Same conversation still alive in this process, or a new one
            // taking the session a probe opened
            newSessionThisTurn = wanted.isEmpty();
            return;
        }
        if (!wanted.isEmpty()) {
            JsonObject sc = obj(agentCaps, "sessionCapabilities");
            boolean canResume = sc != null && sc.has("resume");
            boolean canLoad = agentCaps.has("loadSession") && agentCaps.get("loadSession").getAsBoolean();
            if (canResume || canLoad) {
                JsonObject p = sessionParams(ctx);
                p.addProperty("sessionId", wanted);
                // session/load replays the whole history as updates: not shown
                replaying = !canResume;
                try {
                    JsonElement r = call(canResume ? "session/resume" : "session/load", p, SESSION_TIMEOUT_MS);
                    sessionStarted(wanted, r, mcpKey);
                    return;
                } catch (AcpConnection.AcpException e) {
                    if (e.code == 0) {
                        throw e;
                    }
                    AgentProcess.logAppend(logFile, "[acp] cannot resume " + wanted + ": " + e.describe());
                } finally {
                    replaying = false;
                }
            }
            status("The previous conversation cannot be resumed: starting a new one");
        }
        newSession(ctx);
    }

    private void newSession(AskContext ctx) throws AcpConnection.AcpException {
        JsonElement r;
        try {
            r = call("session/new", sessionParams(ctx), SESSION_TIMEOUT_MS);
        } catch (AcpConnection.AcpException e) {
            if (e.code != AUTH_REQUIRED) {
                throw e;
            }
            if (task == null || !signIn()) {
                // A probe (the settings page offers the sign-in), or the
                // user did not answer
                throw new AcpConnection.AcpException(e.code, signInMessage(), null);
            }
            try {
                r = call("session/new", sessionParams(ctx), SESSION_TIMEOUT_MS);
            } catch (AcpConnection.AcpException again) {
                throw again.code == AUTH_REQUIRED
                    ? new AcpConnection.AcpException(again.code, signInMessage(), null) : again;
            }
        }
        String sid = str(r.isJsonObject() ? r.getAsJsonObject() : new JsonObject(), "sessionId");
        sessionCost = 0;
        sessionStarted(sid, r, mcpKey(ctx));
        newSessionThisTurn = task != null;
    }

    private void sessionStarted(String sid, JsonElement r, String mcpKey) {
        JsonObject res = r.isJsonObject() ? r.getAsJsonObject() : new JsonObject();
        sessionId = sid;
        sessionMcpKey = mcpKey;
        sessionFresh = false;
        configOptions = res.has("configOptions") && res.get("configOptions").isJsonArray()
                        ? res.getAsJsonArray("configOptions") : new JsonArray();
        models = obj(res, "models");
    }

    /**
     * Ask the user how to sign in (a question in the activity feed), then
     * do it. False when the user gave no answer (Stop, end of the turn).
     */
    private boolean signIn() throws AcpConnection.AcpException {
        JsonObject q = new JsonObject();
        q.addProperty("type", "auth");
        q.addProperty("agent", agent.name);
        q.add("methods", signInMethods());
        String method = ask(q);
        if (method == null) {
            return false;
        }
        if (!SIGNED_IN.equals(method)) {
            authenticate(method);
        }
        return true;
    }

    /** The agent's sign-in methods for the UI: [{"id","name","description"?,"terminal"?}]. */
    private JsonArray signInMethods() {
        JsonArray list = new JsonArray();
        for (JsonElement e : authMethods) {
            if (!e.isJsonObject()) {
                continue;
            }
            JsonObject m = e.getAsJsonObject();
            JsonObject c = new JsonObject();
            c.addProperty("id", str(m, "id"));
            c.addProperty("name", str(m, "name") != null ? str(m, "name") : str(m, "id"));
            if (str(m, "description") != null) {
                c.addProperty("description", str(m, "description"));
            }
            // A command the user runs in a terminal (ACP "terminal-auth")
            JsonObject term = obj(obj(m, "_meta"), "terminal-auth");
            if (term != null && str(term, "command") != null) {
                StringBuilder cmd = new StringBuilder(new File(str(term, "command")).getName()
                                                      .replaceFirst("\\.(exe|cmd)$", ""));
                JsonElement args = term.get("args");
                if (args != null && args.isJsonArray()) {
                    for (JsonElement a : args.getAsJsonArray()) {
                        cmd.append(' ').append(a.getAsString());
                    }
                }
                c.addProperty("terminal", cmd.toString());
            }
            list.add(c);
        }
        return list;
    }

    /** Run a sign-in method of the agent (may open a browser page). */
    private void authenticate(String methodId) throws AcpConnection.AcpException {
        status("Signing in to " + agent.name + "...");
        JsonObject p = new JsonObject();
        p.addProperty("methodId", methodId);
        call("authenticate", p, AUTH_TIMEOUT_MS);
    }

    /**
     * Sign in from the settings page: starts the agent when needed, then
     * runs the method. The page probes again afterwards.
     * @return null when done, else the error
     */
    String signInFromSettings(String methodId, AskContext ctx) {
        synchronized (this) {
            logFile = AgentProcess.logFile();
            turnId = 0;
            try {
                List<String> cmd = agent.commandLine();
                if (cmd == null) {
                    return agent.name + " is not installed";
                }
                ensureConnection(cmd, ctx);
                authenticate(methodId);
                // The session opened before signing in (if any) is unusable
                sessionId = null;
                return null;
            } catch (AcpConnection.AcpException e) {
                if (e.code == 0) {
                    drop();
                }
                return e.describe();
            }
        }
    }

    private String signInMessage() {
        StringBuilder sb = new StringBuilder("you must sign in first");
        if (authMethods.size() > 0) {
            sb.append(" (");
            for (int i = 0; i < authMethods.size(); i++) {
                JsonObject m = authMethods.get(i).getAsJsonObject();
                sb.append(i > 0 ? ", " : "").append(str(m, "name") != null ? str(m, "name") : str(m, "id"));
            }
            sb.append(")");
        }
        return sb.append(". Sign in with the agent's own program, then retry.").toString();
    }

    private JsonObject sessionParams(AskContext ctx) {
        JsonObject p = new JsonObject();
        String cwd = ctx.workingDir != null && !ctx.workingDir.isEmpty()
                     ? ctx.workingDir : System.getProperty("user.dir");
        p.addProperty("cwd", new File(cwd).getAbsolutePath());
        p.add("mcpServers", mcpServers(ctx));
        return p;
    }

    /**
     * The scilab MCP server, over the transport the agent supports: HTTP or
     * SSE when it says so, else stdio (the transport every ACP agent must
     * accept) through {@link McpStdioBridge}.
     */
    private JsonArray mcpServers(AskContext ctx) {
        JsonArray list = new JsonArray();
        if (ctx.mcpHttpUrl.isEmpty()) {
            return list;
        }
        JsonObject mc = obj(agentCaps, "mcpCapabilities");
        boolean http = mc != null && mc.has("http") && mc.get("http").getAsBoolean();
        boolean sse = mc != null && mc.has("sse") && mc.get("sse").getAsBoolean();
        JsonObject s = new JsonObject();
        s.addProperty("name", "scilab");
        if (http) {
            s.addProperty("type", "http");
            s.addProperty("url", ctx.mcpHttpUrl);
            s.add("headers", new JsonArray());
        } else if (sse && !ctx.mcpSseUrl.isEmpty()) {
            s.addProperty("type", "sse");
            s.addProperty("url", ctx.mcpSseUrl);
            s.add("headers", new JsonArray());
        } else {
            String jar = bridgeClasspath();
            if (jar == null) {
                status(agent.name + " accepts no HTTP MCP server and the stdio bridge is unavailable: "
                       + "the Scilab tools cannot be used");
                return list;
            }
            String java = System.getProperty("java.home") + File.separator + "bin" + File.separator
                          + (AgentPlatform.isWindows() ? "java.exe" : "java");
            s.addProperty("command", java);
            JsonArray args = new JsonArray();
            args.add("-cp");
            args.add(jar);
            args.add(McpStdioBridge.class.getName());
            args.add(ctx.mcpHttpUrl);
            s.add("args", args);
            s.add("env", new JsonArray());
        }
        list.add(s);
        return list;
    }

    /** Where McpStdioBridge is loaded from (the scinotes jar), or null. */
    private static String bridgeClasspath() {
        try {
            java.security.CodeSource cs = McpStdioBridge.class.getProtectionDomain().getCodeSource();
            return cs != null ? new File(cs.getLocation().toURI()).getAbsolutePath() : null;
        } catch (Exception e) {
            return null;
        }
    }

    /** A session config option by category (or id), null when not offered. */
    private JsonObject option(String category) {
        for (JsonElement e : configOptions) {
            if (!e.isJsonObject()) {
                continue;
            }
            JsonObject o = e.getAsJsonObject();
            if (category.equals(str(o, "category")) || category.equals(str(o, "id"))) {
                return o;
            }
        }
        return null;
    }

    /** A progress line in the activity feed of the running turn. */
    private void status(String message) {
        TaskState t = task;
        if (t != null) {
            t.emitEvent(TaskState.EVT_STATUS, message);
        }
    }

    /**
     * Ask the user a question in the activity feed (an event with a "type"
     * the panel renders, plus the "id" added here) and wait for the answer.
     * Null when the turn ends first (Stop, agent stopped).
     */
    private String ask(JsonObject question) {
        TaskState t = task;
        if (t == null) {
            return null;
        }
        String id = "q" + questionIds.incrementAndGet();
        question.addProperty("id", id);
        CompletableFuture<String> f = new CompletableFuture<>();
        questions.put(id, f);
        t.emitJson(question.toString());
        try {
            while (true) {
                try {
                    return f.get(500, TimeUnit.MILLISECONDS);
                } catch (java.util.concurrent.TimeoutException e) {
                    if (cancelRequested || task != t || conn == null) {
                        return null;
                    }
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return null;
        } catch (java.util.concurrent.ExecutionException e) {
            return null;
        } finally {
            questions.remove(id);
        }
    }

    /** The user's answer to a question (from the panel). */
    private void answer(String questionId, String value) {
        CompletableFuture<String> f = questions.get(questionId);
        if (f != null) {
            f.complete(value);
        }
    }

    /** Model and effort chosen in the UI, through the session config options. */
    private void applyOptions(AskContext ctx) throws AcpConnection.AcpException {
        setOption("model", ctx.model);
        setOption("thought_level", ctx.effort);
    }

    private void setOption(String category, String value) throws AcpConnection.AcpException {
        if (value == null || value.isEmpty()) {
            return;
        }
        JsonObject opt = option(category);
        if (opt != null) {
            if (value.equals(str(opt, "currentValue"))) {
                return;
            }
            if (!offers(opt.get("options"), "value", value)) {
                AgentProcess.logAppend(logFile, "[acp] " + category + " \"" + value + "\" not offered by " + agent.id);
                return;
            }
            JsonObject p = new JsonObject();
            p.addProperty("sessionId", sessionId);
            p.addProperty("configId", str(opt, "id"));
            p.addProperty("value", value);
            JsonElement r = call("session/set_config_option", p, OPTION_TIMEOUT_MS);
            if (r.isJsonObject() && r.getAsJsonObject().has("configOptions")) {
                configOptions = r.getAsJsonObject().getAsJsonArray("configOptions");
            }
        } else if ("model".equals(category) && models != null
                   && !value.equals(str(models, "currentModelId"))
                   && offers(models.get("availableModels"), "modelId", value)) {
            // Older, unstable form of the model selection
            JsonObject p = new JsonObject();
            p.addProperty("sessionId", sessionId);
            p.addProperty("modelId", value);
            call("session/set_model", p, OPTION_TIMEOUT_MS);
            models.addProperty("currentModelId", value);
        }
    }

    /** True when a list of options (possibly grouped) holds this value. */
    private static boolean offers(JsonElement options, String key, String value) {
        if (options == null || !options.isJsonArray()) {
            return false;
        }
        for (JsonElement e : options.getAsJsonArray()) {
            if (!e.isJsonObject()) {
                continue;
            }
            JsonObject o = e.getAsJsonObject();
            if (value.equals(str(o, key)) || offers(o.get("options"), key, value)) {
                return true;
            }
        }
        return false;
    }

    /**
     * The system prompt merged into the user prompt: ACP has no field for
     * it, and the agent's own base prompt must stay (it carries its tool
     * instructions). The wrapper is deliberately imperative: harnesses tend
     * to treat embedded guidance as mere context and answer with code
     * snippets instead of acting through the scilab MCP tools.
     */
    static String withSystemPrompt(String systemPrompt, String prompt) {
        if (systemPrompt == null || systemPrompt.isEmpty()) {
            return prompt;
        }
        return "<system_instructions>\n" + systemPrompt + "\n</system_instructions>\n\n"
               + "The block above is your system prompt for this task: follow it strictly. "
               + "You are operating INSIDE the SciNotes editor with a live Scilab session. "
               + "Do the work through the scilab MCP tools (edit the user's file, execute the "
               + "code, capture figures to verify plots) rather than only writing code "
               + "in your reply.\n\nUser request: " + prompt;
    }

    private JsonArray promptBlocks(AskContext ctx) {
        JsonArray blocks = new JsonArray();
        JsonObject text = new JsonObject();
        text.addProperty("type", "text");
        // The system prompt travels with every request: ACP has no standard
        // field for it, and the editor context in it changes between turns
        String prompt = ctx.prompt;
        if (newSessionThisTurn && ctx.history != null && !ctx.history.trim().isEmpty()) {
            // The agent lost the conversation the user sees (session gone,
            // agent changed): give it the gist
            status("Giving " + agent.name + " the previous messages of this conversation");
            prompt = "<previous_conversation>\n"
                     + "The conversation so far, as shown in the SciNotes panel (you have no other "
                     + "memory of it). Continue from it.\n\n"
                     + ctx.history.trim() + "\n</previous_conversation>\n\n" + prompt;
        }
        text.addProperty("text", withSystemPrompt(ctx.systemPrompt, prompt));
        blocks.add(text);
        JsonObject pc = obj(agentCaps, "promptCapabilities");
        boolean images = pc != null && pc.has("image") && pc.get("image").getAsBoolean();
        if (ctx.images != null) {
            for (String url : ctx.images) {
                // data:image/png;base64,....
                int semi = url.indexOf(';');
                int comma = url.indexOf(',');
                if (!images || !url.startsWith("data:") || semi < 0 || comma < semi) {
                    continue;
                }
                JsonObject img = new JsonObject();
                img.addProperty("type", "image");
                img.addProperty("mimeType", url.substring(5, semi));
                img.addProperty("data", url.substring(comma + 1));
                blocks.add(img);
            }
        }
        return blocks;
    }

    /** Ask the running turn to stop (the UI's Stop button). */
    void cancel() {
        TaskState t = task;
        AcpConnection c = conn;
        if (t == null || c == null) {
            return;
        }
        cancelRequested = true;
        if (promptInFlight && sessionId != null) {
            JsonObject p = new JsonObject();
            p.addProperty("sessionId", sessionId);
            c.notify("session/cancel", p);
        } else {
            // Still starting (initialize, session/new): nothing to cancel
            // in the protocol, stop the process
            drop();
        }
    }

    /** The running connection; fails when it was stopped meanwhile. */
    private AcpConnection live() throws AcpConnection.AcpException {
        AcpConnection c = conn;
        if (c == null) {
            throw new AcpConnection.AcpException(0, "the agent was stopped", null);
        }
        return c;
    }

    private JsonElement call(String method, JsonObject params, long timeoutMs)
        throws AcpConnection.AcpException {
        return live().call(method, params, timeoutMs);
    }

    /** Stop the agent process; the next turn starts a new one. Not
     *  synchronized: must work while a turn holds the client. */
    private void drop() {
        AcpConnection c = conn;
        conn = null;
        connKey = null;
        sessionId = null;
        if (c != null) {
            c.close();
        }
    }

    // -- What the agent sends -----------------------------------------------

    @Override
    public void onNotification(String method, JsonObject params) {
        TaskState t = task;
        if (!"session/update".equals(method) || t == null || replaying) {
            return;
        }
        JsonObject u = obj(params, "update");
        if (u == null) {
            return;
        }
        String kind = str(u, "sessionUpdate");
        if (kind == null) {
            return;
        }
        switch (kind) {
            case "agent_message_chunk": {
                String text = contentText(u);
                if (text.isEmpty()) {
                    break;
                }
                flushThinking();
                String mid = str(u, "messageId");
                if (mid != null && !mid.equals(lastMessageId)) {
                    if (lastMessageId != null) {
                        t.startNewMessage();
                    }
                    lastMessageId = mid;
                }
                t.appendStreamedText(text);
                break;
            }
            case "agent_thought_chunk": {
                String text = contentText(u);
                thinking.append(text);
                t.liveEstChars += text.length();
                break;
            }
            case "tool_call": {
                flushThinking();
                String id = str(u, "toolCallId");
                toolCalls++;
                if (hasInput(u) || id == null) {
                    showToolCall(u);
                } else {
                    unshownToolCalls.put(id, u);
                }
                if (toolCalls > maxToolCalls && !stepLimitHit) {
                    stepLimitHit = true;
                    cancel();
                }
                break;
            }
            case "tool_call_update": {
                String id = str(u, "toolCallId");
                if (id != null && unshownToolCalls.containsKey(id)
                    && (hasInput(u) || !"pending".equals(str(u, "status")))) {
                    // An update carries only the fields that changed: the
                    // title is in the tool_call, the input may come later
                    JsonObject merged = unshownToolCalls.remove(id).deepCopy();
                    for (Map.Entry<String, JsonElement> e : u.entrySet()) {
                        merged.add(e.getKey(), e.getValue());
                    }
                    showToolCall(merged);
                }
                break;
            }
            case "usage_update": {
                JsonObject cost = obj(u, "cost");
                if (cost != null && cost.has("amount")) {
                    sessionCost = cost.get("amount").getAsDouble();
                }
                break;
            }
            default:
                break;
        }
    }

    @Override
    public JsonElement onRequest(String method, JsonObject params) throws AcpConnection.AcpException {
        if ("session/request_permission".equals(method)) {
            return permission(params);
        }
        // fs/* and terminal/* are not offered in clientCapabilities
        throw new AcpConnection.AcpException(AcpConnection.METHOD_NOT_FOUND, "not supported: " + method, null);
    }

    /**
     * The scilab tools (the assistant's job) and the harmless kinds (read,
     * search, think) are allowed at once. Anything else — editing files,
     * running commands, fetching from the web, deleting... — is asked to
     * the user, with the options the agent offers.
     */
    private JsonElement permission(JsonObject params) {
        JsonObject call = obj(params, "toolCall");
        String title = call != null ? str(call, "title") : null;
        String kind = call != null ? str(call, "kind") : null;
        JsonArray options = params.has("options") && params.get("options").isJsonArray()
                            ? params.getAsJsonArray("options") : new JsonArray();
        boolean scilab = ToolLabels.findIn(title) != null;
        boolean harmless = "read".equals(kind) || "search".equals(kind) || "think".equals(kind);
        String optionId;
        if (scilab || harmless) {
            optionId = optionOfKind(options, "allow_once", "allow_always");
        } else {
            JsonObject q = new JsonObject();
            q.addProperty("type", "permission");
            q.addProperty("agent", agent.name);
            q.addProperty("title", title != null ? title : "Tool call");
            if (kind != null) {
                q.addProperty("kind", kind);
            }
            String detail = permissionDetail(call);
            if (detail != null) {
                q.addProperty("detail", detail);
            }
            JsonArray choices = new JsonArray();
            for (JsonElement e : options) {
                JsonObject o = e.getAsJsonObject();
                JsonObject c = new JsonObject();
                c.addProperty("id", str(o, "optionId"));
                c.addProperty("name", str(o, "name") != null ? str(o, "name") : str(o, "optionId"));
                c.addProperty("kind", str(o, "kind"));
                choices.add(c);
            }
            q.add("options", choices);
            optionId = ask(q);
        }
        AgentProcess.logAppend(logFile, "[acp] permission " + title + " kind=" + kind
                               + " -> " + (optionId != null ? optionId : "cancelled"));
        JsonObject outcome = new JsonObject();
        if (optionId == null || cancelRequested) {
            outcome.addProperty("outcome", "cancelled");
        } else {
            outcome.addProperty("outcome", "selected");
            outcome.addProperty("optionId", optionId);
        }
        JsonObject res = new JsonObject();
        res.add("outcome", outcome);
        return res;
    }

    /** The first option of one of these kinds, in this order of preference. */
    private static String optionOfKind(JsonArray options, String... kinds) {
        for (String k : kinds) {
            for (JsonElement e : options) {
                JsonObject o = e.getAsJsonObject();
                if (k.equals(str(o, "kind"))) {
                    return str(o, "optionId");
                }
            }
        }
        return null;
    }

    /** What a permission is about, shown under its title: the command,
     *  the file or the URL when the input names one, else the raw input. */
    private static String permissionDetail(JsonObject call) {
        if (call == null) {
            return null;
        }
        JsonObject in = obj(call, "rawInput");
        if (in != null) {
            for (String k : new String[] {"command", "cmd", "file_path", "path", "url", "query"}) {
                if (in.has(k) && in.get(k).isJsonPrimitive()) {
                    return in.get(k).getAsString();
                }
            }
            if (in.size() > 0) {
                String raw = in.toString();
                return raw.length() > 400 ? raw.substring(0, 400) + "..." : raw;
            }
        }
        JsonElement locs = call.get("locations");
        if (locs != null && locs.isJsonArray() && locs.getAsJsonArray().size() > 0
            && locs.getAsJsonArray().get(0).isJsonObject()) {
            return str(locs.getAsJsonArray().get(0).getAsJsonObject(), "path");
        }
        return null;
    }

    // -- Activity feed --------------------------------------------------------

    private void flushThinking() {
        TaskState t = task;
        if (t == null || thinking.length() == 0) {
            return;
        }
        String summary = thinking.toString().trim().split("\n")[0];
        if (summary.length() > 120) {
            summary = summary.substring(0, 120) + "...";
        }
        thinking.setLength(0);
        if (!summary.isEmpty()) {
            t.emitEvent(TaskState.EVT_THINKING, summary);
        }
    }

    private void showToolCall(JsonObject u) {
        TaskState t = task;
        String title = str(u, "title");
        String tool = ToolLabels.findIn(title);
        String label = tool != null ? ToolLabels.label(tool) : (title != null ? title : "Tool");
        String key = tool != null ? ToolLabels.contextKey(tool) : null;
        String context = key != null ? inputValue(u, key) : null;
        if (context != null) {
            // First line only, 80 characters at most
            int nl = context.indexOf('\n');
            context = nl >= 0 ? context.substring(0, nl) : context;
            context = context.length() > 80 ? context.substring(0, 80) + "..." : context;
        }
        String display = context != null && !context.isEmpty() ? label + ": " + context : label;
        AgentProcess.logAppend(logFile, "[tool_use] " + title);
        t.emitEvent(TaskState.EVT_TOOL_USE, display);
    }

    private static boolean hasInput(JsonObject u) {
        JsonObject in = obj(u, "rawInput");
        return in != null && in.size() > 0;
    }

    /** A value of the tool input, at top level or under "arguments". */
    private static String inputValue(JsonObject u, String key) {
        JsonObject in = obj(u, "rawInput");
        if (in == null) {
            return null;
        }
        if (in.has(key) && in.get(key).isJsonPrimitive()) {
            return in.get(key).getAsString();
        }
        JsonObject args = obj(in, "arguments");
        return args != null && args.has(key) && args.get(key).isJsonPrimitive()
               ? args.get(key).getAsString() : null;
    }

    private static String contentText(JsonObject u) {
        JsonObject c = obj(u, "content");
        return c != null && "text".equals(str(c, "type")) && str(c, "text") != null ? str(c, "text") : "";
    }

    // -- JSON helpers -----------------------------------------------------------

    private static JsonObject obj(JsonObject o, String key) {
        JsonElement e = o != null ? o.get(key) : null;
        return e != null && e.isJsonObject() ? e.getAsJsonObject() : null;
    }

    private static String str(JsonObject o, String key) {
        JsonElement e = o != null ? o.get(key) : null;
        return e != null && e.isJsonPrimitive() ? e.getAsString() : null;
    }

    private static long num(JsonObject o, String key) {
        JsonElement e = o.get(key);
        return e != null && e.isJsonPrimitive() && e.getAsJsonPrimitive().isNumber() ? e.getAsLong() : 0;
    }
}
