package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;
import java.util.regex.Pattern;

/**
 * Discovers and serves assistant skills.
 *
 * A skill is a folder containing a SKILL.md file with a YAML-ish frontmatter
 * holding a one-line-able description (trigger words included). The system
 * prompt only lists skill names with their description; the LLM fetches the
 * full body on demand through the load_skill MCP tool (progressive disclosure,
 * keeps the prompt small).
 *
 * Sources, in override order:
 * - built-ins: SCI/modules/scinotes/etc/assistant/skills/&lt;name&gt;/SKILL.md
 * - user:      SCIHOME/assistant/skills/&lt;name&gt;/SKILL.md (same name overrides built-in)
 *
 * Files are re-read on each request so skills can be edited live.
 */
final class SkillRegistry {

    private static final Logger LOG = Logger.getLogger(SkillRegistry.class.getName());

    /** Valid skill names: folder names only, no path separators. */
    private static final Pattern NAME_PATTERN = Pattern.compile("[A-Za-z0-9_-]+");

    private SkillRegistry() {
    }

    /** SCI/modules/scinotes/etc/assistant, or null when SCI is unavailable.
     *  The system property is a test hook (standalone harness has no SCI). */
    static File getAssistantEtcDir() {
        String override = System.getProperty("scilab.assistant.etc");
        if (override != null && !override.isEmpty()) {
            return new File(override);
        }
        try {
            File sci = org.scilab.modules.commons.ScilabConstants.SCI;
            return sci != null ? new File(sci, "modules/scinotes/etc/assistant") : null;
        } catch (Throwable t) {
            return null;
        }
    }

    /** SCIHOME skills dir (user skills override built-in ones). */
    static File getUserSkillsDir() {
        File dir = AssistantSettings.assistantDir();
        return dir != null ? new File(dir, "skills") : null;
    }

    /**
     * Renders the available skills as a markdown catalog for the system prompt:
     * one "- `name` — description" line per skill. Empty string when no skill
     * is available.
     */
    static String buildCatalog() {
        Map<String, SkillMeta> skills = discover();
        if (skills.isEmpty()) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (Map.Entry<String, SkillMeta> e : skills.entrySet()) {
            sb.append("- `").append(e.getKey()).append("` — ")
              .append(e.getValue().description).append("\n");
        }
        return sb.toString();
    }

    /**
     * Skills whose frontmatter trigger words appear in the user request,
     * best matches first (at most {@code max}). Used to inject skill bodies
     * straight into the system prompt: models seldom bother calling
     * load_skill for requests they believe they can handle, and some CLI
     * approval layers even reject that call.
     */
    static List<String> matchSkills(String userPrompt, int max) {
        if (userPrompt == null || userPrompt.isEmpty()) {
            return java.util.Collections.emptyList();
        }
        String text = normalize(userPrompt);
        List<String[]> scored = new ArrayList<>();  // {name, hits}
        for (Map.Entry<String, SkillMeta> e : discover().entrySet()) {
            int hits = 0;
            for (String trigger : e.getValue().triggers) {
                if (containsWord(text, normalize(trigger))) {
                    hits++;
                }
            }
            if (hits > 0) {
                scored.add(new String[] {e.getKey(), String.valueOf(hits)});
            }
        }
        scored.sort((a, b) -> Integer.parseInt(b[1]) - Integer.parseInt(a[1]));
        List<String> names = new ArrayList<>();
        for (String[] s : scored) {
            if (names.size() >= max) {
                break;
            }
            names.add(s[0]);
        }
        return names;
    }

    /** Lowercase, accents stripped: "légende" and "legende" both match. */
    private static String normalize(String s) {
        String n = java.text.Normalizer.normalize(s.toLowerCase(), java.text.Normalizer.Form.NFD);
        return n.replaceAll("\\p{M}+", "");
    }

    /** Whole-word (or whole-phrase) containment: letters/digits on either
     *  side of a match reject it ("importer" does not trigger "import"). */
    private static boolean containsWord(String text, String word) {
        if (word.isEmpty()) {
            return false;
        }
        int idx = 0;
        while ((idx = text.indexOf(word, idx)) >= 0) {
            int end = idx + word.length();
            boolean okBefore = idx == 0 || !Character.isLetterOrDigit(text.charAt(idx - 1));
            boolean okAfter = end >= text.length() || !Character.isLetterOrDigit(text.charAt(end));
            if (okBefore && okAfter) {
                return true;
            }
            idx = end;
        }
        return false;
    }

    /**
     * Loads the full body of a skill (frontmatter stripped).
     * @param name the skill folder name
     * @return the skill content, or null if not found or the name is invalid
     */
    static String loadSkill(String name) {
        if (name == null || !NAME_PATTERN.matcher(name).matches()) {
            return null;
        }
        // User skills override built-ins
        String body = readSkillFile(getUserSkillsDir(), name);
        if (body == null) {
            File etcDir = getAssistantEtcDir();
            body = readSkillFile(etcDir != null ? new File(etcDir, "skills") : null, name);
        }
        return body != null ? stripFrontmatter(body) : null;
    }

