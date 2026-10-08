package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Base64;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.graphic_objects.ScilabNativeView;

/**
 * MCP tool definitions and dispatch.
 *
 * Defines the tools exposed to the LLM and executes them
 * via CommandServer (Scilab interpreter) or directly via SciNotes Java API.
 */
final class McpToolHandler {

    private static final Logger LOG = Logger.getLogger(McpToolHandler.class.getName());

    private McpToolHandler() {
    }

    /** Optional "path" argument of the editor tools. */
    private static final String PATH_PROP =
        "\"path\":{\"type\":\"string\",\"description\":\"Path of an OPEN editor tab, as on the 'File:' "
        + "line of get_editor_content (default: the file you are working on)\"}";

    /**
     * Editor tab the editor tools act on. Set to the active tab when the user
     * sends a request, then moved by open_file_in_editor and get_editor_content
     * with a path. The tools must NOT follow the tab displayed at call time:
     * the user naturally clicks another tab (e.g. to run it) while the model
     * works, and set_editor_content then overwrote that other file.
     */
    private static java.lang.ref.WeakReference<org.scilab.modules.scinotes.ScilabEditorPane> targetPane =
        new java.lang.ref.WeakReference<>(null);
    /** True once a tab is pinned for the request: if that tab disappears
     *  (closed by the user), the tools fail instead of moving to another
     *  file — they must never overwrite a file the user did not give. */
    private static boolean pinned;
    /** File of the pinned tab ("" for an untitled buffer), for the messages. */
    private static String pinnedName = "";

    /**
     * Text of each document as the agent last saw it (in the request's
     * editor context, get_editor_content, or after its own edit): a whole
     * replacement is refused when the user changed the document meanwhile.
     */
    private static final java.util.Map<javax.swing.text.Document, Integer> seenText =
        java.util.Collections.synchronizedMap(new java.util.WeakHashMap<>());

    /**
     * Pin the editor tools to the tab active now. Called on the EDT when a
     * user request is sent (the request carries the content of that tab).
     */
    static void pinActiveEditor() {
        org.scilab.modules.scinotes.ScilabEditorPane pane = getActivePane();
        seenText.clear();
        pin(pane);
        if (pane != null) {
            markSeen(pane);
        }
    }

    private static void pin(org.scilab.modules.scinotes.ScilabEditorPane pane) {
        targetPane = new java.lang.ref.WeakReference<>(pane);
        pinned = pane != null;
        pinnedName = pane != null && pane.getName() != null ? pane.getName() : "";
    }

    /** Remember the text of the pane as the agent sees it now. */
    private static void markSeen(org.scilab.modules.scinotes.ScilabEditorPane pane) {
        seenText.put(pane.getDocument(), pane.getText().hashCode());
    }

    /** Null when the agent saw the current text (or never read the pane),
     *  else the error telling it to read the file again. */
    private static String changedByUser(Target t) {
        Integer seen = seenText.get(t.pane.getDocument());
        if (seen == null || seen == t.pane.getText().hashCode()) {
            return null;
        }
        return "Error: the user changed " + label(t.pane) + " in the editor since you last read it."
            + " Nothing was written. Call get_editor_content, then make your change on the current"
            + " content (prefer edit_editor_text for targeted changes).";
    }

    /** A document change that may fail (BadLocationException). */
    private interface Edit {
        void run() throws javax.swing.text.BadLocationException;
    }

    /**
     * Apply an edit of the assistant as ONE undo step, whatever it does
     * (a replacement is a removal plus an insertion): a single Ctrl+Z
     * reverts it. Monaco owns its undo stack: the edits are grouped there;
     * the Swing editor groups them in a compound edit.
     */
    private static void undoableEdit(org.scilab.modules.scinotes.ScilabEditorPane pane, Edit edit)
        throws javax.swing.text.BadLocationException {
        org.scilab.modules.scinotes.MonacoEditorPane monaco =
            pane instanceof org.scilab.modules.scinotes.MonacoEditorPane
            ? (org.scilab.modules.scinotes.MonacoEditorPane) pane : null;
        org.scilab.modules.scinotes.ScilabDocument doc =
            monaco == null && pane.getDocument() instanceof org.scilab.modules.scinotes.ScilabDocument
            ? (org.scilab.modules.scinotes.ScilabDocument) pane.getDocument() : null;
        if (monaco != null) {
            monaco.beginEditGroup();
        } else if (doc != null) {
            doc.getUndoManager().startCompoundEdit();
        }
        try {
            edit.run();
        } finally {
            if (monaco != null) {
                monaco.endEditGroup();
            } else if (doc != null) {
                doc.getUndoManager().endCompoundEdit();
            }
        }
    }

