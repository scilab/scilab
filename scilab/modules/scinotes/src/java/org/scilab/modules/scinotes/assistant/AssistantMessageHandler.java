package org.scilab.modules.scinotes.assistant;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefQueryCallback;
import org.cef.handler.CefMessageRouterHandlerAdapter;

import java.util.logging.Logger;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

/**
 * Handles messages from the JCEF browser (JS -> Java bridge).
 *
 * JS calls: assistantQuery({request: jsonString, onSuccess, onFailure})
 * This handler parses the JSON, dispatches to AgentProcess (with the
 * selected AI provider), and returns results via callback.success(jsonResponse).
 */
public final class AssistantMessageHandler extends CefMessageRouterHandlerAdapter {

    private static final Logger LOG = Logger.getLogger(AssistantMessageHandler.class.getName());

    /** Tool calls allowed per request. Each file edit counts: building a
     *  small GUI app step (html + js + css + sce, then tests) needs ~40-50,
     *  and 30 cut every such step before the model could conclude. */
    private static final int MAX_AGENT_TURNS = 60;
    /** assistant.json keys of the provider chosen, and of its model and
     *  effort (+ provider id). */
    private static final String SELECTED_PROVIDER = "provider";
    private static final String SELECTED_MODEL = "model.";
    private static final String SELECTED_EFFORT = "effort.";
    private static final Gson GSON = new Gson();

    private static final String SYSTEM_PROMPT =
        "You are a Scilab coding assistant integrated into the SciNotes editor. "
        + "Always write code in the Scilab language unless the user explicitly asks for another language. "
        + "Reply in the language of the user's latest message (e.g. French if they write in French), "
        + "even though these instructions are in English. "
        + "The CODE you write stays in English by default, whatever the conversation language: "
        + "variable and function names, comments, printed/error messages, plot titles and labels. "
        + "Use another language in code only if the user asks for it, or when editing an existing "
        + "file already written in that language (stay consistent with it). "
        + "Make user-facing strings translatable with _() (alias of gettext()): messages of "
        + "mprintf/error/warning/messagebox/disp and plot titles/labels. Wrap the whole FORMAT "
        + "string, never the concatenation of pieces: mprintf(_(\"%d files read.\\n\"), n), "
        + "error(msprintf(_(\"%s: Wrong value for input argument #%d.\\n\"), \"myfun\", 1)). "
        + "In a toolbox, use its own domain: gettext(\"mytoolbox\", \"...\") (there is no _d). "
        + "Do not wrap non-text data (file names, variable names, format-only strings like \"%g\"). "
        + "\n\n"
        + "## Editor context\n"
        + "You are embedded as a side panel next to the code editor. "
        + "The current file content and selection are automatically provided as context below. "
        + "When the user says 'this code', 'my code', 'this function' — refer to that context directly. "
        + "Do NOT ask the user to paste their code.\n"
        + "\n"
        + "## Scilab-specific MCP tools\n"
        + "You have these tools that interact with the live Scilab session and the SciNotes editor:\n"
        + "- execute_scilab_code: run code in the Scilab interpreter\n"
        + "- list_variables: list user variables in the workspace (name, type, size)\n"
        + "- get_variable: get a variable's value\n"
        + "- last_error: get the last error recorded by the interpreter\n"
        + "- get_figure: capture a plot as image (always use after generating a plot)\n"
        + "- scilab_help / scilab_help_search: look up Scilab documentation\n"
        + "- load_skill: load the full instructions of a skill listed below\n"
        + "- get_editor_content: re-read the current file from the editor\n"
        + "- edit_editor_text: replace an exact text fragment in the editor (preferred for edits)\n"
        + "- set_editor_content: replace the entire editor buffer (only for full rewrites)\n"
        + "- replace_editor_selection: replace the text currently selected by the user\n"
        + "- open_file_in_editor: open a file in a new SciNotes tab\n"
        + "\n"
        + "## How to work\n"
        + "- ACT, don't just suggest: when the user asks for code that creates or changes "
        + "something (a plot, a script, a function), write it into the editor with the editor "
        + "tools, run it with execute_scilab_code, and check plots with get_figure — iterate "
        + "until the result is correct. Answer with a chat-only code snippet ONLY when the "
        + "user explicitly asks for an explanation or an example\n"
        + "- Do NOT repeat code you already wrote into the editor (or a file) in your final "
        + "reply: the user sees it in the editor, pasting it again drowns the useful "
        + "information. Reply with a short summary — what changed, the key choices, how to "
        + "run it. A code block in the reply is only for tiny fragments the user must type "
        + "themselves\n"
        + "- The user references files inline with @ (e.g. 'charge @data/pop.csv'). A "
        + "<referenced_files> block at the top of the message maps each @mention to its absolute "
        + "path ('@rel = /abs/path'). This is a reference, not a request to ingest the file: use "
        + "the absolute path directly in Scilab code (csvRead, fscanfMat, ...) when that is what "
        + "the task needs; read the file with your Read tool only when you actually need its content.\n"
        + "- Documents the user attaches (PDF, Excel, ...) are saved locally and listed in an "
        + "<attached_files> block ('name = /abs/path'). Read a PDF with your Read tool; load "
        + "data files in Scilab (xlsxRead, parquetRead, csvRead, load, ...) as the task requires.\n"
        + "- To modify the file the user is editing: prefer edit_editor_text (exact old/new fragments); "
        + "use set_editor_content only to rewrite the whole file\n"
        + "- The user can edit the file while you work: call get_editor_content to re-read it before "
        + "making edits if your last view of the file is not from this turn\n"
        + "- To create a new file: use your Write tool in the 'Working directory (pwd)' given with "
        + "the editor context (or next to the active file when it is a real saved file and the new "
        + "one belongs with it), then open_file_in_editor to show it. NEVER create files in "
        + "Scilab's TMPDIR: an untitled buffer's temporary copy lives there, it is not the user's folder\n"
        + "- Never overwrite a file that holds the user's work on ANOTHER topic: when a request "
        + "starts something new and the active file already contains unrelated code, write the new "
        + "script to a new file instead of set_editor_content\n"
        + "- To run/test code: use execute_scilab_code\n"
        + "- To run the file open in the editor: take its path from the 'File:' line returned by "
        + "get_editor_content (for an untitled buffer, its temporary copy) and call "
        + "exec(\"<that path>\", -1). Never guess or build the path "
        + "(no get_absolute_file_path, no TMPDIR tricks)\n"
        + "- When debugging: use list_variables to see what is defined, get_variable to inspect values, "
        + "and last_error to see the most recent interpreter error\n"
        + "- To create or edit files on disk: use your built-in Read, Write and Edit tools\n"
        + "- Stay focused on the user's request. Do NOT explore the surrounding filesystem or source tree to 'research' how Scilab works -- use scilab_help / scilab_help_search for that instead\n"
        + "- After generating a plot, ALWAYS use get_figure to see the result\n"
        + "\n"
        + "## Scilab pitfalls (all domains)\n"
        + "- `$` exists ONLY inside an index: L(1:min(8,$)) fails; write "
        + "L(1:min(8, size(L, \"*\")))\n"
        + "- find(x, \"last\") does not exist: k = find(x); k($)\n"
        + "- No ~ placeholder for ignored outputs: [~, k] = max(x) is a syntax error; "
        + "write [_, k] = max(x)\n"
        + "- Both ' and \" delimit strings: a quote INSIDE a string must be doubled whatever the "
        + "delimiter: \"it''s\", \"say \"\"hi\"\"\" (\"d'un\" is a parse error)\n"
        + "- Never name a variable or parameter like a function (type, size, length, sum, max, "
        + "min, find, string, disp, error, f...): by dynamic scoping the library macros you call "
        + "then see YOUR variable and fail with 'Invalid index'\n"
        + "- Write files as UTF-8 WITHOUT BOM (use your Write tool; PowerShell Out-File/Set-Content "
        + "-Encoding utf8 adds a BOM that shows up as a stray character in SciNotes)\n"
        + "- grep(s, pat, \"r\") needs slash-delimited regexps: grep(s, [\"/NA/\", \"/,$/\"], \"r\"); "
        + "without \"r\" the patterns are plain substrings\n"
        + "\n"
        + "## Removed or obsolete functions — NEVER use them (old tutorials still do)\n"
        + "Removed (error 'Undefined variable'), with the replacement:\n"
        + "- stacksize, gstacksize: nothing needed (memory is dynamic)\n"
        + "- xdel, xbasc, xclear: close(winsid()) / clf() — xset, xget, xgetech: gcf()/gca() properties "
        + "— xpause: sleep — xtape: nothing\n"
        + "- plot2d1: plot2d with logflag — fplot2d: plot(x, f(x)) — champ1: champ — "
        + "fcontour2d: contour2d — square: gca().isoview = \"on\"\n"
        + "- captions: legend — x_message: messagebox — havewindow: getscilabmode()\n"
        + "- getf: exec — eval: evstr/execstr — errcatch, iserror: try/catch or execstr(..., \"errcatch\")\n"
        + "- str2code, code2str: ascii — hypermat: zeros(n, m, p) — lex_sort, sort: gsort\n"
        + "- st_deviation: stdev — mvvacov: cov — princomp: pca — mfft: fft\n"
        + "- nanmax, nanmin: max/min already ignore %nan (nanmean, nanmedian, nanstdev exist; "
        + "there is NO \"omitnan\" option)\n"
        + "- dassl, dasrt, daskr, impl: dae — testmatrix: magic/invhilb/frank — help(): doc()\n"
        + "- readb, writb, readc_: mget/mput/mgetl — fort: call — mtlb_load, mtlb_save: "
        + "loadmatfile/savematfile\n"
        + "- demo_begin, demo_end, demo_run: exec — demo_choose, demo_file_choice: x_choose\n"
        + "Obsolete (still run with a warning, removed in 2027.0): dos, unix, unix_g, unix_s, "
        + "unix_w, unix_x -> host(); readmps -> quapro toolbox\n"
        + "MATLAB names that never existed: num2str/int2str -> string\n"
        + "nargin/nargout DO exist, but only as variables injected in a function's scope "
        + "(same as argn()): usable inside a function, undefined at console/script level\n"
        + "\n"
        + "Keep answers concise. Format code blocks with ```scilab.";

