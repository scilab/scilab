package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.logging.Logger;

/**
 * Per-conversation JSONL transcript of the assistant exchanges, for later
 * replay and evaluation (see tests/assistant/README.md): reproduce a failed
 * case with AcpSmokeTest, compare tool traces across agents or agent
 * versions, measure skill usage.
 *
 * One file per conversation under SCIHOME/assistant/logs/&lt;date&gt;/, one
 * event per line: turn_start, command, events, tool_call, tool_result,
 * turn_end, cancelled, resume.
 *
 * Controlled by the "session_logs" setting (checkbox in the provider
 * configuration page). The transcripts contain the user's prompts, code and
 * tool results: they never leave this machine, and the total size is capped
 * (oldest files pruned when a new conversation starts).
 */
final class SessionLogger {

    private static final Logger LOG = Logger.getLogger(SessionLogger.class.getName());

    /** Total size cap of the logs directory. */
    private static final long MAX_TOTAL_BYTES = 50L * 1024 * 1024;
    /** Cap applied to every logged text field. */
    private static final int MAX_FIELD_CHARS = 200_000;

    private static final Object LOCK = new Object();
    /** Current conversation file; null until the first logged event. */
    private static Path currentFile;
    private static int fileCounter = 0;
    /** Last logged system prompt hash (per conversation): identical
     *  re-sends are logged as a reference instead of duplicated. */
    private static int lastSystemPromptHash = 0;
    /** Turn start times, for turn_end durations. */
    private static final Map<Integer, Long> TURN_START_MS = new ConcurrentHashMap<>();

    private SessionLogger() {
    }

    static boolean isEnabled() {
        return AssistantSettings.getBool("session_logs", true);
    }

    static void setEnabled(boolean enabled) {
        AssistantSettings.setBool("session_logs", enabled);
    }

    /** Logs directory path for display, or null when unavailable. */
    static String logsDirPath() {
        File dir = AssistantSettings.assistantDir();
        return dir != null ? new File(dir, "logs").getAbsolutePath() : null;
    }

    // -- Conversation lifecycle -------------------------------------------

    /** Close the current file: the next event starts a new conversation. */
    static void newConversation() {
        synchronized (LOCK) {
            currentFile = null;
            lastSystemPromptHash = 0;
        }
    }

    /** A past conversation is resumed: new file referencing its session. */
    static void resumedConversation(String sessionId, String provider) {
        newConversation();
        if (!isEnabled()) {
            return;
        }
        StringBuilder sb = start("resume");
        strField(sb, "session_id", sessionId);
        strField(sb, "provider", provider);
        write(sb);
    }

    // -- Turn events ------------------------------------------------------

    static void turnStart(int requestId, String provider, String model, String effort,
                          String resumeSessionId, String prompt, String systemPrompt) {
        if (!isEnabled()) {
            return;
        }
        TURN_START_MS.put(requestId, System.currentTimeMillis());
        StringBuilder sb = start("turn_start");
        numField(sb, "request_id", requestId);
        strField(sb, "provider", provider);
        strField(sb, "model", model);
        if (effort != null && !effort.isEmpty()) {
            strField(sb, "effort", effort);
        }
        if (resumeSessionId != null && !resumeSessionId.isEmpty()) {
            strField(sb, "resume_session_id", resumeSessionId);
        }
        strField(sb, "prompt", prompt);
        int h = systemPrompt != null ? systemPrompt.hashCode() : 0;
        synchronized (LOCK) {
            if (h != lastSystemPromptHash) {
                strField(sb, "system_prompt", systemPrompt);
                lastSystemPromptHash = h;
            } else {
                sb.append(",\"system_prompt\":\"(unchanged)\"");
            }
        }
        write(sb);
    }

    /** Long arguments are shortened to this. */
    private static final int MAX_ARG_CHARS = 300;

