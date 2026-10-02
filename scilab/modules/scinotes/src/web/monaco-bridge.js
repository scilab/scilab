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
 * This file is the browser-side API used by the java class MonacoEditorPane. Java calls these
 * functions to update Monaco, and Monaco uses notifyJava() to report edits,
 * cursor moves, undo state, shortcuts, and editor actions back to SciNotes.
 */

window.SciNotesMonacoSetValue = function(value, revision, clean) {
    var state = window.SciNotesMonacoState;
    state.pendingValue = value || "";
    state.pendingClean = clean !== false;
    state.documentRevision = Number.isFinite(revision) ? revision : state.documentRevision;
    if (state.editor) {
        if (state.editor.getValue() !== state.pendingValue) {
            state.applyingJavaValue = true;
            state.editor.setValue(state.pendingValue);
            if (typeof useLfEndOfLine === "function") {
                useLfEndOfLine(state.editor.getModel());
            }
            state.applyingJavaValue = false;
        }
        resetUndoRedoState();
        updateSavedAlternativeVersion();
        /*
         * This sets the page's first buffer, before SciNotes adopts it as a
         * document of its own. Any later caller would be writing into
         * whichever document happens to be active, whatever it holds.
         */
        if (typeof getActiveDocumentEntry === "function") {
            var activeDocument = getActiveDocumentEntry();
            if (activeDocument && activeDocument.id === "initial") {
                activeDocument.revision = state.documentRevision;
                activeDocument.clean = state.pendingClean;
                activeDocument.savedAlternativeVersionId = state.savedAlternativeVersionId;
            }
        }
        notifyUndoStateSoon();
    }
};

window.SciNotesMonacoMarkSaved = function(revision) {
    var state = window.SciNotesMonacoState;
    if (Number.isFinite(revision) && revision !== state.documentRevision) {
        return;
    }
    state.pendingClean = true;
    updateSavedAlternativeVersion();
    if (typeof getActiveDocumentEntry === "function") {
        var activeDocument = getActiveDocumentEntry();
        if (activeDocument) {
            activeDocument.clean = true;
            activeDocument.savedAlternativeVersionId = state.savedAlternativeVersionId;
        }
    }
};

window.SciNotesMonacoSetReadOnly = function(readOnly) {
    var state = window.SciNotesMonacoState;
    state.pendingReadOnly = !!readOnly;
    if (state.editor) {
        state.editor.updateOptions({ readOnly: state.pendingReadOnly });
    }
};

window.SciNotesMonacoApplyOptions = function(options) {
    var state = window.SciNotesMonacoState;
    state.pendingOptions = options || {};
    applyEditorOptions(state.pendingOptions);
};

window.SciNotesMonacoConfigureShortcuts = function(config) {
    var state = window.SciNotesMonacoState;
    state.pendingShortcutConfig = config || { shortcuts: [] };
    if (state.editor) {
        configureShortcuts(state.pendingShortcutConfig);
    }
};

window.SciNotesMonacoConfigureLsp = function(config) {
    var state = window.SciNotesMonacoState;
    state.pendingLspConfig = config || null;
    if (state.editor && typeof configureLsp === "function") {
        configureLsp(state.pendingLspConfig);
    }
};

window.SciNotesMonacoSetAnchors = function(anchors) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor) {
        return;
    }

    anchors = anchors || [];
    state.lastAnchors = anchors;
    setViewDecorations("anchorDecorations", anchors.map(function(anchor) {
        var line = Math.max(1, anchor.line || 1);
        var label = anchor.name || "";
        return {
            range: new monaco.Range(line, 1, line, 1),
            options: {
                isWholeLine: true,
                className: "scinotes-anchor-line",
                lineNumberClassName: "scinotes-anchor-line-number",
                hoverMessage: { value: label ? "Anchor: " + label : "Anchor" },
                overviewRuler: {
                    color: "#d6a800",
                    position: monaco.editor.OverviewRulerLane.Left
                }
            }
        };
    }));
};

window.SciNotesMonacoFocus = function() {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }
    // Java asks for the focus on every tab or window activation: an element
    // of the editor already holding it, such as the find widget, keeps it.
    var domNode = editor.getDomNode();
    if (domNode && domNode.contains(document.activeElement)) {
        return;
    }
    editor.focus();
};

window.SciNotesMonacoBlur = function() {
    var state = window.SciNotesMonacoState;
    var activeElement = document.activeElement;
    [state.editor, state.secondaryEditor].forEach(function(editor) {
        var domNode = editor && editor.getDomNode();
        if (domNode && activeElement && domNode.contains(activeElement)
                && typeof activeElement.blur === "function") {
            activeElement.blur();
        }
    });
};

window.SciNotesMonacoLayout = function() {
    var state = window.SciNotesMonacoState;
    if (state.editor) {
        state.editor.layout();
    }
    if (state.secondaryEditor) {
        state.secondaryEditor.layout();
    }
};

window.SciNotesMonacoRevealLine = function(line) {
    var editor = getViewEditor();
    if (editor) {
        editor.revealLineInCenterIfOutsideViewport(line);
        editor.setPosition({ lineNumber: line, column: 1 });
    }
};

window.SciNotesMonacoApplyEdit = function(expectedRevision, newRevision, offset, length, text) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor) {
        return;
    }

    var model = editor.getModel();
    if (!model) {
        return;
    }

    if (expectedRevision !== state.documentRevision) {
        notifyJava("syncMismatch:", state.documentRevision + ":" + expectedRevision);
        return;
    }

    var start = model.getPositionAt(offset);
    var end = model.getPositionAt(offset + length);
    var range = new monaco.Range(start.lineNumber, start.column, end.lineNumber, end.column);
    state.applyingJavaValue = true;
    try {
        editor.executeEdits("java", [{ range: range, text: text || "", forceMoveMarkers: true }]);
    } finally {
        state.applyingJavaValue = false;
    }
    state.documentRevision = newRevision;
    notifyUndoStateSoon();
};