    /** True when the tab has changes the user did not save. */
    private static boolean unsaved(org.scilab.modules.scinotes.ScilabEditorPane pane) {
        return pane.getDocument() instanceof org.scilab.modules.scinotes.ScilabDocument
            && ((org.scilab.modules.scinotes.ScilabDocument) pane.getDocument()).isContentModified();
    }

    /**
     * Returns the JSON array of tool definitions for tools/list.
     */
    static String getToolsListJson() {
        return "["
            // -- Scilab interpreter --
            + tool("execute_scilab_code",
                   "Execute Scilab code in the active Scilab session. Use for computations, tests, inspections, plots. "
                   + "Results are NOT echoed, even without a trailing semicolon (the code runs "
                   + "through execstr): print what you need to see with disp or mprintf.",
                   "{\"type\":\"object\",\"properties\":{\"code\":{\"type\":\"string\",\"description\":\"Scilab code to execute\"}},\"required\":[\"code\"]}")
            + ","
            + tool("get_variable",
                   "Get the value of a Scilab variable.",
                   "{\"type\":\"object\",\"properties\":{\"name\":{\"type\":\"string\",\"description\":\"Variable name\"}},\"required\":[\"name\"]}")
            + ","
            + tool("list_variables",
                   "List the user variables in the Scilab workspace with their type and size. "
                   + "Use this to discover what is defined before calling get_variable.",
                   "{\"type\":\"object\",\"properties\":{},\"required\":[]}")
            + ","
            + tool("last_error",
                   "Get the last error message recorded by the Scilab interpreter, with where it "
                   + "happened (\"at line 4 of function f\": the line counts from the function "
                   + "line; \"of the executed script\": line in the exec'd file). "
                   + "Useful to diagnose a failure the user reports without re-running the code.",
                   "{\"type\":\"object\",\"properties\":{},\"required\":[]}")
            + ","
            + tool("get_figure",
                   "Capture a Scilab graphic figure as a PNG image. Default mode \"export\" renders "
                   + "the graphics only (axes, curves). Mode \"screen\" takes a screenshot of the "
                   + "figure window as displayed, INCLUDING its uicontrols, menus and embedded HTML "
                   + "(browser) pages: use it to check a GUI. The window is briefly brought to the "
                   + "front, then restored.",
                   "{\"type\":\"object\",\"properties\":{"
                   + "\"figure_id\":{\"type\":\"integer\",\"description\":\"Figure ID (default: current figure)\"},"
                   + "\"mode\":{\"type\":\"string\",\"enum\":[\"export\",\"screen\"],"
                   + "\"description\":\"export (default): graphics only; screen: window screenshot with the GUI\"}"
                   + "},\"required\":[]}")
            + ","
            + tool("scilab_help",
                   "Get full documentation for a Scilab function.",
                   "{\"type\":\"object\",\"properties\":{\"function\":{\"type\":\"string\",\"description\":\"Function name\"}},\"required\":[\"function\"]}")
            + ","
            + tool("scilab_help_search",
                   "Search Scilab documentation by keyword.",
                   "{\"type\":\"object\",\"properties\":{\"query\":{\"type\":\"string\",\"description\":\"Search query\"},\"limit\":{\"type\":\"integer\",\"description\":\"Max results (default 8)\"}},\"required\":[\"query\"]}")
            + ","
            + tool("load_skill",
                   "Load the full body of a skill listed in your system prompt. The prompt only lists "
                   + "skills with a one-line summary; call this tool to fetch the actual instructions "
                   + "when you decide to apply one.",
                   "{\"type\":\"object\",\"properties\":{\"name\":{\"type\":\"string\",\"description\":\"Skill name as listed in the system prompt\"}},\"required\":[\"name\"]}")
            + ","
            // -- Editor --
            + tool("get_editor_content",
                   "Get the content of the editor file you are working on: the file that was "
                   + "active when the user sent the request, or the one you last opened/read. "
                   + "Clicking another tab meanwhile does not change it. Pass path to read another "
                   + "open tab (it then becomes the target of the editing tools). "
                   + "The user may edit the file at any time during the conversation: "
                   + "always re-read before editing if your knowledge of the file may be stale.",
                   "{\"type\":\"object\",\"properties\":{" + PATH_PROP + "},\"required\":[]}")
            + ","
            + tool("edit_editor_text",
                   "Replace an exact text fragment in the editor file you are working on (see "
                   + "get_editor_content), or in the open tab given by path. Preferred over "
                   + "set_editor_content for targeted edits: cheaper and preserves the rest of the file. "
                   + "old_string must match the current content exactly (including whitespace and "
                   + "indentation) and must be unique unless replace_all is true.",
                   "{\"type\":\"object\",\"properties\":{"
                   + "\"old_string\":{\"type\":\"string\",\"description\":\"Exact text to replace\"},"
                   + "\"new_string\":{\"type\":\"string\",\"description\":\"Replacement text (may be empty to delete)\"},"
                   + "\"replace_all\":{\"type\":\"boolean\",\"description\":\"Replace every occurrence (default false: old_string must be unique)\"},"
                   + PATH_PROP
                   + "},\"required\":[\"old_string\",\"new_string\"]}")
            + ","
            + tool("set_editor_content",
                   "Replace the entire content of the editor file you are working on (see "
                   + "get_editor_content), or of the open tab given by path. "
                   + "Prefer edit_editor_text for partial changes.",
                   "{\"type\":\"object\",\"properties\":{\"content\":{\"type\":\"string\",\"description\":\"The new file content\"},"
                   + PATH_PROP + "},\"required\":[\"content\"]}")
            + ","
            + tool("replace_editor_selection",
                   "Replace the selected text in the editor file you are working on (see "
                   + "get_editor_content). If no selection, inserts at cursor.",
                   "{\"type\":\"object\",\"properties\":{\"content\":{\"type\":\"string\",\"description\":\"The replacement text\"}},\"required\":[\"content\"]}")
            + ","
            + tool("open_file_in_editor",
                   "Open a file in a new SciNotes editor tab; it becomes the file the editing tools act on.",
                   "{\"type\":\"object\",\"properties\":{\"path\":{\"type\":\"string\",\"description\":\"File path\"},\"line\":{\"type\":\"integer\",\"description\":\"Line number (optional)\"}},\"required\":[\"path\"]}")
            + "]";
    }

