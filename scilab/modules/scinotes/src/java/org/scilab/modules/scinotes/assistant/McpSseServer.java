package org.scilab.modules.scinotes.assistant;

import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * MCP (Model Context Protocol) server with SSE and streamable-HTTP transports.
 *
 * Implements the MCP specification:
 * - GET  /sse       -> SSE stream (server->client messages) [legacy transport,
 *                      for agents that only accept SSE]
 * - POST /messages   -> client->server JSON-RPC messages (SSE transport)
 * - POST /mcp        -> stateless streamable-HTTP transport (one JSON-RPC
 *                      request per POST, response in the HTTP body) [used by
 *                      the ACP agents, directly or through McpStdioBridge]
 *
 * Uses JDK's built-in com.sun.net.httpserver -- no external dependencies.
 */
public final class McpSseServer {

    private static final Logger LOG = Logger.getLogger(McpSseServer.class.getName());
    private static final String MCP_VERSION = "2024-11-05";
    /** Streamable HTTP exists since the 2025-03-26 revision of the spec. */
    private static final String MCP_VERSION_HTTP = "2025-03-26";
    private static final String SERVER_NAME = "scilab-assistant";

    private static HttpServer httpServer;
    private static volatile boolean running = false;

    /** Active SSE connections: sessionId -> SseConnection */
    private static final Map<String, SseConnection> sessions = new ConcurrentHashMap<>();

    private McpSseServer() {
    }

    /**
     * Start the MCP SSE server on auto-assigned port.
     * @return the actual port, or -1 on failure
     */
    public static synchronized int start() {
        if (running) {
            return httpServer.getAddress().getPort();
        }

        try {
            InetSocketAddress addr = new InetSocketAddress(InetAddress.getLoopbackAddress(), 0);
            httpServer = HttpServer.create(addr, 10);
            httpServer.setExecutor(Executors.newCachedThreadPool(r -> {
                Thread t = new Thread(r, "McpSse-worker");
                t.setDaemon(true);
                return t;
            }));

            httpServer.createContext("/sse", McpSseServer::handleSse);
            httpServer.createContext("/messages", McpSseServer::handleMessages);
            httpServer.createContext("/mcp", McpSseServer::handleStreamableHttp);

            // HttpServer.start() creates the "HTTP-Dispatcher" thread, which
            // inherits the daemon status of the calling thread. Started from
            // a user thread, it is non-daemon: the JVM then never exits and
            // Scilab stays alive as a zombie process after being closed.
            Thread starter = new Thread(httpServer::start, "McpSse-start");
            starter.setDaemon(true);
            starter.start();
            starter.join();
            running = true;

            int port = httpServer.getAddress().getPort();
            LOG.info("McpSseServer started on port " + port);
            return port;
        } catch (IOException e) {
            LOG.log(Level.SEVERE, "Failed to start McpSseServer", e);
            return -1;
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return -1;
        }
    }

    /**
     * Stop the MCP SSE server.
     */
    public static synchronized void stop() {
        if (!running) {
            return;
        }
        running = false;

        // Close all SSE connections
        for (SseConnection conn : sessions.values()) {
            conn.close();
        }
        sessions.clear();

        httpServer.stop(1);
        LOG.info("McpSseServer stopped");
    }

    public static boolean isRunning() {
        return running;
    }

    public static int getPort() {
        if (httpServer != null && running) {
            return httpServer.getAddress().getPort();
        }
        return -1;
    }

    // --- SSE endpoint: GET /sse ---