    /** Current session ID for conversation resume. Empty = new conversation. */
    private String currentSessionId = "";

    /** Agent of the current conversation (session IDs are not portable
     *  across agents). */
    private String currentProviderId = defaultAgent().id;

    /** The first agent of the catalogue. */
    private static AcpAgent defaultAgent() {
        java.util.List<AcpAgent> agents = AcpAgent.all();
        if (agents.isEmpty()) {
            java.io.File etc = SkillRegistry.getAssistantEtcDir();
            throw new IllegalStateException("no ACP agent in the catalogue: "
                + (etc != null ? new java.io.File(etc, "agents.json") : "agents.json") + " is missing or invalid");
        }
        return agents.get(0);
    }

    /** The agent of this id, or the default one when unknown. */
    private static AcpAgent agentOrDefault(String id) {
        AcpAgent a = AcpAgent.find(id);
        return a != null ? a : defaultAgent();
    }

    @Override
    public boolean onQuery(CefBrowser browser, CefFrame frame, long queryId,
                           String request, boolean persistent, CefQueryCallback callback) {
        try {
            return handleMessage(request, callback);
        } catch (Exception e) {
            LOG.warning("Error handling message: " + e.getMessage());
            callback.success("{\"error\":\"" + escapeJson(e.getMessage()) + "\"}");
            return true;
        }
    }

    private boolean handleMessage(String request, CefQueryCallback callback) {
        // Parse the type field from JSON
        String type = extractString(request, "\"type\"");
        if (type == null) {
            callback.success("{\"error\":\"missing type\"}");
            return true;
        }

        switch (type) {
            case "ask":
                return handleAsk(request, callback);
            case "poll":
                return handlePoll(request, callback);
            case "cancel":
                return handleCancel(request, callback);
            case "get_providers":
                return handleGetProviders(callback);
            case "probe_provider":
                return handleProbeProvider(request, callback);
            case "insert_code":
                return handleInsertCode(request, callback);
            case "run_code":
                return handleRunCode(request, callback);
            case "list_files":
                return handleListFiles(request, callback);
            case "read_file":
                return handleReadFile(request, callback);
            case "clear":
                currentSessionId = "";
                SessionLogger.newConversation();
                callback.success("{\"success\":true}");
                return true;
            case "resume":
                String sid = extractString(request, "\"session_id\"");
                if (sid != null) currentSessionId = sid;
                String rprov = extractString(request, "\"provider\"");
                if (rprov != null && AcpAgent.find(rprov) != null) {
                    currentProviderId = rprov;
                }
                SessionLogger.resumedConversation(currentSessionId, currentProviderId);
                callback.success("{\"success\":true}");
                return true;
            case "stage_file":
                return handleStageFile(request, callback);
            case "get_settings":
                callback.success("{\"settings\":{\"session_logs\":" + SessionLogger.isEnabled()
                    + ",\"open_with_scinotes\":" + AssistantPanel.isOpenWithSciNotes()
                    + "},\"logs_dir\":\"" + escapeJson(SessionLogger.logsDirPath()) + "\"}");
                return true;
            case "set_setting":
                return handleSetSetting(request, callback);
            case "set_provider_path":
                return handleSetProviderPath(request, callback);
            case "set_selection":
                return handleSetSelection(request, callback);
            case "get_provider_config":
                return handleGetProviderConfig(request, callback);
            case "set_provider_overrides":
                return handleSetProviderOverrides(request, callback);
            case "apply_provider_fix":
                return handleApplyProviderFix(request, callback);
            case "answer": {
                // The user's choice on a permission or sign-in card
                JsonObject req = parseRequest(request);
                JsonElement rid = req.get("request_id");
                String qid = stringOf(req, "question_id");
                String value = stringOf(req, "value");
                if (rid != null && rid.isJsonPrimitive() && qid != null && value != null) {
                    AgentProcess.answer(rid.getAsInt(), qid, value);
                }
                callback.success("{\"success\":true}");
                return true;
            }
            case "agent_sign_in":
                return handleAgentSignIn(request, callback);
            case "add_agent":
                return handleAddAgent(request, callback);
            case "remove_agent":
                return handleRemoveAgent(request, callback);
            case "open_help":
                // Queued, not awaited: the interpreter may be busy with a request
                org.scilab.modules.action_binding.InterpreterManagement.requestScilabExec("doc(\"scinotes_assistant\")");
                callback.success("{\"success\":true}");
                return true;
            case "ready":
                // Frontend loaded — send initial editor context
                org.scilab.modules.scinotes.SciNotes.refreshAssistantContext();
                callback.success("{\"success\":true}");
                return true;
            case "save_conversation":
                return handleSaveConversation(request, callback);
            case "load_conversation":
                return handleLoadConversation(request, callback);
            case "list_conversations":
                return handleListConversations(callback);
            case "delete_conversation":
                return handleDeleteConversation(request, callback);
            default:
                callback.success("{\"error\":\"unknown type: " + escapeJson(type) + "\"}");
                return true;
        }
    }