    /**
     * Call a tool by name with the given JSON arguments.
     * @return the tool result as text
     */
    static String callTool(String name, String argsJson) {
        LOG.info("Tool call: " + name + " args=" + argsJson);
        SessionLogger.toolCall(name, argsJson);
        long t0 = System.currentTimeMillis();
        String result = dispatchTool(name, argsJson);
        SessionLogger.toolResult(name, resultPreview(result),
                                 System.currentTimeMillis() - t0);
        return result;
    }

    /** Compact form of a tool result for the session log. */
    private static String resultPreview(String result) {
        if (result != null && result.startsWith("![figure](data:image/png;base64,")) {
            return "[png figure, " + result.length() + " chars]";
        }
        return result;
    }

    private static String dispatchTool(String name, String argsJson) {
        switch (name) {
            case "execute_scilab_code":
                return executeScilabCode(argsJson);
            case "get_variable":
                return getVariable(argsJson);
            case "list_variables":
                return listVariables();
            case "last_error":
                return lastError();
            case "scilab_help":
                return scilabHelp(argsJson);
            case "scilab_help_search":
                return scilabHelpSearch(argsJson);
            case "load_skill":
                return loadSkill(argsJson);
            case "get_figure":
                return getFigure(argsJson);
            case "get_editor_content":
                return getEditorContent(argsJson);
            case "edit_editor_text":
                return editEditorText(argsJson);
            case "set_editor_content":
                return setEditorContent(argsJson);
            case "replace_editor_selection":
                return replaceEditorSelection(argsJson);
            case "open_file_in_editor":
                return openFileInEditor(argsJson);
            default:
                return "Unknown tool: " + name;
        }
    }

    // --- Scilab interpreter tools ---

    private static String executeScilabCode(String argsJson) {
        String code = JsonUtil.extractString(argsJson, "code");
        if (code == null || code.isEmpty()) {
            return "Error: missing 'code' argument";
        }

        String result = CommandServer.executeCommand(code, false);
        if (result == null) {
            return "Error: failed to queue command";
        }
        return result;
    }

