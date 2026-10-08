package org.scilab.modules.scinotes.assistant;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;

import com.google.gson.JsonElement;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

/**
 * JSON-RPC 2.0 over the stdio of an ACP agent process: one JSON message per
 * line in both directions. Requests from the client wait for their answer
 * (matched by id); notifications and requests from the agent go to the
 * {@link Handler}. Agent requests are answered from their own thread, so a
 * handler may block (e.g. while the user decides on a permission) without
 * stalling the stream.
 */
final class AcpConnection {

    /** Receives what the agent sends on its own initiative. */
    interface Handler {
        /** A notification (session/update...). Called on the reader thread. */
        void onNotification(String method, JsonObject params);

        /** A request (session/request_permission, fs/...): return the result,
         *  or throw an {@link AcpException} to answer with an error. */
        JsonElement onRequest(String method, JsonObject params) throws AcpException;
    }

    /** A JSON-RPC error answered by the agent, or a transport failure. */
    static final class AcpException extends Exception {
        private static final long serialVersionUID = 1L;
        /** JSON-RPC error code; 0 for a transport failure (process died, timeout). */
        final int code;
        /** The "data" member of the error, or null. */
        final transient JsonElement data;

        AcpException(int code, String message, JsonElement data) {
            super(message);
            this.code = code;
            this.data = data;
        }

        /** The message, with the agent's details when it gave some. */
        String describe() {
            String m = getMessage();
            if (data != null && data.isJsonObject()) {
                JsonObject d = data.getAsJsonObject();
                for (String k : new String[] {"message", "details"}) {
                    if (d.has(k) && d.get(k).isJsonPrimitive()) {
                        return m + ": " + d.get(k).getAsString();
                    }
                }
            }
            return m;
        }
    }

    /** JSON-RPC "method not found". */
    static final int METHOD_NOT_FOUND = -32601;

    private final Process process;
    private final OutputStream stdin;
    private final Handler handler;
    private final String logFile;
    private final AtomicInteger nextId = new AtomicInteger(0);
    private final Map<Integer, CompletableFuture<JsonElement>> pending = new ConcurrentHashMap<>();
    private final StringBuilder stderrTail = new StringBuilder();

    private AcpConnection(Process process, Handler handler, String logFile) {
        this.process = process;
        this.stdin = process.getOutputStream();
        this.handler = handler;
        this.logFile = logFile;
    }

    /**
     * Start the agent and its reader threads.
     * @param cwd working directory, null to inherit
     */
    static AcpConnection start(List<String> cmd, File cwd, Map<String, String> env,
                               Handler handler, String logFile) throws IOException {
        ProcessBuilder pb = new ProcessBuilder(cmd);
        if (cwd != null && cwd.isDirectory()) {
            pb.directory(cwd);
        }
        AgentPlatform.adaptUnixEnvironment(pb, cmd.get(0));
        pb.environment().putAll(env);
        pb.redirectErrorStream(false);
        AgentProcess.logAppend(logFile, "[acp] start " + String.join(" ", cmd));
        AcpConnection c = new AcpConnection(pb.start(), handler, logFile);
        c.startThreads();
        return c;
    }

    private void startThreads() {
        Thread out = new Thread(this::readStdout, "acp-stdout");
        out.setDaemon(true);
        out.start();
        Thread err = new Thread(() -> {
            try (BufferedReader r = new BufferedReader(
                    new InputStreamReader(process.getErrorStream(), StandardCharsets.UTF_8))) {
                String line;
                while ((line = r.readLine()) != null) {
                    AgentProcess.logAppend(logFile, "[acp stderr] " + line);
                    synchronized (stderrTail) {
                        stderrTail.append(line).append('\n');
                        if (stderrTail.length() > 2000) {
                            stderrTail.delete(0, stderrTail.length() - 2000);
                        }
                    }
                }
            } catch (IOException e) {
                // process gone
            }
        }, "acp-stderr");
        err.setDaemon(true);
        err.start();
    }