window.SciNotesMonacoSetPositionAtOffset = function(offset) {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }

    var model = editor.getModel();
    if (!model) {
        return;
    }

    var position = model.getPositionAt(offset || 0);
    editor.setPosition(position);
    editor.revealPositionInCenterIfOutsideViewport(position);
    editor.focus();
    notifySelection();
};

window.SciNotesMonacoSetSelectionAtOffsets = function(selectionStart, selectionEnd) {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }

    var model = editor.getModel();
    if (!model) {
        return;
    }

    var length = model.getValueLength();
    var start = Math.max(0, Math.min(selectionStart || 0, length));
    var end = Math.max(0, Math.min(selectionEnd || 0, length));
    var startPosition = model.getPositionAt(start);
    var endPosition = model.getPositionAt(end);
    editor.setSelection(new monaco.Selection(startPosition.lineNumber, startPosition.column, endPosition.lineNumber, endPosition.column));
    editor.revealPositionInCenterIfOutsideViewport(endPosition);
    editor.focus();
    notifySelection();
};

window.SciNotesMonacoReplaceSelection = function(text) {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }

    var model = editor.getModel();
    var selections = editor.getSelections();
    if (!model || !selections || selections.length === 0) {
        return;
    }

    editor.focus();
    editor.pushUndoStop();
    editor.executeEdits("scinotes-paste", selections.map(function(selection) {
        return {
            range: selection,
            text: text || "",
            forceMoveMarkers: true
        };
    }));
    editor.pushUndoStop();
    notifyUndoStateSoon();
};

window.SciNotesMonacoCopyAsHtmlWithLineNumbers = function() {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    var selection = editor && editor.getSelection();
    if (!editor || !model || !selection || selection.isEmpty()) {
        return;
    }

    notifyJava("copyHtmlWithLineNumbers:", model.getValueInRange(selection));
};

window.SciNotesMonacoPasteColumn = function(text) {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    if (!editor || !model) {
        return;
    }

    var selection = editor.getSelection();
    if (!selection) {
        return;
    }

    var lines = (text || "").split(/\r\n|\r|\n/);
    var position = selection.getStartPosition();

    editor.focus();
    editor.pushUndoStop();
    if (!selection.isEmpty()) {
        editor.executeEdits("scinotes-paste-column", [{ range: selection, text: "", forceMoveMarkers: true }]);
        position = selection.getStartPosition();
    }

    ensureLineCount(model, position.lineNumber + lines.length - 1);

    var edits = [];
    for (var i = 0; i < lines.length; i++) {
        var lineNumber = position.lineNumber + i;
        var maxColumn = model.getLineMaxColumn(lineNumber);
        var column = Math.min(position.column, maxColumn);
        var padding = position.column > maxColumn ? repeatChar(" ", position.column - maxColumn) : "";
        edits.push({
            range: new monaco.Range(lineNumber, column, lineNumber, column),
            text: padding + lines[i],
            forceMoveMarkers: true
        });
    }

    editor.executeEdits("scinotes-paste-column", edits);
    editor.setPosition(position);
    editor.pushUndoStop();
    notifyUndoStateSoon();
};

window.SciNotesMonacoRepeatSelectionOnColumn = function() {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    if (!editor || !model) {
        return;
    }

    var selection = editor.getSelection();
    var text;
    var sourcePosition;
    var shouldSelect = false;
    if (selection && !selection.isEmpty()) {
        text = model.getValueInRange(selection);
        sourcePosition = selection.getStartPosition();
        shouldSelect = true;
    } else {
        sourcePosition = editor.getPosition();
        if (!sourcePosition) {
            return;
        }
        var nextPosition = model.modifyPosition(sourcePosition, 1);
        if (!nextPosition || (nextPosition.lineNumber === sourcePosition.lineNumber && nextPosition.column === sourcePosition.column)) {
            return;
        }
        text = model.getValueInRange(new monaco.Range(sourcePosition.lineNumber, sourcePosition.column, nextPosition.lineNumber, nextPosition.column));
    }

    if (!text) {
        return;
    }

    var targetLine = sourcePosition.lineNumber + 1;
    ensureLineCount(model, targetLine);

    var maxColumn = model.getLineMaxColumn(targetLine);
    var column = Math.min(sourcePosition.column, maxColumn);
    var padding = sourcePosition.column > maxColumn ? repeatChar(" ", sourcePosition.column - maxColumn) : "";
    var insertedText = padding + text;
    var range = new monaco.Range(targetLine, column, targetLine, column);

    editor.focus();
    editor.pushUndoStop();
    editor.executeEdits("scinotes-repeat-column", [{ range: range, text: insertedText, forceMoveMarkers: true }]);
    if (shouldSelect) {
        var start = new monaco.Position(targetLine, sourcePosition.column);
        var end = model.modifyPosition(start, text.length);
        editor.setSelection(new monaco.Selection(start.lineNumber, start.column, end.lineNumber, end.column));
    } else {
        editor.setPosition(new monaco.Position(targetLine, sourcePosition.column + text.length));
    }
    editor.pushUndoStop();
    notifyUndoStateSoon();
};