    private static String getVariable(String argsJson) {
        String name = JsonUtil.extractString(argsJson, "name");
        if (name == null || name.isEmpty()) {
            return "Error: missing 'name' argument";
        }

        String result = CommandServer.executeCommand("disp(" + name + ")", false, 60_000);
        if (result == null) {
            return "Error: failed to get variable '" + name + "'";
        }
        return result;
    }

    /**
     * List user variables (name, type, size) using who_user() to filter
     * out predefined/protected variables. Temp variables are %_-prefixed
     * and cleared afterwards to keep the workspace clean.
     */
    private static String listVariables() {
        String code =
            "%_n = who_user(%f);\n"
            + "for %_k = 1:size(%_n, \"*\")\n"
            + "    %_t = \"?\"; %_s = \"?\";\n"
            + "    execstr(\"%_t = typeof(\" + %_n(%_k) + \")\", \"errcatch\");\n"
            // size() errors on functions, mlists and objects without a
            // %<type>_size overload, and the error would replace the user's
            // one in lasterror: only call it on native types or overloads
            + "    %_y = -1;\n"
            + "    execstr(\"%_y = type(\" + %_n(%_k) + \")\", \"errcatch\");\n"
            + "    if or(%_y == [1 2 4 5 6 8 10 15 16]) | isdef(\"%\" + %_t + \"_size\") then\n"
            + "        execstr(\"%_s = strcat(string(size(\" + %_n(%_k) + \")), \"\"x\"\")\", \"errcatch\");\n"
            + "    end\n"
            + "    mprintf(\"%-24s %-14s %s\\n\", %_n(%_k), %_t, %_s);\n"
            + "end\n"
            + "clear %_n %_k %_t %_s %_y";
        String result = CommandServer.executeCommand(code, true, 60_000);
        if (result == null) {
            return "Error: failed to list variables";
        }
        if (result.trim().isEmpty()) {
            return "No user variables in the workspace.";
        }
        return "Name                     Type           Size\n" + result;
    }

    /**
     * Return the last recorded interpreter error without clearing it.
     */
    private static String lastError() {
        // Message + location ("at line 4 of function badfun")
        String code = CommandServer.LASTERROR_WITH_LOCATION
            + "if size(%_m, \"*\") > 0 then mprintf(\"%s\\n\", %_m); end\n"
            + "clear %_m %_n %_l %_f";
        String result = CommandServer.executeCommand(code, true, 30_000);
        if (result == null) {
            return "Error: failed to query last error";
        }
        if (result.trim().isEmpty()) {
            return "No error recorded in this Scilab session.";
        }
        return result;
    }

    private static String getFigure(String argsJson) {
        String figIdStr = JsonUtil.extractString(argsJson, "figure_id");
        int figNum;
        if (figIdStr != null && !figIdStr.isEmpty()) {
            try {
                figNum = Integer.parseInt(figIdStr.trim());
            } catch (NumberFormatException e) {
                return "Error: invalid figure_id '" + figIdStr + "'";
            }
        } else {
            // A bare expression prints nothing under execstr: mprintf it.
            // Guard on winsid() so gcf() never creates an empty figure.
            String numStr = CommandServer.executeCommand(
                "if winsid() <> [] then mprintf(\"%d\", gcf().figure_id); end", true, 30_000);
            if (numStr == null || numStr.trim().isEmpty()) {
                return "Error: no figure open";
            }
            numStr = numStr.trim();
            if (numStr.startsWith("ERROR:")) {
                return "Error: no figure open (" + numStr + ")";
            }
            try {
                figNum = Integer.parseInt(numStr.replaceAll("[^0-9]", ""));
            } catch (NumberFormatException e) {
                return "Error: could not determine current figure number: " + numStr;
            }
        }

        int figUID = ScilabNativeView.ScilabNativeView__getFigureFromIndex(figNum);
        if (figUID == 0) {
            return "Error: figure " + figNum + " not found";
        }

        if ("screen".equals(JsonUtil.extractString(argsJson, "mode"))) {
            String shot = FigureScreenCapture.capture(figUID);
            if (shot != null) {
                return shot;
            }
            // Window not displayable (hidden tab, locked screen...): fall
            // back to the export, which renders the graphics only.
            LOG.info("get_figure: screen capture unavailable, exporting figure " + figNum);
        }

        String tmpDir = System.getProperty("java.io.tmpdir").replace("\\", "/");
        String pngPath = tmpDir + "/assistant_figure_" + System.currentTimeMillis() + ".png";

        String exportErr = FileExporter.fileExport(figUID, pngPath, "png", 1.0f, 0);
        if (exportErr != null && !exportErr.isEmpty()) {
            return "Error: " + exportErr;
        }

        File pngFile = new File(pngPath);
        if (!pngFile.exists()) {
            return "Error: figure export produced no file";
        }
        try {
            byte[] pngBytes = Files.readAllBytes(pngFile.toPath());
            String b64 = Base64.getEncoder().encodeToString(pngBytes);
            pngFile.delete();
            return "![figure](data:image/png;base64," + b64 + ")";
        } catch (IOException e) {
            return "Error reading PNG: " + e.getMessage();
        }
    }