    private boolean handleAsk(String request, CefQueryCallback callback) {
        String prompt = extractString(request, "\"prompt\"");
        if (prompt == null || prompt.isEmpty()) {
            // Typical cause: a huge inline attachment truncated the CEF
            // query, so the prompt string never closes. Leave a trace —
            // this used to be undiagnosable (nothing reached the logs).
            LOG.warning("ask rejected: no parsable prompt (request "
                        + request.length() + " chars)");
            SessionLogger.askRejected(request.length(), request);
            callback.success("{\"error\":\"missing prompt (a too-large or binary "
                + "attachment can truncate the request \\u2014 use an @path "
                + "mention instead)\"}");
            return true;
        }

        // Provider switch: session IDs are not portable across providers,
        // the conversation context restarts with the new backend
        String providerId = extractString(request, "\"provider\"");
        AcpAgent provider = agentOrDefault(providerId);
        if (!provider.id.equals(currentProviderId)) {
            currentProviderId = provider.id;
            currentSessionId = "";
        }

        // "" leaves the agent's own default
        String model = extractString(request, "\"model\"");
        if (model == null) model = "";
        String effort = extractString(request, "\"effort\"");
        if (effort == null) effort = "";

        // Extract images array if present: "images":["data:image/png;base64,...",...]
        java.util.List<String> images = extractStringArray(request, "\"images\"");

        // The conversation as shown in the panel, sent when the agent may
        // have lost it (conversation reopened, agent changed): used only if
        // the agent cannot resume its own session
        String history = stringOf(parseRequest(request), "history");

        // Build dynamic system prompt with Scilab knowledge, skills catalog
        // (+ auto-injected matching skills) + editor context
        java.util.List<String> injectedSkills = new java.util.ArrayList<>();
        String systemPrompt = buildSystemPrompt(prompt, injectedSkills);

        String workingDir = computeWorkingDir();

        int taskId = AgentProcess.start(provider, prompt, model, effort, systemPrompt,
                                        MAX_AGENT_TURNS, currentSessionId, images, workingDir,
                                        history);

        SessionLogger.turnStart(taskId, provider.id, model, effort,
                                currentSessionId, prompt, systemPrompt);

        // Return task ID for polling, plus the auto-loaded skills so the
        // activity feed can show them
        StringBuilder resp = new StringBuilder("{\"streaming\":true,\"request_id\":")
            .append(taskId);
        if (!injectedSkills.isEmpty()) {
            resp.append(",\"skills\":[");
            for (int i = 0; i < injectedSkills.size(); i++) {
                if (i > 0) resp.append(",");
                resp.append("\"").append(escapeJson(injectedSkills.get(i))).append("\"");
            }
            resp.append("]");
        }
        resp.append("}");
        callback.success(resp.toString());
        return true;
    }

    /**
     * Save an attached file's bytes ({"name","data_b64"}) to a local
     * directory and return its absolute path: the prompt then references
     * the path (<attached_files>) and the CLI reads the file natively —
     * inlining binary content is what used to truncate the ask request.
     * Decoding can be several MB: answered from a background thread.
     */
    private boolean handleStageFile(String request, CefQueryCallback callback) {
        String name = extractString(request, "\"name\"");
        String b64 = extractString(request, "\"data_b64\"");
        if (name == null || name.isEmpty() || b64 == null || b64.isEmpty()) {
            callback.success("{\"error\":\"missing name or data (file too large?)\"}");
            return true;
        }
        final String safeName = name.replaceAll("[\\\\/:*?\"<>|]", "_");
        new Thread(() -> {
            try {
                byte[] bytes = java.util.Base64.getDecoder().decode(b64);
                java.io.File dir = new java.io.File(
                    System.getProperty("java.io.tmpdir"),
                    "scilab-assistant-attachments");
                dir.mkdirs();
                java.io.File f = new java.io.File(dir, safeName);
                if (f.exists()) {
                    // Never overwrite a file an earlier turn may still reference
                    f = new java.io.File(dir, System.currentTimeMillis() + "_" + safeName);
                }
                java.nio.file.Files.write(f.toPath(), bytes);
                SessionLogger.fileStaged(f.getAbsolutePath(), bytes.length);
                callback.success("{\"path\":\"" + escapeJson(f.getAbsolutePath()) + "\"}");
            } catch (Exception e) {
                LOG.warning("stage_file failed for " + safeName + ": " + e.getMessage());
                callback.success("{\"error\":\"could not save file: "
                    + escapeJson(e.getMessage()) + "\"}");
            }
        }, "assistant-stage-file").start();
        return true;
    }

    /** Persist one assistant setting ({"key":"session_logs","value":true}). */
    private boolean handleSetSetting(String request, CefQueryCallback callback) {
        String key = extractString(request, "\"key\"");
        String value = extractString(request, "\"value\"");
        if ("session_logs".equals(key)) {
            SessionLogger.setEnabled("true".equals(value));
            callback.success("{\"success\":true}");
        } else if ("open_with_scinotes".equals(key)) {
            AssistantPanel.setOpenWithSciNotes("true".equals(value));
            callback.success("{\"success\":true}");
        } else {
            callback.success("{\"error\":\"unknown setting: " + escapeJson(key) + "\"}");
        }
        return true;
    }

    private boolean handlePoll(String request, CefQueryCallback callback) {
        String idStr = extractNumber(request, "\"request_id\"");
        if (idStr == null) {
            callback.success("{\"success\":false,\"error\":\"missing request_id\"}");
            return true;
        }

        int taskId = Integer.parseInt(idStr);
        boolean done = AgentProcess.isComplete(taskId);

        if (done) {
            String result = AgentProcess.getResult(taskId);
            double cost = AgentProcess.getCost(taskId);
            long tokensIn = AgentProcess.getTokensIn(taskId);
            long tokensOut = AgentProcess.getTokensOut(taskId);
            String error = AgentProcess.getError(taskId);
            String sessionId = AgentProcess.getSessionId(taskId);
            SessionLogger.events(taskId, AgentProcess.drainEvents(taskId));
            SessionLogger.turnEnd(taskId, sessionId, result, error,
                                  cost, tokensIn, tokensOut);
            AgentProcess.cleanup(taskId);

            // Store session ID for conversation continuity
            if (sessionId != null && !sessionId.isEmpty()) {
                currentSessionId = sessionId;
            }

            // Refresh any editor tabs whose files may have been modified externally
            refreshOpenEditors();

            if (error != null && !error.isEmpty()) {
                callback.success("{\"success\":false,\"done\":true,\"error\":\"" + escapeJson(error) + "\"}");
            } else {
                // Base64 encode to avoid JSON escaping issues with complex markdown
                String b64 = java.util.Base64.getEncoder().encodeToString(
                    ("{\"success\":true,\"done\":true,\"final_result\":\""
                     + escapeJson(result) + "\",\"cost\":" + cost
                     + ",\"tokens_in\":" + tokensIn
                     + ",\"tokens_out\":" + tokensOut
                     + ",\"session_id\":\"" + escapeJson(currentSessionId) + "\"}")
                    .getBytes(java.nio.charset.StandardCharsets.UTF_8));
                callback.success("{\"json_b64\":\"" + b64 + "\"}");
            }
        } else {
            String events = AgentProcess.drainEvents(taskId);
            SessionLogger.events(taskId, events);
            long liveTokens = AgentProcess.getLiveTokens(taskId);
            callback.success("{\"success\":true,\"done\":false,\"events\":" + events
                + ",\"live_tokens\":" + liveTokens + "}");
        }

        return true;
    }

    private boolean handleCancel(String request, CefQueryCallback callback) {
        String idStr = extractNumber(request, "\"request_id\"");
        if (idStr != null) {
            int taskId = Integer.parseInt(idStr);
            AgentProcess.cancel(taskId);
            SessionLogger.cancelled(taskId);
        }
        callback.success("{\"success\":true,\"cancelled\":true}");
        return true;
    }

    /**
     * Set (or clear, with an empty path) the program of an agent, for
     * programs installed outside the auto-detected locations. The file must
     * exist; quotes around a pasted path are dropped.
     */
    private boolean handleSetProviderPath(String request, CefQueryCallback callback) {
        AcpAgent p = AcpAgent.find(extractString(request, "\"provider\""));
        if (p == null) {
            callback.success("{\"error\":\"unknown provider\"}");
            return true;
        }
        String path = extractString(request, "\"path\"");
        path = path == null ? "" : path.trim();
        if (path.length() >= 2 && path.startsWith("\"") && path.endsWith("\"")) {
            path = path.substring(1, path.length() - 1).trim();
        }
        if (!path.isEmpty() && !new java.io.File(path).isFile()) {
            callback.success("{\"error\":\"" + escapeJson("file not found: " + path) + "\"}");
            return true;
        }
        AssistantSettings.setString("acp_path." + p.id, path);
        callback.success("{\"success\":true}");
        return true;
    }

    /**
     * Provider list for the panel. Looking the CLIs up touches the disk:
     * answered from a background thread so that the panel never freezes
     * while it loads.
     */
    private boolean handleGetProviders(CefQueryCallback callback) {
        new Thread(() -> callback.success("{\"providers\":" + agentsJson()
            + ",\"current\":\"" + escapeJson(currentProviderId) + "\""
            + ",\"selection\":" + selectionJson() + "}"),
            "assistant-providers").start();
        return true;
    }