window.SciNotesMonacoRemoveTrailingSpaces = function() {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    if (!editor || !model) {
        return;
    }

    var edits = [];
    for (var lineNumber = 1; lineNumber <= model.getLineCount(); lineNumber++) {
        var line = model.getLineContent(lineNumber);
        var trimmedLength = line.replace(/[ \t]+$/, "").length;
        if (trimmedLength !== line.length) {
            edits.push({
                range: new monaco.Range(lineNumber, trimmedLength + 1, lineNumber, line.length + 1),
                text: "",
                forceMoveMarkers: true
            });
        }
    }

    executeMonacoEdits("scinotes-remove-trailing-spaces", edits, editor.getSelections());
};

window.SciNotesMonacoCommentSelection = function() {
    transformSelectedLines("scinotes-comment-selection", function(line) {
        return "//" + line;
    });
};

window.SciNotesMonacoUncommentSelection = function() {
    transformSelectedLines("scinotes-uncomment-selection", function(line) {
        return line.replace(/^(\s*)\/\//, "$1");
    });
};

window.SciNotesMonacoTransformSelection = function(kind) {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    var selections = editor && editor.getSelections();
    if (!editor || !model || !selections || selections.length === 0) {
        return;
    }

    var edits = [];
    var outputSelections = [];
    for (var i = 0; i < selections.length; i++) {
        var selection = selections[i];
        var range = selection;
        var text = model.getValueInRange(selection);
        if (selection.isEmpty()) {
            var position = selection.getPosition();
            var next = model.modifyPosition(position, 1);
            if (!next || (next.lineNumber === position.lineNumber && next.column === position.column)) {
                continue;
            }
            range = new monaco.Range(position.lineNumber, position.column, next.lineNumber, next.column);
            text = model.getValueInRange(range);
        }

        var replacement = transformText(text, kind);
        edits.push({ range: range, text: replacement, forceMoveMarkers: true });
        outputSelections.push(new monaco.Selection(range.startLineNumber, range.startColumn, range.startLineNumber, range.startColumn + replacement.length));
    }

    executeMonacoEdits("scinotes-transform-selection", edits, outputSelections);
};

window.SciNotesMonacoDoubleQuoteStrings = function() {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    if (!editor || !model) {
        return;
    }

    // A transposition quote is indistinguishable from a string one here, so
    // a selection is the way to keep the conversion away from them.
    var selections = editor.getSelections() || [];
    var ranges = [];
    for (var i = 0; i < selections.length; i++) {
        if (!selections[i].isEmpty()) {
            ranges.push(selections[i]);
        }
    }

    if (ranges.length === 0) {
        ranges = [model.getFullModelRange()];
    }

    var edits = [];
    for (var j = 0; j < ranges.length; j++) {
        var text = model.getValueInRange(ranges[j]);
        var replacement = convertSingleQuotedStrings(text);
        if (replacement !== text) {
            edits.push({
                range: ranges[j],
                text: replacement,
                forceMoveMarkers: true
            });
        }
    }

    executeMonacoEdits("scinotes-double-quote-strings", edits, editor.getSelections());
};

window.SciNotesMonacoUndo = function() {
    var editor = getViewEditor();
    if (editor) {
        editor.focus();
        editor.trigger("scinotes", "undo", null);
        notifyUndoStateSoon();
    }
};

window.SciNotesMonacoRedo = function() {
    var editor = getViewEditor();
    if (editor) {
        editor.focus();
        editor.trigger("scinotes", "redo", null);
        notifyUndoStateSoon();
    }
};

window.SciNotesMonacoRunCommand = function(commandId) {
    var editor = getViewEditor();
    if (editor) {
        editor.focus();
        editor.trigger("scinotes", commandId, null);
        notifyUndoStateSoon();
    }
};

/*
 * Correct indentation: the selected lines only, or the whole document when
 * nothing is selected.
 */
window.SciNotesMonacoCorrectIndentation = function() {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }
    var selections = editor.getSelections() || [];
    var hasSelection = selections.some(function(selection) {
        return !selection.isEmpty();
    });
    editor.focus();
    editor.trigger("scinotes",
        hasSelection ? "editor.action.formatSelection" : "editor.action.formatDocument", null);
    notifyUndoStateSoon();
};

window.SciNotesMonacoOpenFind = function() {
    var editor = getViewEditor();
    if (editor) {
        editor.focus();
        editor.trigger("scinotes", "actions.find", null);
    }
};

window.SciNotesMonacoOpenReplace = function() {
    var editor = getViewEditor();
    if (editor) {
        editor.focus();
        editor.trigger("scinotes", "editor.action.startFindReplaceAction", null);
    }
};

window.SciNotesMonacoFindNext = function() {
    findOrMove("editor.action.nextMatchFindAction");
};

window.SciNotesMonacoFindPrevious = function() {
    findOrMove("editor.action.previousMatchFindAction");
};

window.SciNotesMonacoNotifyUndoState = function() {
    notifyUndoState();
};

