package org.scilab.modules.scinotes.assistant;

import java.util.List;

/**
 * Everything one assistant request needs.
 */
final class AskContext {
    /** The user prompt (already includes file references / attachments). */
    String prompt;
    /** Model chosen in the UI, "" for the agent's default. */
    String model = "";
    /** Effort level chosen in the UI, "" for the agent's default. */
    String effort = "";
    /** MCP SSE endpoint URL ("" when the MCP server is not running). */
    String mcpSseUrl = "";
    /** MCP streamable-HTTP endpoint URL ("" when not running). */
    String mcpHttpUrl = "";
    /** Full system prompt (Scilab knowledge + skills + editor context). */
    String systemPrompt;
    /** Max tool calls in one request (safety stop). */
    int maxTurns;
    /** Agent session ID to resume ("" = new conversation). */
    String resumeSessionId = "";
    /** Attached images as data URLs ("data:image/png;base64,...."), or null. */
    List<String> images;
    /** Directory the agent works in. */
    String workingDir;
    /** The conversation shown in the panel, for an agent that lost it
     *  (new session where one was expected), or null. */
    String history;
}