    private static String scilabHelp(String argsJson) {
        String func = JsonUtil.extractString(argsJson, "function");
        if (func == null || func.isEmpty()) {
            return "Error: missing 'function' argument";
        }
        String help = ScilabHelpIndex.getInstance().getHelp(func);
        if (help == null) {
            List<ScilabHelpIndex.SearchResult> results =
                ScilabHelpIndex.getInstance().search(func, 5);
            if (results.isEmpty()) {
                return "No documentation found for '" + func + "'";
            }
            StringBuilder sb = new StringBuilder();
            sb.append("No exact match for '").append(func).append("'. Did you mean:\n");
            for (ScilabHelpIndex.SearchResult r : results) {
                sb.append("- ").append(r.name);
                if (!r.purpose.isEmpty()) sb.append(": ").append(r.purpose);
                sb.append("\n");
            }
            return sb.toString();
        }
        return help;
    }

    private static String scilabHelpSearch(String argsJson) {
        String query = JsonUtil.extractString(argsJson, "query");
        if (query == null || query.isEmpty()) {
            return "Error: missing 'query' argument";
        }
        String limitStr = JsonUtil.extractString(argsJson, "limit");
        int limit = 8;
        if (limitStr != null) {
            try { limit = Integer.parseInt(limitStr); } catch (NumberFormatException e) {}
        }
        limit = Math.max(1, Math.min(20, limit));

        List<ScilabHelpIndex.SearchResult> results =
            ScilabHelpIndex.getInstance().search(query, limit);
        if (results.isEmpty()) {
            return "No results for '" + query + "'";
        }
        StringBuilder sb = new StringBuilder();
        for (ScilabHelpIndex.SearchResult r : results) {
            sb.append("**").append(r.name).append("**");
            if (!r.purpose.isEmpty()) sb.append(" — ").append(r.purpose);
            if (!r.syntax.isEmpty()) sb.append("\n  `").append(r.syntax.split("\n")[0]).append("`");
            sb.append("\n\n");
        }
        return sb.toString();
    }

    private static String loadSkill(String argsJson) {
        String name = JsonUtil.extractString(argsJson, "name");
        if (name == null || name.isEmpty()) {
            return "Error: missing 'name' argument";
        }
        String body = SkillRegistry.loadSkill(name);
        if (body == null) {
            return "Skill not found: " + name;
        }
        return body;
    }

    // --- Editor tools (SciNotes integration) ---

    /** Editor tab resolved for a tool call, or the error to return. */
    private static final class Target {
        org.scilab.modules.scinotes.SciNotes editor;
        org.scilab.modules.scinotes.ScilabEditorPane pane;
        int index = -1;
        String error;
    }

    /**
     * The open SciNotes windows, the active one first: the single assistant
     * acts on the tabs of all of them.
     */
    private static List<org.scilab.modules.scinotes.SciNotes> editors() {
        List<org.scilab.modules.scinotes.SciNotes> list =
            new java.util.ArrayList<>(org.scilab.modules.scinotes.SciNotes.getSciNotesList());
        org.scilab.modules.scinotes.SciNotes active = org.scilab.modules.scinotes.SciNotes.getActiveEditor();
        if (active != null && list.remove(active)) {
            list.add(0, active);
        }
        return list;
    }

    /** Find the tab holding the pane's document in any window. */
    private static Target locate(org.scilab.modules.scinotes.ScilabEditorPane pane) {
        Target t = new Target();
        for (org.scilab.modules.scinotes.SciNotes ed : editors()) {
            int i = findTab(ed, pane);
            if (i >= 0) {
                t.editor = ed;
                t.index = i;
                t.pane = pane;
                return t;
            }
        }
        return t;
    }