function applyEditorOptions(options) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor) {
        return;
    }

    options = options || {};
    if (typeof applyScilabTheme === "function" && options.theme) {
        var themeSignature = JSON.stringify(options.theme);
        if (themeSignature !== state.appliedThemeSignature) {
            applyScilabTheme(options.theme);
            state.appliedThemeSignature = themeSignature;
        }
    }

    var editorOptions = {
        fontFamily: options.fontFamily || undefined,
        fontSize: options.fontSize || undefined,
        wordWrap: options.wordWrap || "off",
        tabSize: options.tabSize || 4,
        insertSpaces: !!options.insertSpaces,
        detectIndentation: false,
        renderLineHighlight: options.highlightCurrentLine ? "line" : "none",
        renderWhitespace: options.showWhitespaces ? "all" : "none",
        "semanticHighlighting.enabled": !options.theme || options.theme.semanticHighlighting !== false
    };

    if (typeof options.snippets === "boolean") {
        // "inline" leaves the order to the sortText of the server, which puts
        // the snippets first among the items starting with what is typed.
        editorOptions.snippetSuggestions = options.snippets ? "inline" : "none";
    }

    var localLineNumbers = options.lineNumbers && options.lineNumbers.mode === "local";
    if (!localLineNumbers) {
        state.astLocalLineNumbersActive = false;
        editorOptions.lineNumbers = getLineNumbersOption(options.lineNumbers);
    } else if (!state.astLocalLineNumbersActive) {
        // Display stable absolute numbers until the first AST ranges arrive.
        editorOptions.lineNumbers = "on";
    }

    if (typeof options.autoClosingPairs === "boolean") {
        editorOptions.autoClosingBrackets = options.autoClosingPairs ? "languageDefined" : "never";
        editorOptions.autoClosingQuotes = options.autoClosingPairs ? "languageDefined" : "never";
    }

    editor.updateOptions(editorOptions);
    if (state.secondaryEditor) {
        // Keep the split's second view (see monaco-split.js) in visual sync;
        // it does not go through this function on its own.
        state.secondaryEditor.updateOptions(editorOptions);
    }
    state.localLineNumberFallback = options.lineNumbers;
    if (!localLineNumbers || !state.astLocalLineNumbersActive) {
        applyLocalFunctionDecorations(null);
    }
    var model = editor.getModel();
    if (model) {
        model.updateOptions({
            tabSize: editorOptions.tabSize,
            insertSpaces: editorOptions.insertSpaces
        });
    }
}

function getLineNumbersOption(lineNumbers) {
    if (!lineNumbers || typeof lineNumbers === "string") {
        return lineNumbers || "on";
    }
    if (lineNumbers.mode === "off") {
        return "off";
    }
    if (lineNumbers.mode === "local") {
        var numbers = Array.isArray(lineNumbers.numbers) ? lineNumbers.numbers : [];
        return function(originalLineNumber) {
            var value = numbers[originalLineNumber - 1];
            return value > 0 ? String(value) : "";
        };
    }
    return "on";
}

function applyLocalFunctionDecorations(lineNumbers) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor) {
        return;
    }

    var ranges = getLocalLevelRanges(lineNumbers);
    setViewDecorations("localFunctionDecorations", ranges.map(function(range) {
        var start = Math.max(1, range.start || 1);
        var end = Math.max(start, range.end || start);
        return {
            range: new monaco.Range(start, 1, end, 1),
            options: {
                lineNumberClassName: "scinotes-local-function-line-number-" + range.colorIndex
            }
        };
    }));
}

function getLocalLevelRanges(lineNumbers) {
    if (!lineNumbers || lineNumbers.mode !== "local" || !Array.isArray(lineNumbers.levels)) {
        return [];
    }

    var numbers = Array.isArray(lineNumbers.numbers) ? lineNumbers.numbers : [];
    var levels = lineNumbers.levels;
    var ranges = [];
    var current = null;
    for (var i = 0; i < levels.length; i++) {
        var number = numbers[i];
        var colorIndex = number > 0 ? levels[i] % 3 : 0;
        if (colorIndex !== 1 && colorIndex !== 2) {
            colorIndex = 0;
        }

        if (current && current.colorIndex === colorIndex && colorIndex !== 0) {
            current.end = i + 1;
            continue;
        }

        if (current) {
            ranges.push(current);
            current = null;
        }
        if (colorIndex !== 0) {
            current = {
                start: i + 1,
                end: i + 1,
                colorIndex: colorIndex
            };
        }
    }

    if (current) {
        ranges.push(current);
    }
    return ranges;
}

window.SciNotesMonacoApplyLineNumbers = function(lineNumbers) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor) {
        return;
    }
    state.localLineNumberFallback = lineNumbers;
    if (lineNumbers && lineNumbers.mode === "local") {
        if (state.astLocalLineNumbersActive) {
            return;
        }
        setViewLineNumbers("on");
        applyLocalFunctionDecorations(null);
        return;
    }
    state.astLocalLineNumbersActive = false;
    setViewLineNumbers(getLineNumbersOption(lineNumbers));
    applyLocalFunctionDecorations(lineNumbers);
};

// Replace the synchronous Java fallback once AST function ranges are available.
function applyStoredAstLineNumbers(astLineNumbers) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor || !astLineNumbers) {
        return;
    }
    state.astLocalLineNumbersActive = true;
    setViewLineNumbers(getLineNumbersOption(astLineNumbers));
    applyLocalFunctionDecorations(astLineNumbers);
}

/*
 * The line numbering and the decorations below belong to the document, but
 * Monaco keeps them per editor: a decoration added through one editor only
 * shows in that one. A split tab's second view (monaco-split.js) gets the
 * same, with its own decoration ids.
 */
function setViewLineNumbers(lineNumbers) {
    var state = window.SciNotesMonacoState;
    state.editor.updateOptions({ lineNumbers: lineNumbers });
    if (state.secondaryEditor) {
        state.secondaryEditor.updateOptions({ lineNumbers: lineNumbers });
    }
}

/**
 * @param key name of the primary editor's decoration id list in the state;
 *        the second view's list is the same name prefixed with "secondary"
 * @param decorations the decorations to show in place of the previous ones
 */
function setViewDecorations(key, decorations) {
    var state = window.SciNotesMonacoState;
    state[key] = state.editor.deltaDecorations(state[key], decorations);
    var secondaryKey = "secondary" + key.charAt(0).toUpperCase() + key.slice(1);
    state[secondaryKey] = state.secondaryEditor
        ? state.secondaryEditor.deltaDecorations(state[secondaryKey] || [], decorations) : [];
}

window.SciNotesMonacoApplyStoredAstLineNumbers = applyStoredAstLineNumbers;

