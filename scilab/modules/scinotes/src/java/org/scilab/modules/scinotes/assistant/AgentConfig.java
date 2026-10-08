package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Logger;

import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

/**
 * The ACP agents catalogue, agents.json: the defaults shipped in
 * SCI/modules/scinotes/etc/assistant, and the user's overrides in
 * SCIHOME/assistant (same format, only what differs: a key there replaces
 * the default one, a new id adds a custom agent). Both files are read again
 * when they change on disk.
 *
 * Per agent: "name", "command" (program names), "args", "dirs", "env".
 * How to install each agent is in the help page (scinotes_assistant).
 */
final class AgentConfig {

    private static final Logger LOG = Logger.getLogger(AgentConfig.class.getName());

    private static final String FILE_NAME = "agents.json";

    /** Keys whose value is a list of strings, and plain string keys. */
    private static final List<String> STRING_LISTS = Arrays.asList("command", "args", "dirs");
    private static final List<String> STRINGS = Arrays.asList("name");
    private static final String ENV = "env";

    private static JsonObject defaults;
    private static long defaultsStamp = -1;
    private static JsonObject overrides;
    private static long overridesStamp = -1;
    /** Why the user's file was ignored, null when it is fine. */
    private static String overridesError;

    private AgentConfig() {
    }

    // -- Files ------------------------------------------------------------

    private static File defaultsFile() {
        File etc = SkillRegistry.getAssistantEtcDir();
        return etc != null ? new File(etc, FILE_NAME) : null;
    }

    private static File overridesFile() {
        File dir = AssistantSettings.assistantDir();
        return dir != null ? new File(dir, FILE_NAME) : null;
    }

    private static long stamp(File f) {
        return f != null && f.isFile() ? f.lastModified() * 31 + f.length() : 0;
    }

    /** Read both files again if they changed on disk. */
    private static void refresh() {
        File d = defaultsFile();
        long ds = stamp(d);
        if (defaults == null || ds != defaultsStamp) {
            String[] error = new String[1];
            defaults = read(d, error);
            defaultsStamp = ds;
            if (error[0] != null) {
                LOG.severe(error[0]);
            }
        }
        File o = overridesFile();
        long os = stamp(o);
        if (overrides == null || os != overridesStamp) {
            String[] error = new String[1];
            overrides = read(o, error);
            overridesStamp = os;
            overridesError = error[0];
            if (error[0] != null) {
                LOG.warning(error[0]);
            }
        }
    }

    /** Parse a file (comments allowed: Gson's lenient mode), {} when missing or invalid. */
    private static JsonObject read(File f, String[] error) {
        if (f == null || !f.isFile()) {
            return new JsonObject();
        }
        try (Reader r = new InputStreamReader(new FileInputStream(f), StandardCharsets.UTF_8)) {
            JsonElement e = JsonParser.parseReader(r);
            if (!e.isJsonObject()) {
                error[0] = f + ": not a JSON object, ignored";
                return new JsonObject();
            }
            return e.getAsJsonObject();
        } catch (Exception e) {
            error[0] = f + ": invalid JSON, ignored (" + e.getMessage() + ")";
            return new JsonObject();
        }
    }

    private static JsonObject section(JsonObject root, String id) {
        JsonElement s = root.get(id);
        return s != null && s.isJsonObject() ? s.getAsJsonObject() : new JsonObject();
    }

    /** A key of an agent: the user's value when set, else the default one. */
    private static synchronized JsonElement value(String id, String key) {
        refresh();
        JsonElement v = section(overrides, id).get(key);
        return v != null && !v.isJsonNull() ? v : section(defaults, id).get(key);
    }

    // -- Values -----------------------------------------------------------

    /** Agent ids: the defaults in file order, then the user's custom agents. */
    static synchronized List<String> ids() {
        refresh();
        Set<String> ids = new LinkedHashSet<>();
        for (JsonObject root : new JsonObject[] {defaults, overrides}) {
            for (Map.Entry<String, JsonElement> e : root.entrySet()) {
                if (e.getValue().isJsonObject()) {
                    ids.add(e.getKey());
                }
            }
        }
        return new ArrayList<>(ids);
    }

    /** A list of strings, empty when not set. */
    static List<String> stringList(String id, String key) {
        List<String> list = new ArrayList<>();
        JsonElement e = value(id, key);
        if (e != null && e.isJsonArray()) {
            for (JsonElement item : e.getAsJsonArray()) {
                if (item.isJsonPrimitive()) {
                    list.add(item.getAsString());
                }
            }
        }
        return list;
    }