    private static void handleSse(HttpExchange exchange) throws IOException {
        if (!"GET".equals(exchange.getRequestMethod())) {
            exchange.sendResponseHeaders(405, -1);
            return;
        }

        String sessionId = UUID.randomUUID().toString();
        int port = httpServer.getAddress().getPort();
        String messagesUrl = "http://127.0.0.1:" + port + "/messages?sessionId=" + sessionId;

        // SSE headers
        exchange.getResponseHeaders().set("Content-Type", "text/event-stream");
        exchange.getResponseHeaders().set("Cache-Control", "no-cache");
        exchange.getResponseHeaders().set("Connection", "keep-alive");
        exchange.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
        exchange.sendResponseHeaders(200, 0);

        OutputStream out = exchange.getResponseBody();
        SseConnection conn = new SseConnection(out);
        sessions.put(sessionId, conn);

        // Send the endpoint event (MCP SSE transport spec)
        conn.sendEvent("endpoint", messagesUrl);

        // Keep connection alive until client disconnects
        try {
            while (running && !conn.isClosed()) {
                Thread.sleep(15000);
                // Send keepalive comment
                conn.sendComment("keepalive");
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            sessions.remove(sessionId);
            conn.close();
            exchange.close();
        }
    }

    // --- Messages endpoint: POST /messages?sessionId=xxx ---

    private static void handleMessages(HttpExchange exchange) throws IOException {
        // CORS preflight
        if ("OPTIONS".equals(exchange.getRequestMethod())) {
            exchange.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
            exchange.getResponseHeaders().set("Access-Control-Allow-Methods", "POST, OPTIONS");
            exchange.getResponseHeaders().set("Access-Control-Allow-Headers", "Content-Type");
            exchange.sendResponseHeaders(204, -1);
            return;
        }

        if (!"POST".equals(exchange.getRequestMethod())) {
            exchange.sendResponseHeaders(405, -1);
            return;
        }

        // Extract sessionId from query
        String query = exchange.getRequestURI().getQuery();
        String sessionId = null;
        if (query != null) {
            for (String param : query.split("&")) {
                if (param.startsWith("sessionId=")) {
                    sessionId = param.substring("sessionId=".length());
                }
            }
        }

        SseConnection conn = sessionId != null ? sessions.get(sessionId) : null;
        if (conn == null) {
            byte[] err = "{\"error\":\"unknown session\"}".getBytes(StandardCharsets.UTF_8);
            exchange.sendResponseHeaders(404, err.length);
            exchange.getResponseBody().write(err);
            exchange.close();
            return;
        }

        // Read request body
        InputStream is = exchange.getRequestBody();
        String body = new String(is.readAllBytes(), StandardCharsets.UTF_8);

        // Acknowledge receipt
        exchange.getResponseHeaders().set("Content-Type", "application/json");
        exchange.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
        byte[] ack = "{\"ok\":true}".getBytes(StandardCharsets.UTF_8);
        exchange.sendResponseHeaders(202, ack.length);
        exchange.getResponseBody().write(ack);
        exchange.close();

        // Process JSON-RPC in background
        processJsonRpc(conn, body);
    }

    // --- Streamable HTTP endpoint: POST /mcp ---

    /**
     * Stateless streamable-HTTP transport: each POST carries one JSON-RPC
     * message; requests are answered directly in the HTTP response body
     * (Content-Type: application/json), notifications with 202/empty.
     * The spec allows a plain JSON response instead of an SSE stream.
     */
    private static void handleStreamableHttp(HttpExchange exchange) throws IOException {
        String httpMethod = exchange.getRequestMethod();
        if ("OPTIONS".equals(httpMethod)) {
            exchange.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
            exchange.getResponseHeaders().set("Access-Control-Allow-Methods", "POST, OPTIONS");
            exchange.getResponseHeaders().set("Access-Control-Allow-Headers",
                "Content-Type, Accept, Mcp-Session-Id, MCP-Protocol-Version");
            exchange.sendResponseHeaders(204, -1);
            return;
        }
        if (!"POST".equals(httpMethod)) {
            // No server-initiated stream: GET is not supported (405 is a
            // valid answer per the spec, clients fall back to POST-only).
            exchange.sendResponseHeaders(405, -1);
            exchange.close();
            return;
        }

        String body = new String(exchange.getRequestBody().readAllBytes(), StandardCharsets.UTF_8);
        String response = dispatchJsonRpc(body, MCP_VERSION_HTTP);

        exchange.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
        if (response == null) {
            // Notification: accepted, no body
            exchange.sendResponseHeaders(202, -1);
        } else {
            byte[] bytes = response.getBytes(StandardCharsets.UTF_8);
            exchange.getResponseHeaders().set("Content-Type", "application/json");
            exchange.sendResponseHeaders(200, bytes.length);
            exchange.getResponseBody().write(bytes);
        }
        exchange.close();
    }

    // --- JSON-RPC dispatch (shared by both transports) ---

    private static void processJsonRpc(SseConnection conn, String body) {
        String response = dispatchJsonRpc(body, MCP_VERSION);
        if (response != null) {
            conn.sendEvent("message", response);
        }
    }

    /**
     * Handle one JSON-RPC message.
     * @return the response JSON, or null for notifications
     */
    private static String dispatchJsonRpc(String body, String defaultProtocolVersion) {
        String method = JsonUtil.extractString(body, "method");
        String id = JsonUtil.extractString(body, "id");

        if (method == null) {
            return id != null ? buildError(id, -32600, "missing method") : null;
        }

        switch (method) {
            case "initialize":
                return handleInitialize(id, body, defaultProtocolVersion);
            case "notifications/initialized":
            case "notifications/cancelled":
                // No response needed for notifications
                return null;
            case "tools/list":
                return handleToolsList(id);
            case "tools/call":
                return handleToolsCall(id, body);
            case "ping":
                return buildResult(id, "{}");
            default:
                // Notifications must not be answered, even unknown ones
                if (id == null) {
                    return null;
                }
                return buildError(id, -32601, "method not found: " + method);
        }
    }

    // --- MCP method handlers ---

    private static String handleInitialize(String id, String body, String defaultVersion) {
        // Echo the client's protocol version when it sent one — the claude
        // CLI talks 2024-11-05 over SSE while streamable-HTTP clients ask
        // for 2025-03-26 or later, and some reject a mismatched answer.
        String version = JsonUtil.extractString(body, "protocolVersion");
        if (version == null || version.isEmpty()) {
            version = defaultVersion;
        }
        return buildResult(id,
            "{\"protocolVersion\":\"" + JsonUtil.escape(version) + "\"," +
            "\"capabilities\":{\"tools\":{}}," +
            "\"serverInfo\":{\"name\":\"" + SERVER_NAME + "\",\"version\":\"1.0.0\"}}");
    }

    private static String handleToolsList(String id) {
        String tools = McpToolHandler.getToolsListJson();
        return buildResult(id, "{\"tools\":" + tools + "}");
    }

    private static String handleToolsCall(String id, String body) {
        // Extract tool name and arguments from nested JSON
        String paramsStr = extractObject(body, "params");
        if (paramsStr == null) {
            return buildError(id, -32602, "missing params");
        }

        String toolName = JsonUtil.extractString(paramsStr, "name");
        if (toolName == null) {
            return buildError(id, -32602, "missing tool name");
        }

        String argsStr = extractObject(paramsStr, "arguments");
        String result = McpToolHandler.callTool(toolName, argsStr != null ? argsStr : "{}");

        // MCP tool result format -- detect image content
        if (result.startsWith("![figure](data:image/png;base64,")) {
            String b64 = result.substring("![figure](data:image/png;base64,".length(),
                                          result.length() - 1); // remove trailing )
            return buildResult(id,
                "{\"content\":[{\"type\":\"image\",\"data\":\"" + b64
                + "\",\"mimeType\":\"image/png\"}]}");
        }
        return buildResult(id,
            "{\"content\":[{\"type\":\"text\",\"text\":\"" + JsonUtil.escape(result) + "\"}]}");
    }

    // --- JSON-RPC response builders ---

    private static String buildResult(String id, String result) {
        return "{\"jsonrpc\":\"2.0\",\"id\":" + formatId(id) + ",\"result\":" + result + "}";
    }

    private static String buildError(String id, int code, String message) {
        return "{\"jsonrpc\":\"2.0\",\"id\":" + formatId(id) +
               ",\"error\":{\"code\":" + code + ",\"message\":\"" + JsonUtil.escape(message) + "\"}}";
    }

    private static String formatId(String id) {
        if (id == null) return "null";
        // Try to detect if id is a number
        try {
            Long.parseLong(id);
            return id;
        } catch (NumberFormatException e) {
            return "\"" + JsonUtil.escape(id) + "\"";
        }
    }

    /**
     * Extract a JSON object value (as raw string) for a given key.
     * Handles nested braces.
     */
    static String extractObject(String json, String key) {
        String search = "\"" + key + "\"";
        int idx = json.indexOf(search);
        if (idx < 0) return null;

        idx = json.indexOf(':', idx + search.length());
        if (idx < 0) return null;
        idx++;

        while (idx < json.length() && json.charAt(idx) == ' ') idx++;
        if (idx >= json.length() || json.charAt(idx) != '{') return null;

        int depth = 0;
        int start = idx;
        for (int i = idx; i < json.length(); i++) {
            char c = json.charAt(i);
            if (c == '"') {
                // Skip string content
                i++;
                while (i < json.length()) {
                    if (json.charAt(i) == '\\') { i++; }
                    else if (json.charAt(i) == '"') break;
                    i++;
                }
            } else if (c == '{') {
                depth++;
            } else if (c == '}') {
                depth--;
                if (depth == 0) {
                    return json.substring(start, i + 1);
                }
            }
        }
        return null;
    }

    // --- SSE Connection wrapper ---

    private static class SseConnection {
        private final OutputStream out;
        private volatile boolean closed = false;

        SseConnection(OutputStream out) {
            this.out = out;
        }

        synchronized void sendEvent(String event, String data) {
            if (closed) return;
            try {
                StringBuilder sb = new StringBuilder();
                sb.append("event: ").append(event).append("\n");
                // Split data by newlines for SSE format
                for (String line : data.split("\n")) {
                    sb.append("data: ").append(line).append("\n");
                }
                sb.append("\n");
                out.write(sb.toString().getBytes(StandardCharsets.UTF_8));
                out.flush();
            } catch (IOException e) {
                closed = true;
            }
        }

        synchronized void sendComment(String comment) {
            if (closed) return;
            try {
                out.write((":" + comment + "\n\n").getBytes(StandardCharsets.UTF_8));
                out.flush();
            } catch (IOException e) {
                closed = true;
            }
        }

        boolean isClosed() {
            return closed;
        }

        void close() {
            closed = true;
            try {
                out.close();
            } catch (IOException e) {
                // ignore
            }
        }
    }
}
