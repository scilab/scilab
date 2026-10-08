package org.scilab.modules.scinotes.assistant;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.concurrent.atomic.AtomicLong;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.action_binding.utils.Signal;

/**
 * TCP command server for the Scilab assistant.
 *
 * Accepts JSON-line commands over TCP, executes them in the Scilab interpreter,
 * captures output, and returns results. Used internally by McpSseServer.
 *
 * Protocol (JSON lines over TCP):
 *   Request:  {"id":"abc", "command":"disp(1+1)", "silent":"true"}
 *   Response: {"id":"abc", "status":"ok", "output":"   2.\n"}
 *   Error:    {"id":"abc", "status":"error", "output":"", "error":"..."}
 */
public final class CommandServer {

    private static final Logger LOG = Logger.getLogger(CommandServer.class.getName());

    private static ServerSocket serverSocket;
    private static Thread serverThread;
    private static volatile boolean running = false;

    private static final AtomicLong requestCounter = new AtomicLong(0);
    private static final Object execLock = new Object();

    private CommandServer() {
    }

    /**
     * Start the command server on auto-assigned port.
     * @return the actual port number, or -1 on failure
     */
    public static synchronized int start() {
        if (running) {
            return serverSocket.getLocalPort();
        }

        try {
            serverSocket = new ServerSocket(0, 5, InetAddress.getLoopbackAddress());
            running = true;

            int actualPort = serverSocket.getLocalPort();

            serverThread = new Thread(CommandServer::acceptLoop, "AssistantCommandServer");
            serverThread.setDaemon(true);
            serverThread.start();

            LOG.info("AssistantCommandServer started on port " + actualPort);
            return actualPort;
        } catch (IOException e) {
            LOG.log(Level.SEVERE, "Failed to start AssistantCommandServer", e);
            running = false;
            return -1;
        }
    }

    /**
     * Stop the command server.
     */
    public static synchronized void stop() {
        if (!running) {
            return;
        }
        running = false;
        try {
            serverSocket.close();
        } catch (IOException e) {
            // ignore
        }
        LOG.info("AssistantCommandServer stopped");
    }

    public static int getPort() {
        if (serverSocket != null && !serverSocket.isClosed()) {
            return serverSocket.getLocalPort();
        }
        return -1;
    }

    public static boolean isRunning() {
        return running;
    }

    // --- Accept loop ---

    private static void acceptLoop() {
        while (running) {
            try {
                Socket client = serverSocket.accept();
                Thread handler = new Thread(
                    () -> handleClient(client),
                    "Assistant-client-" + requestCounter.incrementAndGet());
                handler.setDaemon(true);
                handler.start();
            } catch (IOException e) {
                if (running) {
                    LOG.log(Level.WARNING, "Accept failed", e);
                }
            }
        }
    }

    // --- Client handler ---

    private static void handleClient(Socket client) {
        try {
            client.setTcpNoDelay(true);
            BufferedReader in = new BufferedReader(
                new InputStreamReader(client.getInputStream(), StandardCharsets.UTF_8));
            OutputStream out = client.getOutputStream();

            String line;
            while ((line = in.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) {
                    continue;
                }

                String id = JsonUtil.extractString(line, "id");
                String command = JsonUtil.extractString(line, "command");
                String silentStr = JsonUtil.extractString(line, "silent");
                boolean silent = "true".equals(silentStr);

                if (id == null) {
                    id = String.valueOf(requestCounter.incrementAndGet());
                }

                if (command == null || command.isEmpty()) {
                    writeResponse(out, id, "error", "", "missing 'command' field");
                    continue;
                }

                synchronized (execLock) {
                    executeAndRespond(out, id, command, silent);
                }
            }
        } catch (IOException e) {
            // Client disconnected
        } finally {
            try {
                client.close();
            } catch (IOException e) {
                // ignore
            }
        }
    }

    // --- Command execution with output capture ---

    /** Default timeout for LLM-provided code: generous, computations can be long. */
    static final long DEFAULT_TIMEOUT_MS = 10 * 60 * 1000;

    static String executeCommand(String command, boolean silent) {
        return executeCommand(command, silent, DEFAULT_TIMEOUT_MS);
    }

    /**
     * Execute a Scilab command and return the output.
     * Uses InterpreterManagement output capture (C-level buffer).
     * Can be called directly by McpToolHandler.
     *
     * The command is executed through execstr(..., "errcatch") so that even a
     * PARSE error in the command is caught: with a plain try/catch wrapper, a
     * parse error would abort the whole block including the final notify(),
     * leaving the wait — and execLock — stuck forever and jamming every
     * subsequent tool call.
     *
     * @param timeoutMs max time to wait for the interpreter; on expiry an
     *                  error string is returned and the lock is released.
     */
    /**
     * Scilab code setting %_m to the last error message followed by where it
     * happened ("at line 4 of function badfun"), from lasterror(%f), which
     * does not clear the error. Also sets %_n, %_l, %_f: clear them after.
     * lasterror reports the function "execstr" for the code run by
     * execute_scilab_code, and "exec" for a script run with exec().
     */
    static final String LASTERROR_WITH_LOCATION =
        "[%_m, %_n, %_l, %_f] = lasterror(%f);\n"
        + "if %_l > 0 then\n"
        + "    if %_f == \"execstr\" then\n"
        + "        %_m($+1) = msprintf(\"at line %d of the executed code\", %_l);\n"
        + "    elseif %_f == \"exec\" then\n"
        + "        %_m($+1) = msprintf(\"at line %d of the executed script\", %_l);\n"
        + "    elseif %_f == \"\" then\n"
        + "        %_m($+1) = msprintf(\"at line %d\", %_l);\n"
        + "    else\n"
        + "        %_m($+1) = msprintf(\"at line %d of function %s\", %_l, %_f);\n"
        + "    end\n"
        // A builtin failing on its arguments (e.g. cos(\"t\"): no %c_cos
        // overload) reports itself with line 0: the caller's line is lost
        + "elseif %_f <> \"\" & %_f <> \"execstr\" & %_f <> \"exec\" then\n"
        + "    %_m($+1) = msprintf(\"raised by %s (line unknown: look for its calls)\", %_f);\n"
        + "end\n";