    /** Find the tab of the file {@code path} in any window. */
    private static Target locate(String path) {
        Target t = new Target();
        for (org.scilab.modules.scinotes.SciNotes ed : editors()) {
            int i = findTab(ed, path);
            if (i >= 0) {
                t.editor = ed;
                t.index = i;
                t.pane = ed.getTextPane(i);
                return t;
            }
        }
        return t;
    }

    /**
     * Resolve the tab an editor tool acts on, in any SciNotes window: the
     * open tab named by the "path" argument, else the pinned target tab,
     * else the active one. Must be called on the EDT.
     */
    private static Target resolveTarget(String argsJson) {
        String path = JsonUtil.extractString(argsJson, "path");
        if (path != null && !path.isEmpty()) {
            Target t = locate(path);
            if (t.pane == null) {
                t.error = "Error: no open editor tab for " + path
                    + " — open it first with open_file_in_editor";
                return t;
            }
            pin(t.pane);
            return t;
        }
        org.scilab.modules.scinotes.ScilabEditorPane target = targetPane.get();
        if (target != null) {
            Target t = locate(target);
            if (t.pane != null) {
                return t;
            }
        }
        if (pinned) {
            // The user closed the tab of the file being worked on: never
            // move to another file (it would be overwritten and saved)
            Target t = new Target();
            t.error = pinnedName.isEmpty()
                ? "Error: the user closed the untitled editor tab you were working on. Nothing was"
                  + " written. Do not write into another tab: tell the user and ask what to do."
                : "Error: the user closed the editor tab of " + pinnedName + ". Nothing was written."
                  + " Do not write into another tab: reopen it with open_file_in_editor if it still"
                  + " exists and the user still wants the change, else tell the user.";
            return t;
        }
        // No tab at all when the request was sent: the active one, if any
        org.scilab.modules.scinotes.ScilabEditorPane active = getActivePane();
        Target t = active != null ? locate(active) : new Target();
        if (t.pane == null) {
            t.error = "Error: no editor open";
            return t;
        }
        pin(t.pane);
        return t;
    }

    /** Index of the tab holding the pane's document (split views share it), or -1. */
    private static int findTab(org.scilab.modules.scinotes.SciNotes editor,
                               org.scilab.modules.scinotes.ScilabEditorPane pane) {
        int n = editor.getTabPane().getTabCount();
        for (int i = 0; i < n; i++) {
            org.scilab.modules.scinotes.ScilabEditorPane p = editor.getTextPane(i);
            if (p != null && p.getDocument() == pane.getDocument()) {
                return i;
            }
        }
        return -1;
    }

    /** Index of the tab whose file is {@code path}, or -1. */
    private static int findTab(org.scilab.modules.scinotes.SciNotes editor, String path) {
        String wanted = canonical(path);
        int n = editor.getTabPane().getTabCount();
        for (int i = 0; i < n; i++) {
            org.scilab.modules.scinotes.ScilabEditorPane p = editor.getTextPane(i);
            if (p != null && p.getName() != null && !p.getName().isEmpty()
                    && wanted.equals(canonical(p.getName()))) {
                return i;
            }
        }
        return -1;
    }

    private static String canonical(String path) {
        try {
            return new File(path).getCanonicalPath();
        } catch (IOException e) {
            return new File(path).getAbsolutePath();
        }
    }

    /** Short name of the tab's file, for the tool results. */
    private static String label(org.scilab.modules.scinotes.ScilabEditorPane pane) {
        String name = pane.getName();
        return (name == null || name.isEmpty()) ? "the untitled buffer" : new File(name).getName();
    }

    /**
     * Return the content of the target editor tab.
     */
    private static String getEditorContent(String argsJson) {
        return runOnEDT(() -> {
            Target t = resolveTarget(argsJson);
            if (t.error != null) return t.error;
            String header = AssistantMessageHandler.describeActiveFile(t.pane.getName());
            org.scilab.modules.scinotes.ScilabEditorPane active = getActivePane();
            if (active != null && active.getDocument() != t.pane.getDocument()) {
                header += "(The user is currently viewing another tab: the editor tools still act on this file.)\n";
            }
            if (t.pane.getName() != null && !t.pane.getName().isEmpty() && !new File(t.pane.getName()).exists()) {
                header += "(This file was deleted on disk: your changes stay in the editor, unsaved.)\n";
            }
            markSeen(t.pane);
            return header + "\n" + t.pane.getText();
        });
    }

