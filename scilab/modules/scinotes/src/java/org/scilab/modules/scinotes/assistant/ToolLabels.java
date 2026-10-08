package org.scilab.modules.scinotes.assistant;

/**
 * Display labels of the scilab MCP tools for the activity feed. These are
 * our own tools: whatever prefix or decoration an agent adds to their name
 * ("mcp__scilab__x", "scilab-x", "mcp.scilab.x", "scilab_x: x"...), the
 * bare name is found in it.
 */
final class ToolLabels {

    /** Bare tool name, label, input key previewed after the label (or null). */
    private static final String[][] TOOLS = {
        {"execute_scilab_code", "Executing Scilab code", "code"},
        {"get_figure", "Capturing figure", null},
        {"get_variable", "Reading variable", "name"},
        {"list_variables", "Listing variables", null},
        {"last_error", "Reading last error", null},
        {"scilab_help_search", "Searching help", "query"},
        {"scilab_help", "Reading help", "function_name"},
        {"load_skill", "Loading skill", "name"},
        {"get_editor_content", "Reading editor", null},
        {"get_editor_selection", "Reading selection", null},
        {"get_editor_filename", "Reading filename", null},
        {"edit_editor_text", "Editing in editor", "old_string"},
        {"set_editor_content", "Writing to editor", "content"},
        {"replace_editor_selection", "Replacing selection", "content"},
        {"open_file_in_editor", "Opening file", "path"},
    };

    private ToolLabels() {
    }

    private static String[] entry(String bareName) {
        for (String[] t : TOOLS) {
            if (t[0].equals(bareName)) {
                return t;
            }
        }
        return null;
    }

    /** Label of a bare scilab tool name, null when it is not one of ours. */
    static String label(String bareName) {
        String[] t = entry(bareName);
        return t != null ? t[1] : null;
    }

    /** Input key worth previewing for a bare scilab tool name, or null. */
    static String contextKey(String bareName) {
        String[] t = entry(bareName);
        return t != null ? t[2] : null;
    }

    /**
     * The scilab tool named somewhere in a decorated tool title, or null.
     * The longest match wins (scilab_help_search before scilab_help), and a
     * match must not be glued to other letters.
     */
    static String findIn(String title) {
        if (title == null) {
            return null;
        }
        String best = null;
        for (String[] t : TOOLS) {
            int i = title.indexOf(t[0]);
            while (i >= 0) {
                int end = i + t[0].length();
                boolean startOk = i == 0 || !Character.isLetterOrDigit(title.charAt(i - 1));
                boolean endOk = end == title.length()
                                || !(Character.isLetterOrDigit(title.charAt(end)) || title.charAt(end) == '_');
                if (startOk && endOk && (best == null || t[0].length() > best.length())) {
                    best = t[0];
                    break;
                }
                i = title.indexOf(t[0], i + 1);
            }
        }
        return best;
    }
}
