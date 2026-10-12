/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*
 * This file applies the user-configured SciNotes shortcuts inside Monaco. It
 * captures keydown events before Monaco handles them, disables conflicting
 * native Monaco shortcuts, runs explicitly configured Monaco-only commands,
 * and forwards mapped SciNotes actions to Java.
 */

function configureShortcuts(config) {
    var state = window.SciNotesMonacoState;
    if (!state.editor) {
        return;
    }

    if (state.shortcutKeydownListener && state.shortcutKeydownTarget) {
        state.shortcutKeydownTarget.removeEventListener("keydown", state.shortcutKeydownListener, true);
        state.shortcutKeydownListener = null;
        state.shortcutKeydownTarget = null;
    }

    var shortcuts = config && config.shortcuts ? config.shortcuts : [];
    state.shortcutKeydownListener = function(event) {
        // The fields of the breakpoint panel take their keys themselves.
        if (typeof isDapBreakpointEditorTarget === "function"
                && isDapBreakpointEditorTarget(event.target)) {
            return;
        }
        // Resolved per key: in a split tab, the view being typed in.
        var editor = getViewEditor();
        // SciNotes maps Tab to indentation. Give a visible Monaco completion
        // priority, matching Enter's validation behavior, and keep the normal
        // indentation shortcut when no completion list is open.
        if (shouldAcceptCompletionWithTab(event)) {
            consumeShortcutEvent(event);
            editor.trigger("scinotes", "acceptSelectedSuggestion", null);
            return;
        }

        // Leave Tab to Monaco while a snippet is being filled: its own
        // bindings walk the tabstops, and Escape ends the session.
        if (isSnippetTabstopKey(editor, event)) {
            return;
        }

        var shortcut = findShortcut(shortcuts, event);
        if (!shortcut) {
            // Shortcuts unknown to SciNotes belong to Monaco. Conflicting
            // Monaco defaults are blocked explicitly by disabled entries in
            // the generated shortcut configuration.
            return;
        }

        if (shortcut.nativePassthrough) {
            return;
        }

        // Let Monaco reuse Ctrl+Space to expand the documentation of the
        // currently selected completion item when its suggest widget is open.
        if (shortcut.monacoCommand === "editor.action.triggerSuggest"
                && document.querySelector(".suggest-widget.visible")) {
            return;
        }

        consumeShortcutEvent(event);

        if (shortcut.disabled) {
            return;
        }

        if (shortcut.monacoCommand) {
            var command = shortcut.monacoCommand;
            if (command === "editor.action.triggerSuggest") {
                var position = editor.getPosition();
                var model = editor.getModel();
                var previousCharacter = position && model && position.column > 1
                    ? model.getLineContent(position.lineNumber).charAt(position.column - 2) : "";
                if (previousCharacter === "(" || previousCharacter === ",") {
                    command = "editor.action.triggerParameterHints";
                }
            }
            editor.trigger("scinotes", command, null);
            if (typeof notifyUndoStateSoon === "function") {
                notifyUndoStateSoon();
            }
        } else if (shortcut.bridge) {
            notifyJava("shortcut:", shortcut.bridge);
        }
    };
    state.shortcutKeydownTarget = document;
    state.shortcutKeydownTarget.addEventListener("keydown", state.shortcutKeydownListener, true);
}

function isSnippetTabstopKey(editor, event) {
    if ((event.key !== "Tab" && event.code !== "Tab")
            || event.ctrlKey || event.metaKey || event.altKey) {
        return false;
    }

    var snippets = editor.getContribution ? editor.getContribution("snippetController2") : null;
    return !!(snippets && snippets.isInSnippet && snippets.isInSnippet());
}

function shouldAcceptCompletionWithTab(event) {
    return !event.isComposing
        && (event.key === "Tab" || event.code === "Tab")
        && !event.ctrlKey
        && !event.metaKey
        && !event.shiftKey
        && !event.altKey
        && !!document.querySelector(".suggest-widget.visible");
}

function consumeShortcutEvent(event) {
    event.preventDefault();
    event.stopPropagation();
    if (typeof event.stopImmediatePropagation === "function") {
        event.stopImmediatePropagation();
    }
}

function findShortcut(shortcuts, event) {
    for (var i = 0; i < shortcuts.length; i++) {
        if (!shortcuts[i].disabled && matchesShortcut(shortcuts[i], event)) {
            return shortcuts[i];
        }
    }

    for (var j = 0; j < shortcuts.length; j++) {
        if (shortcuts[j].disabled && matchesShortcut(shortcuts[j], event)) {
            return shortcuts[j];
        }
    }

    return null;
}

function matchesShortcut(shortcut, event) {
    if (event.isComposing || !matchesShortcutKey(shortcut, event)) {
        return false;
    }

    return shortcut.ctrlCmd === (event.ctrlKey || event.metaKey)
        && shortcut.shift === event.shiftKey
        && shortcut.alt === event.altKey;
}

function matchesShortcutKey(shortcut, event) {
    if (shortcut.key) {
        var expected = shortcut.key;
        var actual = event.key || "";
        if (expected.length === 1 && actual.length === 1) {
            expected = expected.toLowerCase();
            actual = actual.toLowerCase();
        }

        return expected === actual
            || (expected === "<" && actual === ">");
    }

    return shortcut.keyCode === event.code;
}