    /**
     * The agents for the panel: [{"id","name","found","path","custom_path",
     * "customized","custom"}]. Models and efforts are not known
     * before the agent runs: probe_provider gives them.
     */
    private static String agentsJson() {
        JsonArray list = new JsonArray();
        for (AcpAgent a : AcpAgent.all()) {
            String path = a.binary();
            String custom = a.customPath();
            JsonObject o = new JsonObject();
            o.addProperty("id", a.id);
            o.addProperty("name", a.name);
            o.addProperty("found", path != null);
            o.addProperty("path", path != null ? path : "");
            o.addProperty("custom_path", custom != null ? custom : "");
            o.addProperty("customized", AgentConfig.isCustomized(a.id));
            o.addProperty("custom", !AgentConfig.isBuiltIn(a.id));
            list.add(o);
        }
        return GSON.toJson(list);
    }

    /**
     * The provider chosen by the user, and the last model and effort used
     * with each provider, saved in assistant.json:
     * {"provider":"claude","models":{"claude":"sonnet"},"efforts":{"claude":"high"}};
     * provider is null when the user never chose one.
     */
    private static String selectionJson() {
        JsonObject selection = new JsonObject();
        selection.addProperty(SELECTED_PROVIDER, AssistantSettings.getString(SELECTED_PROVIDER));
        JsonObject models = new JsonObject();
        JsonObject efforts = new JsonObject();
        for (AcpAgent p : AcpAgent.all()) {
            models.addProperty(p.id, AssistantSettings.getString(SELECTED_MODEL + p.id));
            efforts.addProperty(p.id, AssistantSettings.getString(SELECTED_EFFORT + p.id));
        }
        selection.add("models", models);
        selection.add("efforts", efforts);
        return GSON.toJson(selection);
    }

    /**
     * Save the provider, model and effort chosen in the panel
     * ({"provider":"claude","model":"sonnet","effort":"high"}; model
     * optional, an empty value goes back to the agent's default).
     */
    private boolean handleSetSelection(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        AcpAgent p = AcpAgent.find(stringOf(req, "provider"));
        if (p == null) {
            callback.success("{\"error\":\"unknown provider\"}");
            return true;
        }
        AssistantSettings.setString(SELECTED_PROVIDER, p.id);
        String model = stringOf(req, "model");
        if (model != null) {
            // "" goes back to the agent's default
            AssistantSettings.setString(SELECTED_MODEL + p.id, model);
        }
        String effort = stringOf(req, "effort");
        if (effort != null) {
            AssistantSettings.setString(SELECTED_EFFORT + p.id, effort);
        }
        callback.success("{\"success\":true}");
        return true;
    }

    // -- Command line of the agents (agents.json) -------------------------

    /**
     * The command line configuration of an agent for the settings page:
     * {"provider", "defaults": {...SCI section}, "overrides": {...SCIHOME
     * section}, "command": [...], "env": {...}, "error"?}.
     */
    private boolean handleGetProviderConfig(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        AcpAgent p = AcpAgent.find(stringOf(req, "provider"));
        if (p == null) {
            callback.success("{\"error\":\"unknown provider\"}");
            return true;
        }
        // Looks the program up on disk: not on the CEF thread
        new Thread(() -> callback.success(GSON.toJson(agentConfigJson(p.id, null))),
                   "assistant-provider-config").start();
        return true;
    }

    private static JsonObject agentConfigJson(String id, String error) {
        AcpAgent a = AcpAgent.find(id);
        JsonObject o = new JsonObject();
        o.addProperty("provider", id);
        o.add("defaults", AgentConfig.defaults(id));
        o.add("overrides", AgentConfig.overrides(id));
        java.util.List<String> cmd = a != null ? a.commandLine(a.binary()) : null;
        if (cmd != null) {
            o.add("command", AgentConfig.toArray(cmd));
        }
        JsonObject env = new JsonObject();
        for (java.util.Map.Entry<String, String> v : AgentConfig.env(id).entrySet()) {
            env.addProperty(v.getKey(), v.getValue());
        }
        o.add("env", env);
        if (error == null) {
            error = AgentConfig.overridesError();
        }
        if (error != null) {
            o.addProperty("error", error);
        }
        return o;
    }

    /**
     * Replace the user's overrides of an agent, edited in the settings
     * page: {"provider", "overrides": {...}} ({} resets the agent). Answers
     * like get_provider_config.
     */
    private boolean handleSetProviderOverrides(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        AcpAgent p = AcpAgent.find(stringOf(req, "provider"));
        JsonElement overrides = req.get("overrides");
        if (p == null || overrides == null || !overrides.isJsonObject()) {
            callback.success("{\"error\":\"provider and overrides expected\"}");
            return true;
        }
        java.util.Map<String, JsonObject> sections = new java.util.LinkedHashMap<>();
        sections.put(p.id, overrides.getAsJsonObject());
        String error = saveOverrides(sections);
        callback.success(GSON.toJson(agentConfigJson(p.id, error)));
        return true;
    }

    /**
     * "Paste a fix": a snippet given on a forum, {"claude": {...}, ...} (a
     * new id with a "command" adds a custom agent), or a bare agent section
     * applied to the current agent. Its keys are merged into the user's
     * overrides. Nothing is saved when one part is invalid. Answers like
     * get_provider_config for the current agent, with "changed": the agents
     * modified.
     */
    private boolean handleApplyProviderFix(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        AcpAgent current = AcpAgent.find(stringOf(req, "provider"));
        String text = stringOf(req, "fix");
        if (current == null || text == null) {
            callback.success("{\"error\":\"provider and fix expected\"}");
            return true;
        }
        JsonObject fix;
        try {
            JsonElement e = JsonParser.parseString(text);
            if (!e.isJsonObject()) {
                throw new IllegalArgumentException("a JSON object {...} is expected");
            }
            fix = e.getAsJsonObject();
        } catch (Exception e) {
            callback.success(GSON.toJson(agentConfigJson(current.id, "Invalid fix: " + e.getMessage())));
            return true;
        }
        // Keyed by agent ids, or one bare section for the current agent
        boolean byAgent = fix.size() > 0;
        for (String key : fix.keySet()) {
            byAgent &= fix.get(key).isJsonObject()
                       && (AcpAgent.find(key) != null || fix.getAsJsonObject(key).has("command"));
        }
        java.util.Map<String, JsonObject> sections = new java.util.LinkedHashMap<>();
        if (byAgent) {
            for (String id : fix.keySet()) {
                sections.put(id, merge(AgentConfig.overrides(id), fix.getAsJsonObject(id)));
            }
        } else {
            sections.put(current.id, merge(AgentConfig.overrides(current.id), fix));
        }
        String error = saveOverrides(sections);
        JsonObject answer = agentConfigJson(current.id, error);
        if (error == null) {
            answer.add("changed", AgentConfig.toArray(new java.util.ArrayList<>(sections.keySet())));
        }
        callback.success(GSON.toJson(answer));
        return true;
    }