window.SciNotesMonacoApplyAstFunctionRanges = function(functionRanges, targetModel) {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    var model = targetModel || editor && editor.getModel();
    var fallback = state.localLineNumberFallback;
    if (!model) {
        return;
    }

    var lineCount = model.getLineCount();
    var ranges = functionRanges.map(function(range) {
        return {
            start: Math.max(0, Math.min(lineCount - 1, range.startLine)),
            end: Math.max(0, Math.min(lineCount - 1, range.endLine))
        };
    }).filter(function(range) {
        return range.end >= range.start;
    }).sort(function(left, right) {
        return left.start - right.start || right.end - left.end;
    });

    var numbers = new Array(lineCount);
    var levels = new Array(lineCount);
    var active = [];
    var nextRange = 0;
    for (var line = 0; line < lineCount; line++) {
        active = active.filter(function(range) { return range.end >= line; });
        while (nextRange < ranges.length && ranges[nextRange].start === line) {
            active.push(ranges[nextRange++]);
        }

        levels[line] = active.length;
        numbers[line] = active.length
            ? line - active[active.length - 1].start + 1
            : line + 1;
    }

    var astLineNumbers = { mode: "local", numbers: numbers, levels: levels };
    // Several tabs can show this model, and they all display the same buffer:
    // the local numbering computed from it belongs to every one of them.
    var entries = typeof getDocumentEntriesForModel === "function"
        ? getDocumentEntriesForModel(model) : [];
    entries.forEach(function(entry) {
        entry.astLineNumbers = astLineNumbers;
    });
    if (editor && editor.getModel() === model && fallback && fallback.mode === "local") {
        applyStoredAstLineNumbers(astLineNumbers);
    }
};

function configureContextMenu() {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    var domNode = editor && editor.getDomNode();
    if (!editor || !domNode) {
        return;
    }

    if (state.contextMenuDocumentListener) {
        document.removeEventListener("contextmenu", state.contextMenuDocumentListener, true);
    }
    if (state.contextMenuMouseDownNode && state.contextMenuMouseDownListener) {
        state.contextMenuMouseDownNode.removeEventListener(
            "mousedown", state.contextMenuMouseDownListener, true);
    }

    // Monaco registers its own contextmenu listener before this integration is
    // configured. With contextmenu disabled, that listener stops the event on
    // the editor node. Listen from document capture so the Java menu request is
    // handled first, while ignoring events outside the editors and in hovers.
    // A split tab shows two editor views (see monaco-split.js): the menu
    // belongs to the one under the pointer.
    state.contextMenuDocumentListener = function(event) {
        var currentEditor = getEditorAtNode(event.target);
        var currentDomNode = currentEditor && currentEditor.getDomNode();
        if (!currentEditor || !currentDomNode
                || isInsideMonacoHover(event.target, currentDomNode)) {
            return;
        }
        state.secondaryViewActive = currentEditor === state.secondaryEditor;

        var model = currentEditor.getModel();
        if (!model) {
            return;
        }

        event.preventDefault();
        event.stopPropagation();

        var target = currentEditor.getTargetAtClientPoint(event.clientX, event.clientY);
        var position = target && target.position ? target.position : currentEditor.getPosition();
        if (!position) {
            return;
        }

        var selection = currentEditor.getSelection();
        if (!selection || selection.isEmpty() || !containsPosition(selection, position)) {
            currentEditor.setPosition(position);
            selection = currentEditor.getSelection();
        }
        notifySelection();

        var offset = model.getOffsetAt(position);
        var selectionStart = selection ? model.getOffsetAt(selection.getStartPosition()) : offset;
        var selectionEnd = selection ? model.getOffsetAt(selection.getEndPosition()) : offset;
        // Java shows the menu over the native browser, which spans both
        // views of a split: page coordinates, not the view's own.
        var x = Math.round(event.clientX);
        var y = Math.round(event.clientY);
        notifyJava("contextMenu:", offset + ":" + selectionStart + ":" + selectionEnd + ":" + x + ":" + y);
    };
    document.addEventListener("contextmenu", state.contextMenuDocumentListener, true);

    state.contextMenuMouseDownListener = function(event) {
        if (event.button !== 2) {
            notifyJava("contextMenuClose:", "");
        }
    };
    state.contextMenuMouseDownNode = document;
    document.addEventListener("mousedown", state.contextMenuMouseDownListener, true);
}

function getEditorAtNode(node) {
    var state = window.SciNotesMonacoState;
    var editors = [state.editor, state.secondaryEditor];
    for (var i = 0; i < editors.length; i++) {
        var domNode = editors[i] && editors[i].getDomNode();
        if (domNode && domNode.contains(node)) {
            return editors[i];
        }
    }
    return null;
}

function isInsideMonacoHover(node, editorDomNode) {
    while (node && node !== editorDomNode) {
        if (node.classList && node.classList.contains("monaco-hover")) {
            return true;
        }
        node = node.parentNode;
    }
    return false;
}

function configureFocusHandling() {
    var editor = window.SciNotesMonacoState.editor;
    var domNode = editor && editor.getDomNode();
    if (!editor || !domNode) {
        return;
    }

    domNode.addEventListener("mousedown", function() {
        window.SciNotesMonacoState.secondaryViewActive = false;
        notifyJava("focus:", "in");
    }, true);
}