    /**
     * Replace an exact text fragment in the target editor tab.
     * The whole replacement is one compound edit: a single Ctrl+Z reverts it.
     */
    private static String editEditorText(String argsJson) {
        String oldStr = JsonUtil.extractString(argsJson, "old_string");
        String newStr = JsonUtil.extractString(argsJson, "new_string");
        boolean replaceAll = "true".equals(JsonUtil.extractString(argsJson, "replace_all"));
        if (oldStr == null || oldStr.isEmpty()) {
            return "Error: missing 'old_string' argument";
        }
        if (newStr == null) {
            return "Error: missing 'new_string' argument";
        }
        return runOnEDT(() -> {
            Target t = resolveTarget(argsJson);
            if (t.error != null) return t.error;
            org.scilab.modules.scinotes.ScilabDocument doc =
                (org.scilab.modules.scinotes.ScilabDocument) t.pane.getDocument();
            String text = doc.getText(0, doc.getLength());

            // Non-overlapping occurrences, scanned forward
            java.util.List<Integer> positions = new java.util.ArrayList<>();
            for (int i = text.indexOf(oldStr); i >= 0; i = text.indexOf(oldStr, i + oldStr.length())) {
                positions.add(i);
            }
            if (positions.isEmpty()) {
                return "Error: old_string not found in " + label(t.pane) + ". The file may have changed"
                    + " — use get_editor_content to re-read the current content.";
            }
            if (positions.size() > 1 && !replaceAll) {
                return "Error: old_string matches " + positions.size() + " locations in " + label(t.pane) + "."
                    + " Add surrounding context to make it unique, or set replace_all=true.";
            }

            undoableEdit(t.pane, () -> {
                // Replace from last to first so earlier positions stay valid
                for (int i = positions.size() - 1; i >= 0; i--) {
                    doc.replace(positions.get(i), oldStr.length(), newStr, null);
                }
            });
            markSeen(t.pane);
            int n = positions.size();
            return "OK: replaced " + n + " occurrence" + (n > 1 ? "s" : "") + " in " + label(t.pane)
                + saveTab(t);
        });
    }

    /**
     * Replace the entire content of the target editor tab.
     */
    private static String setEditorContent(String argsJson) {
        String content = JsonUtil.extractString(argsJson, "content");
        if (content == null) {
            return "Error: missing 'content' argument";
        }
        return runOnEDT(() -> {
            Target t = resolveTarget(argsJson);
            if (t.error != null) return t.error;
            String changed = changedByUser(t);
            if (changed != null) return changed;
            // Only the part that differs is replaced: one undo step, and the
            // caret, scroll and markers of the unchanged parts stay
            javax.swing.text.Document doc = t.pane.getDocument();
            String old = doc.getText(0, doc.getLength());
            int p = 0;
            int max = Math.min(old.length(), content.length());
            while (p < max && old.charAt(p) == content.charAt(p)) {
                p++;
            }
            int q = 0;
            while (q < max - p && old.charAt(old.length() - 1 - q) == content.charAt(content.length() - 1 - q)) {
                q++;
            }
            final int from = p;
            final int removed = old.length() - p - q;
            final String inserted = content.substring(p, content.length() - q);
            if (removed > 0 || !inserted.isEmpty()) {
                undoableEdit(t.pane, () -> {
                    if (doc instanceof javax.swing.text.AbstractDocument) {
                        ((javax.swing.text.AbstractDocument) doc).replace(from, removed, inserted, null);
                    } else {
                        doc.remove(from, removed);
                        doc.insertString(from, inserted, null);
                    }
                });
            }
            markSeen(t.pane);
            return "OK: content of " + label(t.pane) + " replaced (" + content.length() + " chars)"
                + saveTab(t);
        });
    }

    /**
     * Replace the selection of the target editor tab, or insert at its cursor.
     */
    private static String replaceEditorSelection(String argsJson) {
        String content = JsonUtil.extractString(argsJson, "content");
        if (content == null) {
            return "Error: missing 'content' argument";
        }
        return runOnEDT(() -> {
            Target t = resolveTarget("{}");
            if (t.error != null) return t.error;
            String changed = changedByUser(t);
            if (changed != null) return changed;
            undoableEdit(t.pane, () -> t.pane.replaceSelection(content));
            markSeen(t.pane);
            return "OK: selection of " + label(t.pane) + " replaced (" + content.length() + " chars)"
                + saveTab(t);
        });
    }