    /**
     * Add an agent of the user's choice: {"name", "program", "args": [...],
     * "env": {...}}. The program is a name looked up on the PATH, or a full
     * path (kept as the agent's executable). Answers {"id"} or {"error"}.
     */
    private boolean handleAddAgent(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        String name = stringOf(req, "name");
        String program = stringOf(req, "program");
        name = name == null ? "" : name.trim();
        program = program == null ? "" : program.trim();
        if (program.length() >= 2 && program.startsWith("\"") && program.endsWith("\"")) {
            program = program.substring(1, program.length() - 1).trim();
        }
        if (name.isEmpty() || program.isEmpty()) {
            callback.success("{\"error\":\"a name and a program are expected\"}");
            return true;
        }
        // A full path is kept as the executable; the catalogue gets its name
        String path = null;
        if (program.contains("/") || program.contains("\\")) {
            java.io.File f = new java.io.File(program);
            if (!f.isFile()) {
                callback.success("{\"error\":\"" + escapeJson("file not found: " + program) + "\"}");
                return true;
            }
            path = f.getAbsolutePath();
            program = f.getName().replaceFirst("\\.(exe|cmd|bat)$", "");
        }
        // Id from the name: lowercase letters, digits and dashes, unique
        String base = name.toLowerCase().replaceAll("[^a-z0-9]+", "-").replaceAll("^-+|-+$", "");
        if (base.isEmpty()) {
            base = "agent";
        }
        String id = base;
        java.util.List<String> ids = AgentConfig.ids();
        for (int i = 2; ids.contains(id); i++) {
            id = base + "-" + i;
        }
        JsonObject section = new JsonObject();
        section.addProperty("name", name);
        section.add("command", AgentConfig.toArray(java.util.Collections.singletonList(program)));
        JsonElement args = req.get("args");
        section.add("args", args != null && args.isJsonArray() ? args : new JsonArray());
        JsonElement env = req.get("env");
        if (env != null && env.isJsonObject() && env.getAsJsonObject().size() > 0) {
            section.add("env", env);
        }
        java.util.Map<String, JsonObject> sections = new java.util.LinkedHashMap<>();
        sections.put(id, section);
        String error = saveOverrides(sections);
        if (error != null) {
            callback.success("{\"error\":\"" + escapeJson(error) + "\"}");
            return true;
        }
        if (path != null) {
            AssistantSettings.setString("acp_path." + id, path);
        }
        callback.success("{\"id\":\"" + escapeJson(id) + "\"}");
        return true;
    }

    /**
     * Sign in to an agent from the settings page: {"provider", "method"}.
     * May open a browser page and wait for the user: answered from a
     * background thread, {"success":true} or {"error"}.
     */
    private boolean handleAgentSignIn(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        AcpAgent agent = AcpAgent.find(stringOf(req, "provider"));
        String method = stringOf(req, "method");
        if (agent == null || method == null) {
            callback.success("{\"error\":\"provider and method expected\"}");
            return true;
        }
        String workingDir = computeWorkingDir();
        new Thread(() -> {
            String error = AgentProcess.signIn(agent, method, workingDir);
            callback.success(error == null ? "{\"success\":true}"
                             : "{\"error\":\"" + escapeJson(error) + "\"}");
        }, "assistant-sign-in").start();
        return true;
    }

    /** Remove an agent the user added: {"provider"}. Shipped agents stay. */
    private boolean handleRemoveAgent(String request, CefQueryCallback callback) {
        JsonObject req = parseRequest(request);
        String id = stringOf(req, "provider");
        if (id == null || AcpAgent.find(id) == null || AgentConfig.isBuiltIn(id)) {
            callback.success("{\"error\":\"only an agent you added can be removed\"}");
            return true;
        }
        java.util.Map<String, JsonObject> sections = new java.util.LinkedHashMap<>();
        sections.put(id, new JsonObject());
        String error = saveOverrides(sections);
        if (error != null) {
            callback.success("{\"error\":\"" + escapeJson(error) + "\"}");
            return true;
        }
        AssistantSettings.setString("acp_path." + id, "");
        AssistantSettings.setString(SELECTED_MODEL + id, "");
        AssistantSettings.setString(SELECTED_EFFORT + id, "");
        AcpClient.stop(id);
        callback.success("{\"success\":true}");
        return true;
    }

    /** The keys of {@code fix} replace those of {@code base} (null removes one). */
    private static JsonObject merge(JsonObject base, JsonObject fix) {
        for (java.util.Map.Entry<String, JsonElement> e : fix.entrySet()) {
            if (e.getValue().isJsonNull()) {
                base.remove(e.getKey());
            } else {
                base.add(e.getKey(), e.getValue());
            }
        }
        return base;
    }

    /** Validate then save overrides; the error, or null once saved. */
    private static String saveOverrides(java.util.Map<String, JsonObject> sections) {
        for (java.util.Map.Entry<String, JsonObject> s : sections.entrySet()) {
            String error = AgentConfig.validate(s.getKey(), s.getValue());
            if (error != null) {
                return error;
            }
        }
        try {
            AgentConfig.setOverrides(sections);
            return null;
        } catch (Exception e) {
            return "Not saved: " + e.getMessage();
        }
    }

    private static JsonObject parseRequest(String request) {
        try {
            JsonElement e = JsonParser.parseString(request);
            return e.isJsonObject() ? e.getAsJsonObject() : new JsonObject();
        } catch (Exception e) {
            return new JsonObject();
        }
    }

    /** A string member, null when absent or not a string. */
    private static String stringOf(JsonObject o, String key) {
        JsonElement e = o.get(key);
        return e != null && e.isJsonPrimitive() ? e.getAsString() : null;
    }

    /**
     * Live check of one agent for the settings page: program lookup, then
     * the agent is started and a session opened (kept for the next request)
     * to read its version, models and efforts. Answered from a background
     * thread.
     */
    private boolean handleProbeProvider(String request, CefQueryCallback callback) {
        String providerId = extractString(request, "\"provider\"");
        AcpAgent agent = AcpAgent.find(providerId);
        String workingDir = computeWorkingDir();
        new Thread(() -> {
            String json = agent != null ? AgentProcess.probe(agent, workingDir)
                : "{\"error\":\"unknown provider: " + JsonUtil.escape(String.valueOf(providerId)) + "\"}";
            String b64 = java.util.Base64.getEncoder().encodeToString(
                json.getBytes(java.nio.charset.StandardCharsets.UTF_8));
            callback.success("{\"json_b64\":\"" + b64 + "\"}");
        }, "assistant-probe-" + providerId).start();
        return true;
    }

    /** Insert code at the cursor (or replace the selection) in the active editor tab. */
    private boolean handleInsertCode(String request, CefQueryCallback callback) {
        String code = extractString(request, "\"code\"");
        if (code == null || code.isEmpty()) {
            callback.success("{\"error\":\"missing code\"}");
            return true;
        }
        final String[] err = new String[1];
        try {
            Runnable r = () -> {
                org.scilab.modules.scinotes.SciNotes ed =
                    org.scilab.modules.scinotes.SciNotes.getActiveEditor();
                org.scilab.modules.scinotes.ScilabEditorPane pane =
                    (ed != null) ? ed.getTextPane() : null;
                if (pane == null) {
                    err[0] = "no editor open";
                    return;
                }
                pane.replaceSelection(code);
            };
            if (javax.swing.SwingUtilities.isEventDispatchThread()) {
                r.run();
            } else {
                javax.swing.SwingUtilities.invokeAndWait(r);
            }
        } catch (Exception e) {
            err[0] = e.getMessage();
        }
        if (err[0] == null) {
            callback.success("{\"success\":true}");
        } else {
            callback.success("{\"error\":\"" + escapeJson(err[0]) + "\"}");
        }
        return true;
    }

    /**
     * Execute code in the Scilab interpreter and return the captured output.
     * Runs on a background thread: the CEF callback is invoked asynchronously
     * so long-running code does not block the browser message router.
     */
    private boolean handleRunCode(String request, CefQueryCallback callback) {
        String code = extractString(request, "\"code\"");
        if (code == null || code.isEmpty()) {
            callback.success("{\"error\":\"missing code\"}");
            return true;
        }
        new Thread(() -> {
            String out = CommandServer.executeCommand(code, false);
            String payload = "{\"success\":true,\"output\":\""
                + escapeJson(out == null ? "" : out) + "\"}";
            String b64 = java.util.Base64.getEncoder().encodeToString(
                payload.getBytes(java.nio.charset.StandardCharsets.UTF_8));
            callback.success("{\"json_b64\":\"" + b64 + "\"}");
        }, "assistant-run-code").start();
        return true;
    }

    private boolean handleListFiles(String request, CefQueryCallback callback) {
        String path = extractString(request, "\"path\"");
        java.io.File baseDir;
        if (path != null && !path.isEmpty()) {
            baseDir = new java.io.File(path);
        } else {
            baseDir = new java.io.File(getScilabPwd());
        }

        if (!baseDir.isDirectory()) {
            callback.success("{\"files\":[],\"base\":\"" + escapeJson(baseDir.getAbsolutePath()) + "\"}");
            return true;
        }

        java.util.List<String> files = new java.util.ArrayList<>();
        listFilesRecursive(baseDir, baseDir, files, 3);
        java.util.Collections.sort(files);

        StringBuilder sb = new StringBuilder();
        sb.append("{\"files\":[");
        for (int i = 0; i < files.size(); i++) {
            if (i > 0) sb.append(",");
            sb.append("\"").append(escapeJson(files.get(i))).append("\"");
        }
        sb.append("],\"base\":\"").append(escapeJson(baseDir.getAbsolutePath())).append("\"}");
        callback.success(sb.toString());
        return true;
    }