    private void readStdout() {
        try (BufferedReader r = new BufferedReader(
                new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8))) {
            String line;
            while ((line = r.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) {
                    continue;
                }
                AgentProcess.logAppend(logFile, "[acp <<] " + clip(line));
                JsonObject msg;
                try {
                    JsonElement e = JsonParser.parseString(line);
                    if (!e.isJsonObject()) {
                        continue;
                    }
                    msg = e.getAsJsonObject();
                } catch (RuntimeException e) {
                    // not JSON-RPC (a stray log line on stdout)
                    continue;
                }
                dispatch(msg);
            }
        } catch (IOException e) {
            // process gone
        } finally {
            AcpException gone = new AcpException(0, "the agent stopped" + stderrSuffix(), null);
            for (CompletableFuture<JsonElement> f : pending.values()) {
                f.completeExceptionally(gone);
            }
            pending.clear();
        }
    }

    private void dispatch(JsonObject msg) {
        JsonElement id = msg.get("id");
        String method = msg.has("method") ? msg.get("method").getAsString() : null;
        JsonObject params = msg.has("params") && msg.get("params").isJsonObject()
                            ? msg.getAsJsonObject("params") : new JsonObject();
        if (method == null) {
            // Answer to one of our requests
            if (id == null || !id.isJsonPrimitive()) {
                return;
            }
            CompletableFuture<JsonElement> f = pending.remove(id.getAsInt());
            if (f == null) {
                return;
            }
            if (msg.has("error") && msg.get("error").isJsonObject()) {
                JsonObject err = msg.getAsJsonObject("error");
                f.completeExceptionally(new AcpException(
                    err.has("code") ? err.get("code").getAsInt() : -1,
                    err.has("message") ? err.get("message").getAsString() : "error",
                    err.get("data")));
            } else {
                JsonElement result = msg.get("result");
                f.complete(result != null ? result : JsonNull.INSTANCE);
            }
        } else if (id == null) {
            try {
                handler.onNotification(method, params);
            } catch (RuntimeException e) {
                AgentProcess.logAppend(logFile, "[acp] notification handler failed: " + e);
            }
        } else {
            Thread t = new Thread(() -> answer(id, method, params), "acp-request-" + method);
            t.setDaemon(true);
            t.start();
        }
    }

    private void answer(JsonElement id, String method, JsonObject params) {
        JsonObject reply = new JsonObject();
        reply.addProperty("jsonrpc", "2.0");
        reply.add("id", id);
        try {
            JsonElement result = handler.onRequest(method, params);
            reply.add("result", result != null ? result : new JsonObject());
        } catch (AcpException e) {
            reply.add("error", error(e.code, e.getMessage()));
        } catch (RuntimeException e) {
            reply.add("error", error(-32603, String.valueOf(e)));
        }
        try {
            send(reply);
        } catch (IOException e) {
            // process gone
        }
    }

    private static JsonObject error(int code, String message) {
        JsonObject err = new JsonObject();
        err.addProperty("code", code);
        err.addProperty("message", message);
        return err;
    }

    private void send(JsonObject msg) throws IOException {
        String line = msg.toString();
        AgentProcess.logAppend(logFile, "[acp >>] " + clip(line));
        synchronized (stdin) {
            stdin.write(line.getBytes(StandardCharsets.UTF_8));
            stdin.write('\n');
            stdin.flush();
        }
    }

    /** Send a request without waiting: the future gets the result. */
    CompletableFuture<JsonElement> request(String method, JsonObject params) {
        int id = nextId.incrementAndGet();
        CompletableFuture<JsonElement> f = new CompletableFuture<>();
        pending.put(id, f);
        JsonObject msg = new JsonObject();
        msg.addProperty("jsonrpc", "2.0");
        msg.addProperty("id", id);
        msg.addProperty("method", method);
        msg.add("params", params);
        try {
            send(msg);
        } catch (IOException e) {
            pending.remove(id);
            f.completeExceptionally(new AcpException(0, "the agent stopped" + stderrSuffix(), null));
        }
        return f;
    }

    /** Send a request and wait for its result. */
    JsonElement call(String method, JsonObject params, long timeoutMs) throws AcpException {
        return await(request(method, params), timeoutMs, method);
    }

    /** Wait for a request sent with {@link #request}. */
    JsonElement await(CompletableFuture<JsonElement> f, long timeoutMs, String what) throws AcpException {
        try {
            return f.get(timeoutMs, TimeUnit.MILLISECONDS);
        } catch (ExecutionException e) {
            Throwable c = e.getCause();
            if (c instanceof AcpException) {
                throw (AcpException) c;
            }
            throw new AcpException(0, String.valueOf(c), null);
        } catch (TimeoutException e) {
            throw new AcpException(0, what + ": no answer after " + timeoutMs / 1000 + " s", null);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new AcpException(0, what + ": interrupted", null);
        }
    }

    /** Send a notification (session/cancel). */
    void notify(String method, JsonObject params) {
        JsonObject msg = new JsonObject();
        msg.addProperty("jsonrpc", "2.0");
        msg.addProperty("method", method);
        msg.add("params", params);
        try {
            send(msg);
        } catch (IOException e) {
            // process gone
        }
    }

    boolean isAlive() {
        return process.isAlive();
    }

    /** The last lines the agent wrote on stderr. */
    String stderrTail() {
        synchronized (stderrTail) {
            return stderrTail.toString().trim();
        }
    }

    private String stderrSuffix() {
        String tail = stderrTail();
        return tail.isEmpty() ? "" : ":\n" + tail;
    }

    /**
     * Stop the agent at once: no grace period (agents save their sessions
     * as they go, and SciNotes must not freeze while closing). The process
     * tree is read first: once the launcher (a .cmd shim, npx...) is gone,
     * its children can no longer be found and would survive.
     */
    void close() {
        List<ProcessHandle> tree = new java.util.ArrayList<>();
        try {
            process.descendants().forEach(tree::add);
        } catch (Exception e) {
            // best effort
        }
        try {
            stdin.close();
        } catch (IOException e) {
            // already closed
        }
        for (ProcessHandle h : tree) {
            h.destroyForcibly();
        }
        // And whatever started meanwhile (antigravity's harness...)
        AgentPlatform.killTree(process);
    }

    private static String clip(String s) {
        return s.length() > 500 ? s.substring(0, 500) + "..." : s;
    }
}