function configureOpenUrlHandling() {
    var editor = window.SciNotesMonacoState.editor;
    if (!editor) {
        return;
    }

    /*
     * Left to itself, Monaco opens a link with window.open, which CEF turns
     * into a bare Chromium window. Java opens the browser of the user instead.
     * The scilab-help: scheme keeps its own path, a trusted command URI.
     */
    if (typeof monaco.editor.registerLinkOpener === "function") {
        monaco.editor.registerLinkOpener({
            open: function(resource) {
                var url = typeof resource === "string" ? resource : resource.toString(true);
                if (url.indexOf("command:") === 0) {
                    return false;
                }

                notifyJava("openUrl:", url);
                return true;
            }
        });
    }

    attachOpenUrlHandling(editor);
}

/*
 * Ctrl+click on a URL, for one editor view: the primary one at startup, the
 * split's second view when it is mounted (monaco-split.js).
 */
function attachOpenUrlHandling(editor) {
    var domNode = editor && editor.getDomNode();
    if (!domNode) {
        return;
    }

    domNode.addEventListener("mousedown", function(event) {
        if (!isControlClick(event) || !getUrlTokenAtClientPoint(editor, event.clientX, event.clientY)) {
            return;
        }

        event.preventDefault();
        event.stopPropagation();
        if (typeof event.stopImmediatePropagation === "function") {
            event.stopImmediatePropagation();
        }
    }, true);

    domNode.addEventListener("click", function(event) {
        if (!isControlClick(event)) {
            return;
        }

        var token = getUrlTokenAtClientPoint(editor, event.clientX, event.clientY);
        if (!token) {
            return;
        }

        event.preventDefault();
        event.stopPropagation();
        if (typeof event.stopImmediatePropagation === "function") {
            event.stopImmediatePropagation();
        }

        notifyJava("openUrlAt:", token.offset);
    }, true);
}

function isControlClick(event) {
    return event && event.button === 0 && (event.ctrlKey || event.metaKey);
}

function getUrlTokenAtClientPoint(editor, clientX, clientY) {
    var model = editor && editor.getModel();
    if (!model) {
        return null;
    }

    var target = editor.getTargetAtClientPoint(clientX, clientY);
    var position = target && target.position;
    if (!position) {
        return null;
    }

    return getUrlTokenAtPosition(model, position);
}

function getUrlTokenAtPosition(model, position) {
    var line = model.getLineContent(position.lineNumber);
    var index = Math.max(0, Math.min(line.length, position.column - 1));
    var start = index;
    while (start > 0 && !isUrlBoundary(line.charAt(start - 1))) {
        start--;
    }

    var end = index;
    while (end < line.length && !isUrlBoundary(line.charAt(end))) {
        end++;
    }

    while (end > start && isTrailingUrlPunctuation(line.charAt(end - 1))) {
        end--;
    }

    if (end <= start) {
        return null;
    }

    var token = line.substring(start, end);
    if (!isUrlOrMailToken(token)) {
        return null;
    }

    return {
        text: token,
        offset: model.getOffsetAt({
            lineNumber: position.lineNumber,
            column: start + 1
        })
    };
}

function isUrlBoundary(character) {
    return !character || /\s/.test(character) || character === "\"" || character === "'" || character === "<" || character === ">";
}

function isTrailingUrlPunctuation(character) {
    return character === "." || character === "," || character === ";" || character === ":"
        || character === ")" || character === "]";
}

function isUrlOrMailToken(token) {
    var lower = token.toLowerCase();
    if (lower.indexOf("http://") === 0 || lower.indexOf("https://") === 0
            || lower.indexOf("ftp://") === 0 || lower.indexOf("file://") === 0
            || lower.indexOf("www.") === 0) {
        return true;
    }

    var at = token.indexOf("@");
    return at > 0 && at < token.length - 1 && token.indexOf(" ", at) === -1;
}

function containsPosition(range, position) {
    if (!range || !position) {
        return false;
    }

    if (position.lineNumber < range.startLineNumber || position.lineNumber > range.endLineNumber) {
        return false;
    }

    if (position.lineNumber === range.startLineNumber && position.column < range.startColumn) {
        return false;
    }

    return !(position.lineNumber === range.endLineNumber && position.column > range.endColumn);
}

function ensureLineCount(model, lineNumber) {
    var missingLines = lineNumber - model.getLineCount();
    if (missingLines <= 0) {
        return;
    }

    var lastLine = model.getLineCount();
    var lastColumn = model.getLineMaxColumn(lastLine);
    model.pushEditOperations([], [{
        range: new monaco.Range(lastLine, lastColumn, lastLine, lastColumn),
        text: repeatChar("\n", missingLines),
        forceMoveMarkers: true
    }], function() {
        return null;
    });
}

function executeMonacoEdits(source, edits, selections) {
    var editor = getViewEditor();
    if (!editor || !edits || edits.length === 0) {
        return;
    }

    editor.focus();
    editor.pushUndoStop();
    editor.executeEdits(source, edits, selections || null);
    editor.pushUndoStop();
    notifySelection();
    notifyUndoStateSoon();
}

function transformSelectedLines(source, transformLine) {
    var editor = getViewEditor();
    var model = editor && editor.getModel();
    var selections = editor && editor.getSelections();
    if (!editor || !model || !selections || selections.length === 0) {
        return;
    }

    var edits = [];
    for (var i = 0; i < selections.length; i++) {
        var selection = selections[i];
        var startLine = selection.startLineNumber;
        var endLine = selection.endLineNumber;
        if (!selection.isEmpty() && selection.endColumn === 1) {
            endLine--;
        }

        for (var lineNumber = startLine; lineNumber <= endLine; lineNumber++) {
            var line = model.getLineContent(lineNumber);
            var replacement = transformLine(line);
            if (replacement !== line) {
                edits.push({
                    range: new monaco.Range(lineNumber, 1, lineNumber, line.length + 1),
                    text: replacement,
                    forceMoveMarkers: true
                });
            }
        }
    }

    executeMonacoEdits(source, edits, selections);
}

