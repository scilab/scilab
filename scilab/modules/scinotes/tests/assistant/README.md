# Assistant agent smoke tests (dev only)

The SciNotes assistant talks to AI agents through the Agent Client Protocol
(ACP, https://agentclientprotocol.com): one generic client (AcpClient)
drives every agent of `etc/assistant/agents.json`, with no agent-specific
code. The agents are programs the user installs; Scilab installs nothing.
They update themselves on the user's machine: the tools in this directory
recheck an agent in minutes, without launching Scilab.

They are development tools: compiled by hand against the scinotes jar,
never shipped in it, and they run REAL requests on the developer's own
accounts.

## Prerequisites

- the scinotes jar built (`cd modules/prebuildjava && ant scinotes`, with
  `JAVA_HOME` set and `SCI/java/ant/bin` on the PATH, as ant-all.vcxproj does);
- a JDK, python 3, and the agent(s) to test installed and logged in.

## 1. Fake MCP server

A minimal streamable-HTTP MCP server that mimics the scilab tools
(canned answers, logs every request):

    python fake_mcp_server.py 8765 fake_mcp.log

## 2. Probe and requests through one agent

    javac -cp "../../jar/org.scilab.modules.scinotes.jar;../../../commons/jar/org.scilab.modules.commons.jar;<SCI>/thirdparty/gson-2.10.1.jar" -d classes AcpSmokeTest.java
    java -Dscilab.assistant.etc=<SCI>/modules/scinotes/etc/assistant -Dscilab.assistant.home=<tmpdir> \
         -cp "classes;../../jar/org.scilab.modules.scinotes.jar;../../../commons/jar/org.scilab.modules.commons.jar;<SCI>/thirdparty/gson-2.10.1.jar" \
         org.scilab.modules.scinotes.assistant.AcpSmokeTest claude http://127.0.0.1:8765/mcp \
         --probe --second "What did I ask before?" "compute 6*7 with the scilab tool"

(`;` is the Windows classpath separator — use `:` elsewhere.)

Arguments: `<agent> <mcpUrl|-> [options] [prompt...]`:
- `--probe`: what the settings page shows (version, models, efforts); the
  session it opens must be taken by the first request (one `session/new`
  in the trace);
- `--model M`, `--effort E`: applied only when the agent offers the value
  in its session config options;
- `--cancel-after S`: the Stop button, S seconds into the first request;
- `--second "prompt"`: a second request in the same session.

What to check in the output:
- `[events]` show the tool calls with their labels ("Executing Scilab
  code: ...") and the MCP calls reach fake_mcp.log;
- `---- result:` holds the final answer, `session_id` is non-empty;
- the full JSON-RPC trace goes to `acp_smoke_<agent>.log`: diff it against
  an old one when an agent update breaks something.

An agent that accepts no HTTP MCP server gets the scilab tools over stdio
through `McpStdioBridge` (a JVM started by the agent): fake_mcp.log then
shows its `initialize` and `tools/list`.

## ACP conformance of each agent

What the protocol gives us — not workarounds: an agent that falls short is
documented here and reported upstream.

| Agent       | Program               | Tested with        | MCP              | Permission requests | Models     | Efforts | Usage / cost |
|-------------|-----------------------|--------------------|------------------|---------------------|------------|---------|--------------|
| claude      | claude-agent-acp      | 0.85.1             | http             | yes                 | yes        | yes     | yes / yes    |
| cursor      | cursor-agent acp      | 2026.08.25-3e8eec8 | http             | yes                 | yes (effort in the model id) | - | no |
| copilot     | copilot --acp         | 1.0.88             | http             | yes                 | no (ignored) | -     | tokens only  |
| codex       | codex-acp             | 2.1.1 (probe only) | http             | no (server review)  | yes (effort in the model id) | - | tokens only |
| antigravity | agy_acp_server        | 1.2.1 (probe only) | http             | yes                 | yes        | -       | no           |
| vibe        | vibe-acp              | 2.25.8             | stdio (bridge)   | -                   | yes        | non-standard category "thinking" | - |

"probe only": checked with a protocol probe, not yet through AcpClient.
Vibe: probe and MCP bridge checked; requests blocked by a Mistral rate
limit on the developer's account. Antigravity needs an `authenticate` call
the first time (sign-in UI to come).

Update this table whenever an agent is rechecked.