    static String executeCommand(String command, boolean silent, long timeoutMs) {
        String id = String.valueOf(requestCounter.incrementAndGet());
        String tmpDir = System.getProperty("java.io.tmpdir");
        String errFile = tmpDir + File.separator + "__assistant_" + id + ".err";
        String uid = "assistant_" + id;
        String sErrFile = errFile.replace("\\", "/");

        StringBuilder wrapped = new StringBuilder();
        wrapped.append("if execstr(").append(toScilabStringMatrix(command))
               .append(", \"errcatch\") <> 0 then\n");
        // lasterror(%f): read without clearing, so that the last_error tool
        // (and the user) still see this error afterwards
        wrapped.append(LASTERROR_WITH_LOCATION);
        wrapped.append("mputl(%_m, '").append(sErrFile).append("')\n");
        wrapped.append("clear %_m %_n %_l %_f\n");
        wrapped.append("end\n");
        wrapped.append("notify('").append(uid).append("')");

        synchronized (execLock) {
            LOG.info("executeCommand: queuing [" + uid + "] silent=" + silent
                     + " cmd=" + (command.length() > 100 ? command.substring(0, 100) + "..." : command));

            InterpreterManagement.startScilabOutputCapture(silent ? 1 : 0);

            int ret = InterpreterManagement.requestScilabExec(wrapped.toString());
            LOG.info("requestScilabExec returned " + ret + " for [" + uid + "]");
            if (ret != 0) {
                InterpreterManagement.stopScilabOutputCapture();
                LOG.warning("requestScilabExec FAILED (ret=" + ret + ") for command: "
                            + (wrapped.length() > 200 ? wrapped.substring(0, 200) + "..." : wrapped));
                return null;
            }

            boolean finished = waitWithTimeout(uid, timeoutMs);
            String output = InterpreterManagement.stopScilabOutputCapture();

            if (!finished) {
                LOG.warning("executeCommand: timeout after " + timeoutMs + "ms for [" + uid + "]");
                return "ERROR: Scilab did not finish within " + (timeoutMs / 1000)
                    + "s (interpreter busy or command blocked). The command may still be running.";
            }

            String error = readAndDelete(errFile);
            if (error != null) {
                return "ERROR: " + error;
            }
            return output != null ? output : "";
        }
    }

    /**
     * Wait for the Scilab-side notify() with a timeout. Signal.wait() blocks
     * with no timeout, so it is parked in a daemon thread: if the command
     * never notifies, the thread stays parked (harmless) and the caller
     * regains control instead of holding execLock forever.
     */
    private static boolean waitWithTimeout(String uid, long timeoutMs) {
        Thread waiter = new Thread(() -> Signal.wait(uid), "assistant-wait-" + uid);
        waiter.setDaemon(true);
        waiter.start();
        try {
            waiter.join(timeoutMs);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        return !waiter.isAlive();
    }

    /**
     * Convert multi-line code to a Scilab string-matrix literal
     * ["line1";"line2";...] suitable for execstr. Quotes are doubled
     * (Scilab escaping) so any content is representable.
     */
    private static String toScilabStringMatrix(String command) {
        String[] lines = command.replace("\r", "").split("\n", -1);
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < lines.length; i++) {
            if (i > 0) {
                sb.append(";");
            }
            sb.append("\"")
              .append(lines[i].replace("\"", "\"\"").replace("'", "''"))
              .append("\"");
        }
        sb.append("]");
        return sb.toString();
    }

    private static void executeAndRespond(OutputStream out, String id,
                                           String command, boolean silent) {
        String result = executeCommand(command, silent);
        if (result == null) {
            writeResponse(out, id, "error", "", "failed to queue command");
        } else if (result.startsWith("ERROR: ")) {
            writeResponse(out, id, "error", "", result.substring(7));
        } else {
            writeResponse(out, id, "ok", result, null);
        }
    }

    private static String readAndDelete(String path) {
        try {
            Path p = Paths.get(path);
            if (Files.exists(p)) {
                String content = new String(Files.readAllBytes(p), StandardCharsets.UTF_8);
                Files.delete(p);
                return content;
            }
        } catch (IOException e) {
            LOG.log(Level.FINE, "Could not read " + path, e);
        }
        return null;
    }

    private static void writeResponse(OutputStream out, String id, String status,
                                       String output, String error) {
        StringBuilder json = new StringBuilder();
        json.append("{\"id\":\"").append(JsonUtil.escape(id)).append("\"");
        json.append(",\"status\":\"").append(status).append("\"");
        json.append(",\"output\":\"").append(JsonUtil.escape(output)).append("\"");
        if (error != null) {
            json.append(",\"error\":\"").append(JsonUtil.escape(error)).append("\"");
        }
        json.append("}\n");

        try {
            out.write(json.toString().getBytes(StandardCharsets.UTF_8));
            out.flush();
        } catch (IOException e) {
            // Client gone
        }
    }
}