    /**
     * Open a file in a new SciNotes editor tab (or reload it if already
     * open) and make it the target of the editor tools.
     */
    private static String openFileInEditor(String argsJson) {
        String path = JsonUtil.extractString(argsJson, "path");
        if (path == null || path.isEmpty()) {
            return "Error: missing 'path' argument";
        }
        String lineStr = JsonUtil.extractString(argsJson, "line");
        int line = 0;
        if (lineStr != null) {
            try { line = Integer.parseInt(lineStr); } catch (NumberFormatException e) {}
        }
        final int targetLine = line;

        java.io.File file = new java.io.File(path);
        if (!file.exists()) {
            return "Error: file not found: " + path;
        }

        return runOnEDT(() -> {
            // Already open in any window: reload it from disk
            String absPath = file.getAbsolutePath();
            Target t = locate(absPath);
            if (t.pane != null) {
                boolean keep = unsaved(t.pane);
                if (!keep) {
                    t.editor.reload(t.index);
                }
                t.editor.getTabPane().setSelectedIndex(t.index);
                org.scilab.modules.scinotes.ScilabEditorPane pane = t.editor.getTextPane(t.index);
                if (targetLine > 0) {
                    pane.scrollTextToLineNumber(targetLine, true);
                }
                pin(pane);
                return keep
                    ? "OK: " + file.getName() + " is already open with changes the user did not save:"
                      + " kept as is, not reloaded from disk. Read it with get_editor_content."
                    : "OK: reloaded " + file.getName() + (targetLine > 0 ? " at line " + targetLine : "");
            }

            // Else in the window the user works in
            org.scilab.modules.scinotes.SciNotes editor =
                org.scilab.modules.scinotes.SciNotes.getActiveEditor();
            if (editor == null) return "Error: no editor open";
            editor.openFile(absPath, targetLine, null);
            int i = findTab(editor, absPath);
            if (i < 0) {
                return "Error: could not open " + path;
            }
            pin(editor.getTextPane(i));
            return "OK: opened " + file.getName() + (targetLine > 0 ? " at line " + targetLine : "");
        });
    }

    // --- Editor helpers ---

    /**
     * Get the active tab of the SciNotes window the user works in.
     * Must be called on the EDT.
     */
    private static org.scilab.modules.scinotes.ScilabEditorPane getActivePane() {
        org.scilab.modules.scinotes.SciNotes editor =
            org.scilab.modules.scinotes.SciNotes.getActiveEditor();
        if (editor == null) return null;
        return editor.getTextPane();
    }

    /**
     * Save the target tab in its SciNotes window, unless its file was
     * deleted on disk meanwhile (the user's choice: not recreated behind
     * their back). Must be called on the EDT.
     * @return the end of the tool result: " and saved", or why not
     */
    private static String saveTab(Target t) {
        String name = t.pane.getName();
        if (name == null || name.isEmpty()) {
            return " (untitled buffer, not saved to disk)";
        }
        if (!new File(name).exists()) {
            return ", NOT saved: " + new File(name).getName() + " was deleted on disk. The change is"
                + " only in the editor: tell the user, who can save it or close the tab.";
        }
        if (t.editor != null && t.index >= 0) {
            t.editor.save(t.index, true);
        }
        return " and saved";
    }

    /**
     * Run a task on the EDT and wait for the result.
     * MCP tool calls happen on background threads but Swing access must be on EDT.
     */
    private static String runOnEDT(java.util.concurrent.Callable<String> task) {
        if (SwingUtilities.isEventDispatchThread()) {
            try { return task.call(); } catch (Exception e) { return "Error: " + e.getMessage(); }
        }
        AtomicReference<String> result = new AtomicReference<>("Error: EDT timeout");
        try {
            SwingUtilities.invokeAndWait(() -> {
                try {
                    result.set(task.call());
                } catch (Exception e) {
                    result.set("Error: " + e.getMessage());
                }
            });
        } catch (Exception e) {
            return "Error: " + e.getMessage();
        }
        return result.get();
    }

    // --- Helper ---

    private static String tool(String name, String description, String inputSchema) {
        return "{\"name\":\"" + name + "\""
            + ",\"description\":\"" + JsonUtil.escape(description) + "\""
            + ",\"inputSchema\":" + inputSchema + "}";
    }
}
