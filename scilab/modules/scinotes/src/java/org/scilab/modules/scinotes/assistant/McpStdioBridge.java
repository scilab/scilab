package org.scilab.modules.scinotes.assistant;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

/**
 * The scilab MCP server over stdio, for the ACP agents that accept no HTTP
 * MCP server (stdio is the only transport every ACP agent must support).
 * Started by the agent as a separate JVM: each JSON-RPC line read on stdin
 * is POSTed to the stateless streamable-HTTP endpoint of
 * {@link McpSseServer}, and the answer written back on stdout.
 *
 * Depends on the JDK only: run with the scinotes jar alone on the classpath.
 *
 * Usage: McpStdioBridge &lt;http://127.0.0.1:port/mcp&gt;
 */
public final class McpStdioBridge {

    private McpStdioBridge() {
    }

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.err.println("usage: McpStdioBridge <mcp url>");
            System.exit(2);
        }
        URL url = java.net.URI.create(args[0]).toURL();
        PrintStream out = new PrintStream(System.out, false, "UTF-8");
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8));
        String line;
        while ((line = in.readLine()) != null) {
            String msg = line.trim();
            if (msg.isEmpty()) {
                continue;
            }
            // One thread per message: a long execute_scilab_code must not
            // hold back the next messages (cancellations, pings)
            Thread t = new Thread(() -> forward(url, msg, out), "mcp-bridge");
            t.setDaemon(true);
            t.start();
        }
    }

    private static void forward(URL url, String msg, PrintStream out) {
        try {
            HttpURLConnection c = (HttpURLConnection) url.openConnection();
            c.setRequestMethod("POST");
            c.setDoOutput(true);
            c.setRequestProperty("Content-Type", "application/json");
            c.setRequestProperty("Accept", "application/json, text/event-stream");
            try (OutputStream os = c.getOutputStream()) {
                os.write(msg.getBytes(StandardCharsets.UTF_8));
            }
            int code = c.getResponseCode();
            InputStream is = code < 400 ? c.getInputStream() : c.getErrorStream();
            String body = is != null ? new String(is.readAllBytes(), StandardCharsets.UTF_8).trim() : "";
            if (!body.isEmpty()) {
                synchronized (out) {
                    out.println(body.replace('\n', ' ').replace('\r', ' '));
                    out.flush();
                }
            }
        } catch (Exception e) {
            System.err.println("mcp bridge: " + e);
        }
    }
}