    // -----------------------------------------------------------------

    /** Frontmatter of one skill, as the registry uses it. */
    private static final class SkillMeta {
        final String description;
        final List<String> triggers;

        SkillMeta(String description, List<String> triggers) {
            this.description = description;
            this.triggers = triggers;
        }
    }

    /** name -> frontmatter, built-ins first, user skills override. */
    private static Map<String, SkillMeta> discover() {
        Map<String, SkillMeta> skills = new LinkedHashMap<>();
        File etcDir = getAssistantEtcDir();
        if (etcDir != null) {
            collect(new File(etcDir, "skills"), skills);
        }
        collect(getUserSkillsDir(), skills);
        return skills;
    }

    private static void collect(File skillsDir, Map<String, SkillMeta> out) {
        if (skillsDir == null) {
            return;
        }
        File[] dirs = skillsDir.listFiles(File::isDirectory);
        if (dirs == null) {
            return;
        }
        for (File dir : dirs) {
            if (!NAME_PATTERN.matcher(dir.getName()).matches()) {
                continue;
            }
            String body = readText(new File(dir, "SKILL.md"));
            if (body != null) {
                List<String> triggers = new ArrayList<>();
                String raw = extractFrontmatterField(body, "triggers");
                if (raw != null) {
                    for (String t : raw.split(",")) {
                        t = t.trim();
                        if (!t.isEmpty()) {
                            triggers.add(t);
                        }
                    }
                }
                out.put(dir.getName(), new SkillMeta(extractDescription(body), triggers));
            }
        }
    }

    private static String readSkillFile(File skillsDir, String name) {
        if (skillsDir == null) {
            return null;
        }
        return readText(new File(new File(skillsDir, name), "SKILL.md"));
    }

    private static String readText(File file) {
        if (file == null || !file.isFile()) {
            return null;
        }
        try {
            return new String(Files.readAllBytes(file.toPath()), StandardCharsets.UTF_8);
        } catch (Exception e) {
            LOG.warning("Failed to read " + file + ": " + e.getMessage());
            return null;
        }
    }

    /**
     * Extracts the frontmatter "description:" value, collapsed to one line.
     * Falls back to the first heading or non-empty line of the body.
     */
    private static String extractDescription(String content) {
        String desc = extractFrontmatterField(content, "description");
        if (desc != null) {
            return desc;
        }
        return firstHeadingOrLine(stripFrontmatter(content));
    }

    /**
     * One frontmatter field, collapsed to a single line. Supports inline
     * values and block scalars (field: |). Null when absent or empty.
     */
    private static String extractFrontmatterField(String content, String field) {
        String[] lines = content.split("\\R");
        if (lines.length == 0 || !"---".equals(lines[0].trim())) {
            return null;
        }
        StringBuilder value = new StringBuilder();
        boolean inField = false;
        for (int i = 1; i < lines.length; i++) {
            String line = lines[i];
            if ("---".equals(line.trim())) {
                break;
            }
            if (inField) {
                if (line.startsWith(" ") || line.startsWith("\t")) {
                    if (value.length() > 0) {
                        value.append(' ');
                    }
                    value.append(line.trim());
                    continue;
                }
                inField = false;
            }
            if (line.startsWith(field + ":")) {
                String v = line.substring(field.length() + 1).trim();
                if (v.isEmpty() || "|".equals(v) || ">".equals(v)) {
                    inField = true;
                } else {
                    value.append(v);
                }
            }
        }
        return value.length() > 0 ? value.toString() : null;
    }

    private static String firstHeadingOrLine(String body) {
        for (String line : body.split("\\R", 8)) {
            String t = line.trim();
            if (t.isEmpty()) {
                continue;
            }
            if (t.startsWith("#")) {
                return t.replaceAll("^#+\\s*", "");
            }
            return t.length() > 100 ? t.substring(0, 100) + "..." : t;
        }
        return "(no description)";
    }

    /** Removes the leading --- ... --- frontmatter block, if any. */
    private static String stripFrontmatter(String content) {
        String[] lines = content.split("\\R", -1);
        if (lines.length == 0 || !"---".equals(lines[0].trim())) {
            return content;
        }
        for (int i = 1; i < lines.length; i++) {
            if ("---".equals(lines[i].trim())) {
                StringBuilder sb = new StringBuilder();
                for (int j = i + 1; j < lines.length; j++) {
                    if (sb.length() > 0) {
                        sb.append('\n');
                    }
                    sb.append(lines[j]);
                }
                return sb.toString().trim();
            }
        }
        return content;
    }
}
