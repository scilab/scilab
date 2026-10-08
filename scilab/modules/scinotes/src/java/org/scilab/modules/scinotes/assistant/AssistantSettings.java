package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Assistant settings persisted in SCIHOME/assistant/assistant.json — a flat
 * JSON object written and read only by this class ({"session_logs":true}).
 * Kept Java-side (not in the page's localStorage) because the settings
 * drive Java behavior and must survive a JCEF profile reset.
 */
final class AssistantSettings {

    private static final Logger LOG = Logger.getLogger(AssistantSettings.class.getName());

    /** key -> raw JSON literal (true, false, 42, "text"). */
    private static Map<String, String> values;

    private AssistantSettings() {
    }

    /** Base "assistant" directory, or null when unavailable. The system
     *  property is a test hook (standalone harness has no SCIHOME). */
    static File assistantDir() {
        String override = System.getProperty("scilab.assistant.home");
        if (override != null && !override.isEmpty()) {
            return new File(override);
        }
        try {
            File sciHome = org.scilab.modules.commons.ScilabConstants.SCIHOME;
            return sciHome != null ? new File(sciHome, "assistant") : null;
        } catch (Throwable t) {
            // Outside Scilab (standalone tests): no settings file
            return null;
        }
    }

    private static File settingsFile() {
        File dir = assistantDir();
        return dir != null ? new File(dir, "assistant.json") : null;
    }

    static synchronized boolean getBool(String key, boolean def) {
        String raw = load().get(key);
        if ("true".equals(raw)) {
            return true;
        }
        if ("false".equals(raw)) {
            return false;
        }
        return def;
    }

    static synchronized void setBool(String key, boolean value) {
        load().put(key, String.valueOf(value));
        save();
    }

    static synchronized int getInt(String key, int def) {
        String raw = load().get(key);
        if (raw != null) {
            try {
                return Integer.parseInt(raw);
            } catch (NumberFormatException e) {
                // not an integer: default
            }
        }
        return def;
    }

    static synchronized void setInt(String key, int value) {
        load().put(key, String.valueOf(value));
        save();
    }

    /** String setting, or null when absent. */
    static synchronized String getString(String key) {
        String raw = load().get(key);
        if (raw == null || raw.length() < 2 || raw.charAt(0) != '"') {
            return null;
        }
        return JsonUtil.unescape(raw.substring(1, raw.length() - 1));
    }

    /** Set a string setting; null or empty removes it. */
    static synchronized void setString(String key, String value) {
        if (value == null || value.isEmpty()) {
            load().remove(key);
        } else {
            load().put(key, "\"" + JsonUtil.escape(value) + "\"");
        }
        save();
    }

    // -----------------------------------------------------------------

    private static Map<String, String> load() {
        if (values != null) {
            return values;
        }
        values = new LinkedHashMap<>();
        File f = settingsFile();
        if (f == null || !f.isFile()) {
            return values;
        }
        try {
            String content = new String(Files.readAllBytes(f.toPath()),
                                        StandardCharsets.UTF_8);
            // Flat object of our own making: "key": true|false|number|"string"
            Matcher m = Pattern.compile(
                "\"((?:[^\"\\\\]|\\\\.)+)\"\\s*:\\s*"
                + "(true|false|-?[0-9.eE+]+|\"(?:[^\"\\\\]|\\\\.)*\")").matcher(content);
            while (m.find()) {
                values.put(m.group(1), m.group(2));
            }
        } catch (Exception e) {
            LOG.warning("Failed to read assistant settings: " + e.getMessage());
        }
        return values;
    }

    private static void save() {
        File f = settingsFile();
        if (f == null) {
            return;
        }
        try {
            f.getParentFile().mkdirs();
            StringBuilder sb = new StringBuilder("{");
            boolean first = true;
            for (Map.Entry<String, String> e : values.entrySet()) {
                if (!first) {
                    sb.append(",");
                }
                first = false;
                sb.append("\n  \"").append(e.getKey()).append("\": ").append(e.getValue());
            }
            sb.append("\n}\n");
            Files.write(f.toPath(), sb.toString().getBytes(StandardCharsets.UTF_8));
        } catch (Exception e) {
            LOG.warning("Failed to write assistant settings: " + e.getMessage());
        }
    }
}
