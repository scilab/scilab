package org.scilab.modules.scinotes.assistant;

/**
 * Standalone end-to-end smoke test of one ACP agent through the real
 * AcpClient (the code SciNotes runs), without Scilab. The MCP side is
 * played by fake_mcp_server.py (see README.md).
 *
 * Two turns in the same session check that the conversation stays alive;
 * --cancel-after stops the first turn like the Stop button; --answer
 * gives the answer to the permission questions (an option kind:
 * allow_once, allow_always, reject_once...).
 *
 * Usage: AcpSmokeTest &lt;agent&gt; &lt;mcpUrl|-&gt; [--model M] [--effort E]
 *        [--cancel-after S] [--second "prompt"] &lt;prompt...&gt;
 */
public final class AcpSmokeTest {

    /** Option kind chosen for the permission questions (--answer). */
    private static String answerKind = "allow_once";

    public static void main(String[] args) throws Exception {
        if (args.length < 3) {
            System.out.println("usage: AcpSmokeTest <agent> <mcpUrl|-> [--probe] [--model M] [--effort E]"
                               + " [--cancel-after S] [--answer KIND] [--resume ID] [--history TEXT] [--second \"prompt\"] [prompt...]");
            System.exit(2);
        }
        AcpAgent agent = AcpAgent.find(args[0]);
        if (agent == null) {
            System.out.println("unknown agent: " + args[0] + " (is -Dscilab.assistant.etc set?)");
            System.exit(2);
        }
        String mcpUrl = "-".equals(args[1]) ? "" : args[1];
        String model = "";
        String effort = "";
        String second = null;
        double cancelAfter = -1;
        boolean probe = false;
        String resume = "";
        String history = null;
        StringBuilder p = new StringBuilder();
        for (int i = 2; i < args.length; i++) {
            switch (args[i]) {
                case "--probe": probe = true; break;
                case "--answer": answerKind = args[++i]; break;
                case "--resume": resume = args[++i]; break;
                case "--history": history = args[++i].replace("\\n", "\n"); break;
                case "--model": model = args[++i]; break;
                case "--effort": effort = args[++i]; break;
                case "--second": second = args[++i]; break;
                case "--cancel-after": cancelAfter = Double.parseDouble(args[++i]); break;
                default: p.append(p.length() > 0 ? " " : "").append(args[i]);
            }
        }
        System.out.println("[command] " + agent.commandLine());
        // Full JSON-RPC trace; also keeps AgentProcess away from ScilabConstants
        AgentProcess.setLogFile(new java.io.File("acp_smoke_" + agent.id + ".log").getAbsolutePath());
        new java.io.File("acp_smoke_" + agent.id + ".log").delete();

        AskContext ctx = new AskContext();
        ctx.prompt = p.toString();
        ctx.model = model;
        ctx.effort = effort;
        ctx.systemPrompt = "You are a Scilab coding assistant integrated into the SciNotes editor. "
            + "Always write code in the Scilab language. Use the scilab MCP tools "
            + "(execute_scilab_code, get_figure, editor tools) to act rather than only suggest.";
        ctx.maxTurns = 10;
        ctx.mcpHttpUrl = mcpUrl;
        ctx.mcpSseUrl = mcpUrl.isEmpty() ? "" : mcpUrl.replace("/mcp", "/sse");
        ctx.workingDir = System.getProperty("user.dir");
        ctx.resumeSessionId = resume;
        ctx.history = history;

        AcpClient client = AcpClient.forAgent(agent);
        if (probe) {
            // What the settings page shows; the session it opens must be
            // taken by the first request
            long t0 = System.currentTimeMillis();
            System.out.println("[probe] " + client.probe(ctx));
            System.out.println("---- probe: " + (System.currentTimeMillis() - t0) / 1000.0 + " s");
        }
        if (p.length() == 0) {
            long t0 = System.currentTimeMillis();
            AcpClient.shutdownAll();
            System.out.println("---- shutdown: " + (System.currentTimeMillis() - t0) + " ms");
            System.exit(0);
        }
        TaskState t1 = turn(client, ctx, cancelAfter);
        if (second != null && !t1.sessionId.isEmpty()) {
            ctx.prompt = second;
            ctx.resumeSessionId = t1.sessionId;
            TaskState t2 = turn(client, ctx, -1);
            System.out.println("---- same session: " + t1.sessionId.equals(t2.sessionId));
        }
        AcpClient.shutdownAll();
        System.exit(0);
    }

    private static TaskState turn(AcpClient client, AskContext ctx, double cancelAfter) throws Exception {
        System.out.println("==== prompt: " + ctx.prompt);
        TaskState task = new TaskState();
        long t0 = System.currentTimeMillis();
        Thread runner = new Thread(() -> client.runTurn(1, task, ctx));
        runner.start();
        boolean cancelled = false;
        while (runner.isAlive()) {
            runner.join(200);
            String events = drain(task);
            if (!events.isEmpty()) {
                System.out.println("[events] " + events);
            }
            if (!cancelled && cancelAfter >= 0 && System.currentTimeMillis() - t0 > cancelAfter * 1000) {
                System.out.println("[cancel]");
                Runnable hook = task.cancelHook;
                if (hook != null) {
                    hook.run();
                }
                cancelled = true;
            }
        }
        String events = drain(task);
        if (!events.isEmpty()) {
            System.out.println("[events] " + events);
        }
        System.out.println("---- " + (System.currentTimeMillis() - t0) / 1000.0 + " s, session_id: " + task.sessionId);
        System.out.println("---- tokens in/out: " + task.tokensIn + "/" + task.tokensOut + " cost: " + task.cost);
        System.out.println("---- isError: " + task.isError + " msg: " + task.errorMessage);
        System.out.println("---- result:\n" + task.result);
        return task;
    }

    /** Text deltas are folded into one marker to keep the log readable.
     *  Questions are answered: permissions with --answer, sign-in with
     *  "already signed in". */
    private static String drain(TaskState task) {
        StringBuilder sb = new StringBuilder();
        java.util.List<String> questions = new java.util.ArrayList<>();
        synchronized (task.pendingEvents) {
            boolean text = false;
            for (String e : task.pendingEvents) {
                if (e.startsWith("{\"type\":\"permission\"") || e.startsWith("{\"type\":\"auth\"")) {
                    questions.add(e);
                }
                if (e.startsWith("{\"type\":\"text\"")) {
                    if (!text) {
                        sb.append("txt... ");
                    }
                    text = true;
                } else {
                    sb.append(e).append(' ');
                    text = false;
                }
            }
            task.pendingEvents.clear();
        }
        for (String q : questions) {
            com.google.gson.JsonObject o = com.google.gson.JsonParser.parseString(q).getAsJsonObject();
            String value = "__signed_in";
            if ("permission".equals(o.get("type").getAsString())) {
                value = null;
                for (com.google.gson.JsonElement opt : o.getAsJsonArray("options")) {
                    if (answerKind.equals(opt.getAsJsonObject().get("kind").getAsString())) {
                        value = opt.getAsJsonObject().get("id").getAsString();
                    }
                }
            }
            System.out.println("[answer] " + o.get("id").getAsString() + " -> " + value);
            java.util.function.BiConsumer<String, String> hook = task.answerHook;
            if (hook != null && value != null) {
                hook.accept(o.get("id").getAsString(), value);
            }
        }
        return sb.toString().trim();
    }
}