    private void listFilesRecursive(java.io.File baseDir, java.io.File dir,
                                     java.util.List<String> result, int maxDepth) {
        if (maxDepth <= 0) return;
        java.io.File[] entries = dir.listFiles();
        if (entries == null) return;

        for (java.io.File f : entries) {
            String name = f.getName();
            if (name.startsWith(".")) continue;
            if (f.isDirectory()) {
                // Skip common non-relevant directories
                if ("node_modules".equals(name) || "target".equals(name)
                    || "__pycache__".equals(name) || ".git".equals(name)) continue;
                listFilesRecursive(baseDir, f, result, maxDepth - 1);
            } else if (f.isFile()) {
                String rel = baseDir.toURI().relativize(f.toURI()).getPath();
                result.add(rel);
            }
        }
    }

    private boolean handleReadFile(String request, CefQueryCallback callback) {
        String path = extractString(request, "\"path\"");
        if (path == null || path.isEmpty()) {
            callback.success("{\"error\":\"missing path\"}");
            return true;
        }

        java.io.File baseDir = new java.io.File(getScilabPwd());
        java.io.File file = new java.io.File(path);
        if (!file.isAbsolute()) {
            file = new java.io.File(baseDir, path);
        }

        // Security: ensure file is under pwd()
        try {
            String canonical = file.getCanonicalPath();
            String baseCanonical = baseDir.getCanonicalPath();
            if (!canonical.startsWith(baseCanonical + java.io.File.separator)
                && !canonical.equals(baseCanonical)) {
                callback.success("{\"error\":\"access denied: outside working directory\"}");
                return true;
            }
        } catch (java.io.IOException e) {
            callback.success("{\"error\":\"" + escapeJson(e.getMessage()) + "\"}");
            return true;
        }

        if (!file.exists() || !file.isFile()) {
            callback.success("{\"error\":\"file not found\"}");
            return true;
        }

        // Size limit: 100KB
        if (file.length() > 100 * 1024) {
            callback.success("{\"error\":\"file too large (max 100KB)\"}");
            return true;
        }

        try {
            byte[] bytes = java.nio.file.Files.readAllBytes(file.toPath());
            String content = new String(bytes, java.nio.charset.StandardCharsets.UTF_8);
            // Base64-encode the whole JSON response to avoid escaping issues
            String json = "{\"content\":\"" + escapeJson(content)
                        + "\",\"name\":\"" + escapeJson(file.getName()) + "\"}";
            String b64 = java.util.Base64.getEncoder().encodeToString(
                json.getBytes(java.nio.charset.StandardCharsets.UTF_8));
            callback.success("{\"json_b64\":\"" + b64 + "\"}");
        } catch (Exception e) {
            callback.success("{\"error\":\"" + escapeJson(e.getMessage()) + "\"}");
        }
        return true;
    }

    // -- Conversation persistence (SCIHOME/assistant/conversations/) --

    private static java.io.File getConversationsDir() {
        String scihome = org.scilab.modules.commons.ScilabCommons.getSCIHOME();
        java.io.File dir = new java.io.File(scihome, "assistant/conversations");
        if (!dir.exists()) dir.mkdirs();
        return dir;
    }

    private static java.io.File getConversationsIndex() {
        String scihome = org.scilab.modules.commons.ScilabCommons.getSCIHOME();
        java.io.File dir = new java.io.File(scihome, "assistant");
        if (!dir.exists()) dir.mkdirs();
        return new java.io.File(dir, "conversations.json");
    }

    /**
     * The id of a conversation in a request: "conversation_id" (the panel's
     * own id), else "session_id" (conversations saved before, keyed by the
     * agent session).
     */
    private static String conversationId(String request) {
        String id = extractString(request, "\"conversation_id\"");
        return id != null && !id.isEmpty() ? id : extractString(request, "\"session_id\"");
    }

    /**
     * Save a conversation: {"conversation_id", "session_id" (the agent
     * session, which may change during the conversation), "title",
     * "provider", "messages"}.
     */
    private boolean handleSaveConversation(String request, CefQueryCallback callback) {
        String sessionId = conversationId(request);
        String agentSession = extractString(request, "\"conversation_id\"") != null
                              ? extractString(request, "\"session_id\"") : null;
        String title = extractString(request, "\"title\"");
        String messages = extractString(request, "\"messages\"");
        if (sessionId == null || sessionId.isEmpty()) {
            callback.success("{\"error\":\"missing conversation_id\"}");
            return true;
        }

        try {
            // Save messages to individual file.
            // Empty means "title-only update" (rename): never overwrite the
            // stored messages with nothing.
            if (messages != null && !messages.isEmpty()) {
                java.io.File msgFile = new java.io.File(getConversationsDir(), sessionId + ".json");
                java.nio.file.Files.write(msgFile.toPath(),
                    messages.getBytes(java.nio.charset.StandardCharsets.UTF_8));
            }

            // Update index (agent recorded so resume selects the right one)
            String prov = extractString(request, "\"provider\"");
            updateConversationIndex(sessionId, title != null ? title : "Untitled", prov, agentSession);

            callback.success("{\"success\":true}");
        } catch (Exception e) {
            callback.success("{\"error\":\"" + escapeJson(e.getMessage()) + "\"}");
        }
        return true;
    }

    private boolean handleLoadConversation(String request, CefQueryCallback callback) {
        String sessionId = conversationId(request);
        if (sessionId == null || sessionId.isEmpty()) {
            callback.success("{\"error\":\"missing session_id\"}");
            return true;
        }

        try {
            java.io.File msgFile = new java.io.File(getConversationsDir(), sessionId + ".json");
            if (!msgFile.exists()) {
                callback.success("{\"messages\":[]}");
                return true;
            }

            String content = new String(java.nio.file.Files.readAllBytes(msgFile.toPath()),
                                        java.nio.charset.StandardCharsets.UTF_8);
            // Content is already a JSON array, pass through via b64 to avoid escaping issues
            String b64 = java.util.Base64.getEncoder().encodeToString(
                ("{\"messages\":" + content + "}").getBytes(java.nio.charset.StandardCharsets.UTF_8));
            callback.success("{\"json_b64\":\"" + b64 + "\"}");
        } catch (Exception e) {
            callback.success("{\"error\":\"" + escapeJson(e.getMessage()) + "\"}");
        }
        return true;
    }

    private boolean handleListConversations(CefQueryCallback callback) {
        try {
            java.io.File indexFile = getConversationsIndex();
            if (!indexFile.exists()) {
                callback.success("{\"conversations\":[]}");
                return true;
            }

            String content = new String(java.nio.file.Files.readAllBytes(indexFile.toPath()),
                                        java.nio.charset.StandardCharsets.UTF_8);
            callback.success("{\"conversations\":" + content + "}");
        } catch (Exception e) {
            callback.success("{\"conversations\":[]}");
        }
        return true;
    }

    private boolean handleDeleteConversation(String request, CefQueryCallback callback) {
        String sessionId = conversationId(request);
        if (sessionId == null) {
            callback.success("{\"error\":\"missing session_id\"}");
            return true;
        }

        try {
            // Delete message file from SCIHOME
            java.io.File msgFile = new java.io.File(getConversationsDir(), sessionId + ".json");
            if (msgFile.exists()) msgFile.delete();

            // Remove from index
            removeFromConversationIndex(sessionId);

            callback.success("{\"success\":true}");
        } catch (Exception e) {
            callback.success("{\"error\":\"" + escapeJson(e.getMessage()) + "\"}");
        }
        return true;
    }