    /** A string, null when not set. */
    static String string(String id, String key) {
        JsonElement e = value(id, key);
        return e != null && e.isJsonPrimitive() ? e.getAsString() : null;
    }

    /** Environment variables set for the agent. */
    static Map<String, String> env(String id) {
        Map<String, String> env = new LinkedHashMap<>();
        JsonElement e = value(id, ENV);
        if (e != null && e.isJsonObject()) {
            for (Map.Entry<String, JsonElement> v : e.getAsJsonObject().entrySet()) {
                if (v.getValue().isJsonPrimitive()) {
                    env.put(v.getKey(), v.getValue().getAsString());
                }
            }
        }
        return env;
    }

    // -- Settings page ----------------------------------------------------

    /** The defaults of an agent, as in the SCI file. */
    static synchronized JsonObject defaults(String id) {
        refresh();
        return section(defaults, id).deepCopy();
    }

    /** The user's overrides of an agent, {} when none. */
    static synchronized JsonObject overrides(String id) {
        refresh();
        return section(overrides, id).deepCopy();
    }

    /** True for an agent shipped with Scilab, false for one the user added. */
    static synchronized boolean isBuiltIn(String id) {
        refresh();
        return defaults.has(id);
    }

    /** True when the user's file changes something for this agent. */
    static synchronized boolean isCustomized(String id) {
        refresh();
        return section(overrides, id).size() > 0;
    }

    /** Why the user's file is ignored, null when it is fine. */
    static synchronized String overridesError() {
        refresh();
        return overridesError;
    }

    /**
     * Check an agent section: known keys only, each with the right type.
     * @return the error, null when valid
     */
    static String validate(String id, JsonObject section) {
        for (Map.Entry<String, JsonElement> e : section.entrySet()) {
            String key = e.getKey();
            JsonElement v = e.getValue();
            if (STRING_LISTS.contains(key)) {
                if (!isStringArray(v)) {
                    return id + "." + key + ": an array of strings is expected";
                }
            } else if (STRINGS.contains(key)) {
                if (!v.isJsonPrimitive()) {
                    return id + "." + key + ": a string is expected";
                }
            } else if (ENV.equals(key)) {
                if (!v.isJsonObject()) {
                    return id + ".env: an object {\"NAME\": \"value\"} is expected";
                }
                for (Map.Entry<String, JsonElement> var : v.getAsJsonObject().entrySet()) {
                    if (!var.getValue().isJsonPrimitive()) {
                        return id + ".env." + var.getKey() + ": a string is expected";
                    }
                }
            } else {
                return id + ": unknown key \"" + key + "\"";
            }
        }
        return null;
    }

    private static boolean isStringArray(JsonElement e) {
        if (e == null || !e.isJsonArray()) {
            return false;
        }
        for (JsonElement item : e.getAsJsonArray()) {
            if (!item.isJsonPrimitive()) {
                return false;
            }
        }
        return true;
    }

    /**
     * Replace the user's overrides of agents ({} removes them) and save the
     * file. The sections must have been validated.
     */
    static synchronized void setOverrides(Map<String, JsonObject> sections) throws java.io.IOException {
        refresh();
        if (overridesError != null) {
            // Never overwrite a file the user is editing by hand
            throw new java.io.IOException(overridesError);
        }
        for (Map.Entry<String, JsonObject> s : sections.entrySet()) {
            if (s.getValue().size() == 0) {
                overrides.remove(s.getKey());
            } else {
                overrides.add(s.getKey(), s.getValue());
            }
        }
        File f = overridesFile();
        if (f == null) {
            throw new java.io.IOException("no SCIHOME: the overrides cannot be saved");
        }
        if (overrides.size() == 0) {
            Files.deleteIfExists(f.toPath());
        } else {
            f.getParentFile().mkdirs();
            String json = new GsonBuilder().setPrettyPrinting().create().toJson(overrides);
            Files.write(f.toPath(), (json + "\n").getBytes(StandardCharsets.UTF_8));
        }
        overridesStamp = stamp(f);
    }

    /** JSON array of strings, for the page. */
    static JsonArray toArray(List<String> list) {
        JsonArray a = new JsonArray();
        for (String s : list) {
            a.add(s);
        }
        return a;
    }
}