function transformText(text, kind) {
    if (kind === "lower") {
        return text.toLowerCase();
    }
    if (kind === "upper") {
        return text.toUpperCase();
    }
    if (kind === "capitalize") {
        return capitalizeText(text);
    }
    return text;
}

function capitalizeText(text) {
    if (text.length === 1) {
        return isUpperCase(text) ? text.toLowerCase() : text.toUpperCase();
    }

    var chars = text.split("");
    var nonLetter = true;
    for (var i = 0; i < chars.length; i++) {
        if (/[A-Za-z]/.test(chars[i])) {
            if (nonLetter) {
                chars[i] = isUpperCase(chars[i]) ? chars[i].toLowerCase() : chars[i].toUpperCase();
                nonLetter = false;
            }
        } else {
            nonLetter = true;
        }
    }
    return chars.join("");
}

function isUpperCase(text) {
    return text === text.toUpperCase() && text !== text.toLowerCase();
}

/*
 * A quote opens a string or transposes a matrix, and only the tokenizer that
 * colours the file tells the two apart. Tokenize as Scilab whatever the
 * language of the model: the conversion is defined by the Scilab syntax.
 */
function convertSingleQuotedStrings(text) {
    var pieces = text.split(/(\r\n|\r|\n)/);
    var lineTokens = monaco.editor.tokenize(text, "scilab");
    var result = "";

    for (var line = 0; line * 2 < pieces.length; line++) {
        result += convertQuotedLine(pieces[line * 2], lineTokens[line] || []);
        var separator = pieces[line * 2 + 1];
        if (separator !== undefined) {
            result += separator;
        }
    }

    return result;
}

function convertQuotedLine(content, tokens) {
    var characters = content.split("");
    var start = -1;

    for (var i = 0; i <= tokens.length; i++) {
        if (i < tokens.length && isStringToken(tokens[i].type)) {
            if (start < 0) {
                start = tokens[i].offset;
            }
            continue;
        }

        if (start >= 0) {
            // A run of string tokens covers one literal, its quotes included.
            var end = (i < tokens.length ? tokens[i].offset : content.length) - 1;
            if (end > start && characters[start] === "'" && characters[end] === "'") {
                characters[start] = "\"";
                characters[end] = "\"";
            }
            start = -1;
        }
    }

    return characters.join("");
}

function isStringToken(type) {
    // Monarch appends the language id, hence the prefix test.
    if (type.indexOf("string.invalid") === 0) {
        return false;
    }

    return type.indexOf("string") === 0 || type.indexOf("latex.string") === 0;
}

function repeatChar(character, count) {
    var value = "";
    for (var i = 0; i < count; i++) {
        value += character;
    }
    return value;
}

function resetUndoRedoState() {
    var state = window.SciNotesMonacoState;
    state.undoDepth = 0;
    state.redoDepth = 0;
}

function updateSavedAlternativeVersion() {
    var state = window.SciNotesMonacoState;
    var model = state.editor && state.editor.getModel();
    state.savedAlternativeVersionId = state.pendingClean && model
        ? model.getAlternativeVersionId() : null;
}

function notifyModifiedState() {
    var state = window.SciNotesMonacoState;
    var model = state.editor && state.editor.getModel();
    var modified = !model || state.savedAlternativeVersionId === null
        || model.getAlternativeVersionId() !== state.savedAlternativeVersionId;
    state.pendingClean = !modified;
    if (typeof getActiveDocumentEntry === "function") {
        var activeDocument = getActiveDocumentEntry();
        if (activeDocument) {
            activeDocument.clean = !modified;
        }
    }
    notifyJava("modified:", String(modified));
}

function recordUndoableEdit(e) {
    var state = window.SciNotesMonacoState;
    if (e && e.isUndoing) {
        state.undoDepth = Math.max(0, state.undoDepth - 1);
        state.redoDepth++;
    } else if (e && e.isRedoing) {
        state.redoDepth = Math.max(0, state.redoDepth - 1);
        state.undoDepth++;
    } else {
        state.undoDepth++;
        state.redoDepth = 0;
    }
}

function findOrMove(commandId) {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }

    var canMove = isFindWidgetVisible() && hasFindSearchText();
    editor.focus();
    if (canMove) {
        editor.trigger("scinotes", commandId, null);
    } else {
        editor.trigger("scinotes", "actions.find", null);
    }
}

function isFindWidgetVisible() {
    var findWidget = queryInViewEditor(".find-widget");
    if (!findWidget) {
        return false;
    }

    if (findWidget.classList.contains("visible")) {
        return true;
    }

    var style = window.getComputedStyle(findWidget);
    return style.display !== "none" && style.visibility !== "hidden" && style.opacity !== "0";
}

function hasFindSearchText() {
    var findWidget = queryInViewEditor(".find-widget");
    var inputs = findWidget ? findWidget.querySelectorAll("input, textarea") : [];
    for (var i = 0; i < inputs.length; i++) {
        if (inputs[i].value && inputs[i].value.length > 0) {
            return true;
        }
    }
    return false;
}

/*
 * Each editor view has its own find widget; in a split, the page holds two.
 */
function queryInViewEditor(selector) {
    var editor = getViewEditor();
    var domNode = editor && editor.getDomNode();
    return domNode ? domNode.querySelector(selector) : null;
}

function notifyJava(prefix, value) {
    var state = window.SciNotesMonacoState;
    notifyJavaForDocument(state && state.activeDocumentId, prefix, value);
}

function notifyJavaForDocument(documentId, prefix, value) {
    if (typeof window.ScilabMonaco !== "function") {
        return;
    }

    window.ScilabMonaco({
        request: "document:" + (documentId ? String(documentId) : "") + ":" + prefix + value,
        onSuccess: function() {},
        onFailure: function() {}
    });
}