    /**
     * The command line an agent was started with and the environment
     * variables set from agents.json: what support needs to see when an
     * agent update broke something. Logged each time an agent starts
     * (request_id 0 when started by the settings page).
     */
    static void command(int requestId, List<String> cmd, Map<String, String> env) {
        if (!isEnabled()) {
            return;
        }
        StringBuilder sb = start("command");
        numField(sb, "request_id", requestId);
        sb.append(",\"args\":[");
        for (int i = 0; i < cmd.size(); i++) {
            String arg = cmd.get(i);
            if (arg.length() > MAX_ARG_CHARS) {
                arg = arg.substring(0, MAX_ARG_CHARS) + "...[" + arg.length() + " chars]";
            }
            sb.append(i > 0 ? "," : "").append("\"").append(JsonUtil.escape(arg)).append("\"");
        }
        sb.append("],\"env\":{");
        boolean first = true;
        for (Map.Entry<String, String> e : env.entrySet()) {
            sb.append(first ? "" : ",").append("\"").append(JsonUtil.escape(e.getKey()))
              .append("\":\"").append(JsonUtil.escape(e.getValue())).append("\"");
            first = false;
        }
        sb.append("}");
        write(sb);
    }

    /** Activity events drained by a poll, as the JSON array the GUI got. */
    static void events(int requestId, String eventsJsonArray) {
        if (eventsJsonArray == null || eventsJsonArray.isEmpty()
            || "[]".equals(eventsJsonArray) || !isEnabled()) {
            return;
        }
        StringBuilder sb = start("events");
        numField(sb, "request_id", requestId);
        sb.append(",\"events\":").append(eventsJsonArray);
        write(sb);
    }

    static void turnEnd(int requestId, String sessionId, String result,
                        String error, double cost, long tokensIn, long tokensOut) {
        if (!isEnabled()) {
            TURN_START_MS.remove(requestId);
            return;
        }
        StringBuilder sb = start("turn_end");
        numField(sb, "request_id", requestId);
        strField(sb, "session_id", sessionId);
        Long t0 = TURN_START_MS.remove(requestId);
        if (t0 != null) {
            numField(sb, "duration_ms", System.currentTimeMillis() - t0);
        }
        if (error != null && !error.isEmpty()) {
            strField(sb, "error", error);
        } else {
            strField(sb, "result", result);
        }
        if (cost > 0) {
            sb.append(",\"cost\":").append(cost);
        }
        numField(sb, "tokens_in", tokensIn);
        numField(sb, "tokens_out", tokensOut);
        write(sb);
    }

    static void cancelled(int requestId) {
        if (!isEnabled()) {
            TURN_START_MS.remove(requestId);
            return;
        }
        StringBuilder sb = start("cancelled");
        numField(sb, "request_id", requestId);
        Long t0 = TURN_START_MS.remove(requestId);
        if (t0 != null) {
            numField(sb, "duration_ms", System.currentTimeMillis() - t0);
        }
        write(sb);
    }

    /** An attached file was saved locally for path-based referencing. */
    static void fileStaged(String path, long bytes) {
        if (!isEnabled()) {
            return;
        }
        StringBuilder sb = start("file_staged");
        strField(sb, "path", path);
        numField(sb, "bytes", bytes);
        write(sb);
    }

    /** An ask arrived without a parsable prompt (truncated/malformed
     *  request): keep its size and head for diagnosis. */
    static void askRejected(int requestChars, String requestHead) {
        if (!isEnabled()) {
            return;
        }
        StringBuilder sb = start("ask_rejected");
        numField(sb, "request_chars", requestChars);
        strField(sb, "request_head", truncate(requestHead, 300));
        write(sb);
    }

    /** Skills whose bodies were injected into the system prompt. */
    static void skillsInjected(List<String> names) {
        if (names == null || names.isEmpty() || !isEnabled()) {
            return;
        }
        StringBuilder sb = start("skills_injected");
        sb.append(",\"skills\":[");
        for (int i = 0; i < names.size(); i++) {
            if (i > 0) {
                sb.append(",");
            }
            sb.append("\"").append(JsonUtil.escape(names.get(i))).append("\"");
        }
        sb.append("]");
        write(sb);
    }