    private static void updateConversationIndex(String sessionId, String title, String provider,
                                                String agentSession) {
        try {
            java.io.File indexFile = getConversationsIndex();
            String content = "[]";
            if (indexFile.exists()) {
                content = new String(java.nio.file.Files.readAllBytes(indexFile.toPath()),
                                     java.nio.charset.StandardCharsets.UTF_8);
            }

            // Title-only updates (rename) carry no provider nor session:
            // keep the ones already recorded for this conversation
            if (provider == null || provider.isEmpty() || agentSession == null || agentSession.isEmpty()) {
                int existingPos = content.indexOf("{\"id\":\"" + JsonUtil.escape(sessionId) + "\"");
                if (existingPos >= 0) {
                    int end = content.indexOf("}", existingPos);
                    if (end > 0) {
                        String entry = content.substring(existingPos, end + 1);
                        if (provider == null || provider.isEmpty()) {
                            provider = JsonUtil.extractString(entry, "provider");
                        }
                        if (agentSession == null || agentSession.isEmpty()) {
                            agentSession = JsonUtil.extractString(entry, "session");
                        }
                    }
                }
            }
            if (provider == null || provider.isEmpty()) {
                provider = defaultAgent().id;
            }

            // Simple: rebuild the JSON array
            // Remove existing entry for this sessionId, then prepend
            StringBuilder sb = new StringBuilder("[");
            sb.append("{\"id\":\"").append(JsonUtil.escape(sessionId))
              .append("\",\"title\":\"").append(JsonUtil.escape(title))
              .append("\",\"provider\":\"").append(JsonUtil.escape(provider))
              .append("\",\"session\":\"").append(JsonUtil.escape(
                  agentSession != null && !agentSession.isEmpty() ? agentSession : sessionId))
              .append("\",\"date\":\"").append(JsonUtil.escape(
                  new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm").format(new java.util.Date())))
              .append("\"}");

            // Parse existing entries (skip the one we're updating)
            int count = 1;
            int idx = 0;
            while (count < 20 && (idx = content.indexOf("{\"id\":", idx)) >= 0) {
                int end = content.indexOf("}", idx);
                if (end < 0) break;
                String entry = content.substring(idx, end + 1);
                String existingId = JsonUtil.extractString(entry, "id");
                if (!sessionId.equals(existingId)) {
                    sb.append(",").append(entry);
                    count++;
                }
                idx = end + 1;
            }

            sb.append("]");
            java.nio.file.Files.write(indexFile.toPath(),
                sb.toString().getBytes(java.nio.charset.StandardCharsets.UTF_8));
        } catch (Exception e) {
            LOG.warning("Failed to update conversation index: " + e.getMessage());
        }
    }

    private static void removeFromConversationIndex(String sessionId) {
        try {
            java.io.File indexFile = getConversationsIndex();
            if (!indexFile.exists()) return;

            String content = new String(java.nio.file.Files.readAllBytes(indexFile.toPath()),
                                         java.nio.charset.StandardCharsets.UTF_8);

            StringBuilder sb = new StringBuilder("[");
            boolean first = true;
            int idx = 0;
            while ((idx = content.indexOf("{\"id\":", idx)) >= 0) {
                int end = content.indexOf("}", idx);
                if (end < 0) break;
                String entry = content.substring(idx, end + 1);
                String existingId = JsonUtil.extractString(entry, "id");
                if (!sessionId.equals(existingId)) {
                    if (!first) sb.append(",");
                    sb.append(entry);
                    first = false;
                }
                idx = end + 1;
            }

            sb.append("]");
            java.nio.file.Files.write(indexFile.toPath(),
                sb.toString().getBytes(java.nio.charset.StandardCharsets.UTF_8));
        } catch (Exception e) {
            LOG.warning("Failed to remove from conversation index: " + e.getMessage());
        }
    }

    // -- Minimal JSON helpers --

    private static String extractString(String json, String key) {
        int pos = json.indexOf(key);
        if (pos < 0) return null;
        int colonPos = json.indexOf(':', pos);
        if (colonPos < 0) return null;
        int start = json.indexOf('"', colonPos + 1);
        if (start < 0) return null;
        start++;
        StringBuilder sb = new StringBuilder();
        for (int i = start; i < json.length(); i++) {
            char c = json.charAt(i);
            if (c == '\\' && i + 1 < json.length()) {
                char next = json.charAt(i + 1);
                switch (next) {
                    case '"': sb.append('"'); break;
                    case '\\': sb.append('\\'); break;
                    case 'n': sb.append('\n'); break;
                    case 'r': sb.append('\r'); break;
                    case 't': sb.append('\t'); break;
                    default: sb.append('\\').append(next); break;
                }
                i++;
            } else if (c == '"') {
                return sb.toString();
            } else {
                sb.append(c);
            }
        }
        return null;
    }

    private static String extractNumber(String json, String key) {
        int pos = json.indexOf(key);
        if (pos < 0) return null;
        int colonPos = json.indexOf(':', pos);
        if (colonPos < 0) return null;
        int start = colonPos + 1;
        while (start < json.length() && json.charAt(start) == ' ') start++;
        StringBuilder sb = new StringBuilder();
        for (int i = start; i < json.length(); i++) {
            char c = json.charAt(i);
            if (c >= '0' && c <= '9') {
                sb.append(c);
            } else {
                break;
            }
        }
        return sb.length() > 0 ? sb.toString() : null;
    }

    /**
     * Extract a JSON array of strings: "key":["val1","val2",...]
     * Returns null if key not found or array is empty.
     */
    private static java.util.List<String> extractStringArray(String json, String key) {
        int pos = json.indexOf(key);
        if (pos < 0) return null;
        int colonPos = json.indexOf(':', pos);
        if (colonPos < 0) return null;
        int bracketPos = json.indexOf('[', colonPos);
        if (bracketPos < 0) return null;

        java.util.List<String> result = new java.util.ArrayList<>();
        int i = bracketPos + 1;
        while (i < json.length()) {
            char c = json.charAt(i);
            if (c == ']') break;
            if (c == '"') {
                // Read a string value
                i++;
                StringBuilder sb = new StringBuilder();
                while (i < json.length()) {
                    char sc = json.charAt(i);
                    if (sc == '\\' && i + 1 < json.length()) {
                        sb.append(json.charAt(i + 1));
                        i += 2;
                    } else if (sc == '"') {
                        break;
                    } else {
                        sb.append(sc);
                        i++;
                    }
                }
                result.add(sb.toString());
            }
            i++;
        }
        return result.isEmpty() ? null : result;
    }

    /**
     * Decide which directory the agent should work in.
     *
     * Uses the Scilab interpreter's current directory (pwd()), which is the most
     * intuitive place for the assistant to create/read files. Falls back to SCIHOME
     * when pwd() is unusable or points inside the Scilab tree (SCI) -- otherwise, when
     * Scilab is launched from sources (or for an unsaved "untitled" buffer where the cwd
     * stays at SCI), the agent treats the source tree as a codebase and roams it instead
     * of answering.
     *
     * @return an existing directory path, or null to inherit the JVM cwd as a last resort.
     */
    static String computeWorkingDir() {
        java.io.File sciHome = org.scilab.modules.commons.ScilabConstants.SCIHOME;
        java.io.File sci = org.scilab.modules.commons.ScilabConstants.SCI;

        String pwd = getScilabPwd();

        try {
            if (pwd != null && !pwd.isEmpty()) {
                java.io.File pwdDir = new java.io.File(pwd);
                if (pwdDir.isDirectory() && !isInside(pwdDir, sci)) {
                    return pwdDir.getCanonicalPath();
                }
            }
        } catch (Exception e) {
            // fall through to SCIHOME
        }

        try {
            if (sciHome != null && sciHome.isDirectory()) {
                return sciHome.getCanonicalPath();
            }
        } catch (Exception e) {
            // fall through
        }
        return null;
    }

    /**
     * Header lines describing the active editor file for the model: its path,
     * whether it is only an unsaved "untitled" buffer (SciNotes keeps such
     * buffers under TMPDIR), and the directory where new files belong.
     * Without the flag the model took the TMPDIR path for the user's project
     * folder and created its files there.
     */
    static String describeActiveFile(String name) {
        java.io.File tmp = org.scilab.modules.commons.ScilabConstants.TMPDIR;
        String workDir = computeWorkingDir();
        // A file under TMPDIR is a scratch copy, unless pwd itself is there:
        // then it is where new files were rightly created
        boolean scratch = name == null || name.isEmpty()
            || (isInside(new java.io.File(name), tmp)
                && (workDir == null || !isInside(new java.io.File(workDir), tmp)));
        StringBuilder sb = new StringBuilder();
        if (scratch) {
            sb.append("File: (untitled, not saved by the user")
              .append(name != null && !name.isEmpty() ? " — temporary copy: " + name : "")
              .append(")\n");
            sb.append("This is a scratch buffer in Scilab's TMPDIR, NOT the user's project folder: ")
              .append("never create files next to it.\n");
        } else {
            sb.append("File: ").append(name).append("\n");
        }
        if (workDir != null) {
            sb.append("Working directory (pwd): ").append(workDir)
              .append(scratch ? " — create new files here\n"
                              : " — new files go here, or next to the file above if they belong with it\n");
        }
        return sb.toString();
    }

    /** True if {@code child} is {@code ancestor} or sits anywhere under it. */
    private static boolean isInside(java.io.File child, java.io.File ancestor) {
        if (ancestor == null || child == null) {
            return false;
        }
        try {
            String a = ancestor.getCanonicalPath() + java.io.File.separator;
            String c = child.getCanonicalPath() + java.io.File.separator;
            return c.startsWith(a);
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Build the system prompt with current editor context injected.
     * Called on the EDT (from JCEF message handler) so Swing access is safe.
     * @param userPrompt the request, matched against the skills' triggers
     * @param injectedOut filled with the names of the auto-loaded skills
     */
    private String buildSystemPrompt(String userPrompt, java.util.List<String> injectedOut) {
        StringBuilder sb = new StringBuilder(SYSTEM_PROMPT);

        // Load shared Scilab knowledge (always injected)
        String knowledge = loadKnowledgeFile();
        if (knowledge != null) {
            sb.append("\n\n").append(knowledge);
        }

        // List available skills — full bodies are fetched on demand via load_skill
        String catalog = SkillRegistry.buildCatalog();
        if (!catalog.isEmpty()) {
            sb.append("\n\n## Available skills\n")
              .append("Specialized instructions, loadable with the load_skill tool. ")
              .append("When the user's request matches a skill's domain, ")
              .append("load it BEFORE answering and follow its instructions.\n")
              .append(catalog);
        }

        // Deterministic injection: skills whose trigger words match the
        // request go in FULL into the prompt. Left to their own devices,
        // models rarely call load_skill for requests they think they can
        // handle (and some CLI approval layers reject that call).
        // 3, not 2: generic skills (graphics, data-import) match most data
        // requests ("trace", "csv") and used to push the domain skill out
        for (String name : SkillRegistry.matchSkills(userPrompt, 3)) {
            String body = SkillRegistry.loadSkill(name);
            if (body != null && !body.isEmpty()) {
                sb.append("\n\n## Skill: ").append(name).append(" (auto-loaded)\n")
                  .append(body);
                injectedOut.add(name);
            }
        }
        if (!injectedOut.isEmpty()) {
            sb.append("\n\nThe skill sections above were auto-loaded because they match ")
              .append("this request: follow them, and do not call load_skill for them again.");
            SessionLogger.skillsInjected(injectedOut);
        }

        try {
            // The editor tools act on the file active NOW, even if the user
            // clicks another tab while the request runs
            McpToolHandler.pinActiveEditor();
            org.scilab.modules.scinotes.SciNotes editor =
                org.scilab.modules.scinotes.SciNotes.getActiveEditor();
            if (editor != null) {
                org.scilab.modules.scinotes.ScilabEditorPane pane = editor.getTextPane();
                if (pane != null) {
                    String filename = pane.getName();
                    String content = pane.getText();

                    sb.append("\n\n## Current editor context\n");

                    sb.append(describeActiveFile(filename));

                    if (content != null && !content.isEmpty()) {
                        // Limit to ~4000 chars to avoid bloating the prompt
                        if (content.length() > 4000) {
                            sb.append("Content (truncated to first 4000 chars):\n```scilab\n");
                            sb.append(content, 0, 4000);
                            sb.append("\n...\n```\n");
                        } else {
                            sb.append("Content:\n```scilab\n");
                            sb.append(content);
                            sb.append("\n```\n");
                        }
                    } else {
                        sb.append("Content: (empty file)\n");
                    }

                    String selection = pane.getSelectedText();
                    if (selection != null && !selection.isEmpty()) {
                        int startLine = pane.getDocument().getDefaultRootElement()
                            .getElementIndex(pane.getSelectionStart()) + 1;
                        int endLine = pane.getDocument().getDefaultRootElement()
                            .getElementIndex(pane.getSelectionEnd()) + 1;
                        int lineCount = endLine - startLine + 1;
                        sb.append("\nSelected text (lines ").append(startLine)
                          .append("-").append(endLine)
                          .append(", ").append(lineCount).append(" lines):\n```scilab\n");
                        if (selection.length() > 2000) {
                            sb.append(selection, 0, 2000);
                            sb.append("\n...\n```\n");
                        } else {
                            sb.append(selection);
                            sb.append("\n```\n");
                        }
                    }
                }
            }
        } catch (Exception e) {
            // If we can't read the editor, just use the base prompt
        }

        return sb.toString();
    }

    /**
     * Refresh the open editor tabs of all the SciNotes windows by reloading
     * files that may have changed on disk.
     * Called when an assistant task completes (the LLM may have used Write/Edit tools).
     */
    private static void refreshOpenEditors() {
        // Swing: on the EDT. A tab with changes the user did not save is
        // never reloaded (they would be lost): SciNotes' own check of
        // externally modified files deals with it.
        javax.swing.SwingUtilities.invokeLater(() -> {
            try {
                for (org.scilab.modules.scinotes.SciNotes editor
                        : new java.util.ArrayList<>(org.scilab.modules.scinotes.SciNotes.getSciNotesList())) {
                    int count = editor.getTabPane().getTabCount();
                    for (int i = 0; i < count; i++) {
                        org.scilab.modules.scinotes.ScilabEditorPane pane = editor.getTextPane(i);
                        if (pane == null || pane.getName() == null) {
                            continue;
                        }
                        boolean unsaved = pane.getDocument() instanceof org.scilab.modules.scinotes.ScilabDocument
                            && ((org.scilab.modules.scinotes.ScilabDocument) pane.getDocument()).isContentModified();
                        java.io.File f = new java.io.File(pane.getName());
                        if (!unsaved && f.exists() && f.lastModified() > pane.getLastModified()) {
                            editor.reload(i);
                        }
                    }
                }
            } catch (Exception e) {
                // Ignore refresh errors
            }
        });
    }

    /**
     * Load the shared Scilab knowledge file from SCI/modules/scinotes/etc/assistant/scilab-knowledge.md
     * @return the file content, or null if not found
     */
    private static String loadKnowledgeFile() {
        try {
            java.io.File dir = SkillRegistry.getAssistantEtcDir();
            if (dir == null) return null;

            java.io.File file = new java.io.File(dir, "scilab-knowledge.md");
            if (!file.exists()) return null;

            return new String(java.nio.file.Files.readAllBytes(file.toPath()),
                              java.nio.charset.StandardCharsets.UTF_8);
        } catch (Exception e) {
            LOG.warning("Failed to load scilab-knowledge.md: " + e.getMessage());
            return null;
        }
    }

    /**
     * Get Scilab's current working directory via JNI.
     */
    private static String getScilabPwd() {
        try {
            return org.scilab.modules.commons.CommonFileUtils.getCWD();
        } catch (Exception e) {
            return System.getProperty("user.dir", ".");
        }
    }

    private static String escapeJson(String s) {
        if (s == null) return "";
        StringBuilder sb = new StringBuilder(s.length() + 16);
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '\\': sb.append("\\\\"); break;
                case '"':  sb.append("\\\""); break;
                case '\n': sb.append("\\n"); break;
                case '\r': sb.append("\\r"); break;
                case '\t': sb.append("\\t"); break;
                default:
                    // Other control chars are invalid in JSON strings and
                    // would silently break JSON.parse on the JS side
                    if (c < 0x20) {
                        sb.append(String.format("\\u%04x", (int) c));
                    } else {
                        sb.append(c);
                    }
                    break;
            }
        }
        return sb.toString();
    }
}