/*
 * Ctrl+wheel (Cmd+wheel on macOS) changes the SciNotes font size, shared by
 * every tab and saved, as in the former editor. Captured before Monaco, which
 * would scroll. A trackpad pinch sends many small deltas: they are summed so
 * that one wheel notch is one step.
 */
/*
 * Hover height: a hover taller than HOVER_MAX_HEIGHT gets a scrollbar, unless
 * it is so by HOVER_HEIGHT_TOLERANCE at most, then it is shown whole: no
 * scrollbar for a few pixels. Monaco has no option for it; this replaces
 * _updateMaxDimensions() of its content hover widget, where it computes its
 * maximum just before measuring the contents. Should the internals change,
 * nothing is replaced and Monaco keeps its own maximum.
 */
var HOVER_MAX_HEIGHT = 250;
var HOVER_HEIGHT_TOLERANCE = 50;

function limitHoverHeight(editor) {
    var controller = editor && editor.getContribution("editor.contrib.contentHover");
    var wrapper = controller && controller._contentWidget;
    var widget = wrapper && wrapper._contentHoverWidget;
    if (!widget || widget.__scinotesHoverHeight
            || typeof widget._updateMaxDimensions !== "function"
            || !widget._resizableNode || !widget._hover || !widget._hover.contentsDomNode
            || typeof widget._setHoverWidgetMaxDimensions !== "function") {
        return;
    }
    widget.__scinotesHoverHeight = true;
    var monacoMaxDimensions = widget._updateMaxDimensions;
    widget._updateMaxDimensions = function() {
        monacoMaxDimensions.call(this);
        var maxSize = this._resizableNode.maxSize;
        var natural = this._hover.contentsDomNode.scrollHeight;
        var height = natural <= HOVER_MAX_HEIGHT + HOVER_HEIGHT_TOLERANCE
            ? Math.max(natural, HOVER_MAX_HEIGHT) : HOVER_MAX_HEIGHT;
        this._resizableNode.maxSize = new maxSize.constructor(maxSize.width, height);
        this._setHoverWidgetMaxDimensions(maxSize.width, height);
    };
}

function configureWheelZoom() {
    var isMac = /Mac/.test(navigator.platform);
    var pending = 0;
    window.addEventListener("wheel", function(event) {
        if (!(isMac ? event.metaKey : event.ctrlKey)) {
            return;
        }
        event.preventDefault();
        event.stopPropagation();
        var delta = event.deltaMode === 1 ? event.deltaY * 40 : event.deltaY;
        pending += delta;
        if (Math.abs(pending) >= 50) {
            notifyJava("zoom:", pending > 0 ? "1" : "-1");
            pending = 0;
        }
    }, { capture: true, passive: false });
}

function configureFileDrop() {
    function containsFiles(event) {
        var transfer = event.dataTransfer;
        if (!transfer) {
            return false;
        }
        if (transfer.files && transfer.files.length > 0) {
            return true;
        }

        var types = transfer.types || [];
        for (var i = 0; i < types.length; i++) {
            if (types[i] === "Files") {
                return true;
            }
        }
        return false;
    }

    function stopFileDrop(event) {
        if (!containsFiles(event)) {
            return false;
        }
        event.preventDefault();
        event.stopImmediatePropagation();
        if (event.dataTransfer) {
            event.dataTransfer.dropEffect = "copy";
        }
        return true;
    }

    document.addEventListener("dragenter", stopFileDrop, true);
    document.addEventListener("dragover", stopFileDrop, true);
    document.addEventListener("drop", function(event) {
        if (stopFileDrop(event)) {
            notifyJava("dropFiles:", "");
        }
    }, true);
}

function notifySelection() {
    var editor = getViewEditor();
    if (!editor) {
        return;
    }

    var model = editor.getModel();
    var position = editor.getPosition();
    var selection = editor.getSelection();
    if (!model || !position || !selection) {
        return;
    }

    var positionOffset = model.getOffsetAt(position);
    var selectionStart = model.getOffsetAt(selection.getStartPosition());
    var selectionEnd = model.getOffsetAt(selection.getEndPosition());
    notifyJava("selection:", positionOffset + ":" + selectionStart + ":" + selectionEnd);
}

function notifyUndoState() {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    var model = editor && editor.getModel();
    if (!model) {
        notifyJava("undoState:", "false:false");
        return;
    }

    /*
     * The model owns the undo history -- it is shared by the tabs on one file
     * and, between windows, kept identical by monaco-shared-history.js -- so
     * it is what the toolbar must follow. The counters below only see the
     * edits made through this editor, and the context keys lag behind an edit
     * received from another window.
     */
    if (typeof model.canUndo === "function" && typeof model.canRedo === "function") {
        notifyJava("undoState:", model.canUndo() + ":" + model.canRedo());
        return;
    }

    if (state.undoDepth > 0 || state.redoDepth > 0) {
        notifyJava("undoState:", (state.undoDepth > 0) + ":" + (state.redoDepth > 0));
        return;
    }

    if (editor._contextKeyService && typeof editor._contextKeyService.getContextKeyValue === "function") {
        notifyJava("undoState:", !!editor._contextKeyService.getContextKeyValue("canUndo") + ":" + !!editor._contextKeyService.getContextKeyValue("canRedo"));
        return;
    }

    if (typeof model.canUndo !== "function" || typeof model.canRedo !== "function") {
        notifyJava("undoState:", "false:false");
        return;
    }
    notifyJava("undoState:", model.canUndo() + ":" + model.canRedo());
}

function notifyUndoStateSoon() {
    setTimeout(notifyUndoState, 0);
}