    // -- MCP tool events (server side: same shape for every provider) -----

    static void toolCall(String name, String argsJson) {
        if (!isEnabled()) {
            return;
        }
        StringBuilder sb = start("tool_call");
        strField(sb, "tool", name);
        strField(sb, "args", truncate(argsJson, 20_000));
        write(sb);
    }

    static void toolResult(String name, String result, long durationMs) {
        if (!isEnabled()) {
            return;
        }
        StringBuilder sb = start("tool_result");
        strField(sb, "tool", name);
        numField(sb, "duration_ms", durationMs);
        strField(sb, "result", truncate(result, 8_000));
        write(sb);
    }

    // -- JSON building ----------------------------------------------------

    private static StringBuilder start(String type) {
        StringBuilder sb = new StringBuilder(256);
        sb.append("{\"ts\":\"")
          .append(new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS").format(new Date()))
          .append("\",\"type\":\"").append(type).append("\"");
        return sb;
    }

    private static void strField(StringBuilder sb, String key, String value) {
        sb.append(",\"").append(key).append("\":\"")
          .append(JsonUtil.escape(truncate(value, MAX_FIELD_CHARS))).append("\"");
    }

    private static void numField(StringBuilder sb, String key, long value) {
        sb.append(",\"").append(key).append("\":").append(value);
    }

    private static String truncate(String s, int max) {
        if (s == null) {
            return "";
        }
        return s.length() <= max ? s : s.substring(0, max) + "...[truncated]";
    }

    // -- File handling ----------------------------------------------------

    private static void write(StringBuilder body) {
        body.append("}\n");
        synchronized (LOCK) {
            try {
                Path f = ensureFile();
                if (f == null) {
                    return;
                }
                Files.write(f, body.toString().getBytes(StandardCharsets.UTF_8),
                            StandardOpenOption.CREATE, StandardOpenOption.APPEND);
            } catch (Exception e) {
                LOG.warning("Session log write failed: " + e.getMessage());
            }
        }
    }

    /** Current conversation file, created (and old logs pruned) on first use. */
    private static Path ensureFile() {
        if (currentFile != null) {
            return currentFile;
        }
        File base = AssistantSettings.assistantDir();
        if (base == null) {
            return null;
        }
        try {
            File dir = new File(new File(base, "logs"),
                                new SimpleDateFormat("yyyy-MM-dd").format(new Date()));
            dir.mkdirs();
            prune(new File(base, "logs"));
            String name = new SimpleDateFormat("HHmmss").format(new Date())
                        + "-" + (++fileCounter) + ".jsonl";
            currentFile = new File(dir, name).toPath();
            return currentFile;
        } catch (Exception e) {
            LOG.warning("Session log file creation failed: " + e.getMessage());
            return null;
        }
    }

    /** Keep the logs directory under MAX_TOTAL_BYTES: oldest files first. */
    private static void prune(File logsRoot) {
        try {
            List<File> files = new ArrayList<>();
            File[] days = logsRoot.listFiles(File::isDirectory);
            if (days != null) {
                for (File day : days) {
                    File[] fs = day.listFiles(File::isFile);
                    if (fs != null) {
                        for (File f : fs) {
                            files.add(f);
                        }
                    }
                }
            }
            long total = 0;
            for (File f : files) {
                total += f.length();
            }
            if (total <= MAX_TOTAL_BYTES) {
                return;
            }
            files.sort((a, b) -> Long.compare(a.lastModified(), b.lastModified()));
            for (File f : files) {
                if (total <= MAX_TOTAL_BYTES) {
                    break;
                }
                total -= f.length();
                f.delete();
            }
            // Drop day directories left empty
            if (days != null) {
                for (File day : days) {
                    String[] left = day.list();
                    if (left != null && left.length == 0) {
                        day.delete();
                    }
                }
            }
        } catch (Exception e) {
            LOG.warning("Session log pruning failed: " + e.getMessage());
        }
    }
}
