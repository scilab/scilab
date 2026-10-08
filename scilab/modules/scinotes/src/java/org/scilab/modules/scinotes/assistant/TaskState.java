package org.scilab.modules.scinotes.assistant;

import java.util.ArrayList;
import java.util.List;

/**
 * Mutable state of one running assistant request, filled by the
 * {@link AcpClient} and polled through {@link AgentProcess}.
 */
final class TaskState {

    /** Event types for activity display. */
    static final String EVT_THINKING = "thinking";
    static final String EVT_TOOL_USE = "tool_use";
    static final String EVT_TOOL_RESULT = "tool_result";
    static final String EVT_TEXT = "text";
    /** Progress of the request itself (agent starting, session resumed...). */
    static final String EVT_STATUS = "status";

    volatile boolean complete = false;
    volatile String result = "";
    volatile double cost = 0.0;
    volatile long tokensIn = 0;
    volatile long tokensOut = 0;
    /** Streamed chars (text + thinking) of the request: the live token
     *  counter is estimated from them (~4 chars/token). */
    volatile long liveEstChars = 0;
    volatile boolean isError = false;
    volatile String errorMessage = "";
    volatile String sessionId = "";
    /** Stops the running request (the Stop button). */
    volatile Runnable cancelHook = null;
    /** Receives the user's answer to a question of the request (permission,
     *  sign-in): (question id, answer). */
    volatile java.util.function.BiConsumer<String, String> answerHook = null;
    final StringBuilder texts = new StringBuilder();
    /** Pending activity events for the GUI (thread-safe). */
    final List<String> pendingEvents = new ArrayList<>();

    /** Queue an activity event for the GUI. */
    void emitEvent(String type, String message) {
        String json = "{\"type\":\"" + type + "\",\"message\":\""
                      + JsonUtil.escape(message) + "\"}";
        synchronized (pendingEvents) {
            pendingEvents.add(json);
        }
    }

    /** Queue an event built by the caller (a JSON object with a "type"). */
    void emitJson(String json) {
        synchronized (pendingEvents) {
            pendingEvents.add(json);
        }
    }

    /** Append streamed text and emit the live text event. */
    void appendStreamedText(String text) {
        texts.append(text);
        liveEstChars += text.length();
        emitEvent(EVT_TEXT, text);
    }

    /** Separate the text of a new model message from the previous one
     *  (streamed deltas of consecutive messages would otherwise be glued). */
    void startNewMessage() {
        int n = texts.length();
        if (n > 0 && texts.charAt(n - 1) != '\n') {
            texts.append("\n\n");
        }
    }
}
