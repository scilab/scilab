/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

/*
 * Browser-side DAP document registry.
 *
 * One SciNotes window owns one Monaco browser, one DAP client and every tab's
 * model, so breakpoints cannot be attached to the client's lifetime. They are
 * kept per Monaco document here, grouped by canonical source path when they are
 * sent, because DAP setBreakpoints replaces the complete list of one source.
 * The registry lives on the shared state so it survives a client restart.
 */
function getDapDocuments() {
    var state = window.SciNotesMonacoState;
    state.dapDocuments = state.dapDocuments || {};
    return state.dapDocuments;
}

function normalizeDapBreakpoints(breakpoints) {
    if (!Array.isArray(breakpoints)) {
        return [];
    }
    return breakpoints.filter(function(breakpoint) {
        return breakpoint && breakpoint.line > 0;
    }).map(function(breakpoint) {
        return {
            line: breakpoint.line,
            enabled: breakpoint.enabled !== false,
            condition: breakpoint.condition || "",
            hitCondition: breakpoint.hitCondition || "",
            verified: false,
            message: ""
        };
    });
}

/*
 * The durable list Java holds, mirrored per source path. A breakpoint belongs
 * to its file, not to an open tab: without this, a line in a file no window
 * shows would not even be installed, and closing the only editor of a file
 * would disarm it.
 */
function getDapStoredBreakpoints() {
    var state = window.SciNotesMonacoState;
    state.dapStoredBreakpoints = state.dapStoredBreakpoints || {};
    return state.dapStoredBreakpoints;
}

/* Every saved source the session knows: the open documents and the store. */
function getDapSourcePaths() {
    var documents = getDapDocuments();
    var paths = {};
    Object.keys(documents).forEach(function(id) {
        if (documents[id].sourcePath) {
            paths[documents[id].sourcePath] = true;
        }
    });
    Object.keys(getDapStoredBreakpoints()).forEach(function(sourcePath) {
        if (sourcePath) {
            paths[sourcePath] = true;
        }
    });
    return Object.keys(paths);
}

/* The complete desired list for one source, as DAP SourceBreakpoint objects. */
function collectDapSourceBreakpoints(sourcePath) {
    var documents = getDapDocuments();
    var seenLines = {};
    var collected = [];
    var known = false;
    Object.keys(documents).forEach(function(id) {
        var entry = documents[id];
        if (entry.sourcePath !== sourcePath) {
            return;
        }
        /*
         * An open document leads: its list follows the edits and the toggles of
         * this window, and reaches the store afterwards. The store only speaks
         * for the files no window shows.
         */
        known = true;
        (entry.breakpoints || []).forEach(function(breakpoint) {
            if (!breakpoint.enabled || seenLines[breakpoint.line]) {
                return;
            }
            seenLines[breakpoint.line] = true;
            var requested = { line: breakpoint.line };
            if (breakpoint.condition) {
                requested.condition = breakpoint.condition;
            }
            if (breakpoint.hitCondition) {
                requested.hitCondition = breakpoint.hitCondition;
            }
            collected.push(requested);
        });
        // The line of a run to the cursor: sent as a breakpoint, never kept
        // as one.
        var runTo = window.SciNotesMonacoState.dapRunToCursor;
        if (runTo && runTo.documentId === String(entry.id) && !seenLines[runTo.line]) {
            seenLines[runTo.line] = true;
            collected.push({ line: runTo.line });
        }
    });

    if (known === false) {
        (getDapStoredBreakpoints()[sourcePath] || []).forEach(function(breakpoint) {
            if (breakpoint.enabled === false || !(breakpoint.line > 0) || seenLines[breakpoint.line]) {
                return;
            }
            seenLines[breakpoint.line] = true;
            var requested = { line: breakpoint.line };
            if (breakpoint.condition) {
                requested.condition = breakpoint.condition;
            }
            if (breakpoint.hitCondition) {
                requested.hitCondition = breakpoint.hitCondition;
            }
            collected.push(requested);
        });
    }

    return collected.sort(function(left, right) {
        return left.line - right.line;
    });
}

/* Fold the server verification back onto the documents showing that source. */
function applyDapVerification(sourcePath, requested, actual) {
    if (!Array.isArray(actual)) {
        return;
    }

    var byLine = {};
    for (var i = 0; i < requested.length && i < actual.length; i++) {
        byLine[requested[i].line] = actual[i];
    }
    var documents = getDapDocuments();
    var movedEntry = null;
    Object.keys(documents).forEach(function(id) {
        var entry = documents[id];
        if (entry.sourcePath !== sourcePath) {
            return;
        }
        captureDapBreakpointDecorationLines(id);
        // A dirty document's lines follow its edits, not the saved file the
        // server read: they cannot be moved to the server's lines.
        var canMove = isDapDocumentClean(id);
        var moved = false;
        (entry.breakpoints || []).forEach(function(breakpoint) {
            var resolved = byLine[breakpoint.line];
            if (!resolved) {
                return;
            }
            var line = Number.isFinite(resolved.line) ? resolved.line : breakpoint.line;
            breakpoint.verified = resolved.verified === true;
            breakpoint.message = getDapBreakpointMessage(resolved);
            if (canMove && breakpoint.verified && line !== breakpoint.line) {
                breakpoint.line = line;
                delete breakpoint.actualLine;
                moved = true;
            } else {
                breakpoint.actualLine = line;
            }
        });
        if (moved) {
            mergeDapBreakpointLines(entry);
            movedEntry = entry;
        }
        // Decorations first: refreshing reads their ranges back, which would
        // otherwise return a moved breakpoint to its old line.
        updateDapBreakpointDecorations(id);
        refreshDapPresentation(id);
    });

    // The store follows the file: one report is enough for all its tabs.
    if (movedEntry) {
        persistDapBreakpoints(movedEntry);
    }
}

/* Two breakpoints moved onto one line become one. */
function mergeDapBreakpointLines(entry) {
    var seenLines = {};
    entry.breakpoints = (entry.breakpoints || []).slice().sort(function(left, right) {
        return left.line - right.line;
    }).filter(function(breakpoint) {
        if (seenLines[breakpoint.line]) {
            return false;
        }
        seenLines[breakpoint.line] = true;
        return true;
    });
}

/* The reason of a refused breakpoint, in the user's language when it is known. */
function getDapBreakpointMessage(resolved) {
    var keys = {
        noCode: "breakpointNoCode",
        hitCount: "breakpointHitCount",
        noFile: "breakpointNoFile"
    };
    var key = keys[resolved.scilabReason];
    return key ? getDapText(key) : resolved.message || "";
}

function getDapActiveEntry() {
    var state = window.SciNotesMonacoState;
    return state.activeDocumentId ? getDapDocuments()[state.activeDocumentId] || null : null;
}

function findDapBreakpoint(entry, lineNumber) {
    var breakpoints = entry && entry.breakpoints || [];
    for (var i = 0; i < breakpoints.length; i++) {
        if (breakpoints[i].line === lineNumber) {
            return i;
        }
    }
    return -1;
}

/*
 * A verified breakpoint may have been moved by the adapter.  The stopped
 * frame reports that resolved line, so use it as well as the requested line
 * when composing the single stopped marker.
 */
function getDapBreakpointAtLine(entry, lineNumber) {
    var breakpoints = entry && entry.breakpoints || [];
    for (var i = 0; i < breakpoints.length; i++) {
        if (breakpoints[i].line === lineNumber || breakpoints[i].actualLine === lineNumber) {
            return breakpoints[i];
        }
    }
    return null;
}

/* A stop reported as an exception is presented in red instead of amber. */
function isDapStoppedOnError() {
    var stopped = window.SciNotesMonacoState.dapStoppedLocation;
    return !!stopped && stopped.reason === "exception";
}

/* The chevron injected in the code, before the instruction being executed. */
function getDapStoppedMarkClass(isError) {
    return isError
        ? "scinotes-stopped-mark scinotes-stopped-mark-error"
        : "scinotes-stopped-mark";
}

/*
 * The same stop in the glyph margin. A click on it is an ordinary gutter
 * click, which adds or removes the breakpoint of that line.
 */
function getDapStoppedGlyphClass(isError) {
    return isError
        ? "scinotes-stopped-glyph scinotes-stopped-glyph-error codicon-debug-stackframe"
        : "scinotes-stopped-glyph codicon-debug-stackframe";
}

/*
 * The error message goes at the end of its line, so it has to hold on one
 * line: Scilab messages often carry the faulty statement and its location on
 * several lines. The hover keeps the whole text.
 */
function getDapStoppedErrorLabel(text) {
    var label = String(text || "").replace(/\s+/g, " ").trim();
    if (label.length > 120) {
        label = label.slice(0, 117) + "...";
    }
    return label;
}

/*
 * Scilab stops on an instruction, not on a line: `a = 1; b = 2` reports the
 * column of the instruction it stopped on. The chevron marks that instruction
 * only when it starts after the code does: a stop on the first one is already
 * shown by the arrow of the margin.
 *
 * @return a collapsed range at that column, or null when there is nothing to mark
 */
function getDapStoppedMarkRange(model, location) {
    var instruction = getDapStoppedInstructionRange(model, location);
    return instruction
        ? new monaco.Range(location.line, instruction.startColumn, location.line, instruction.startColumn)
        : null;
}

function getDapStoppedInstructionRange(model, location) {
    var column = location.column;
    if (!(column > 1)
            || typeof model.getLineFirstNonWhitespaceColumn !== "function"
            || typeof model.getLineMaxColumn !== "function") {
        return null;
    }

    var firstColumn = model.getLineFirstNonWhitespaceColumn(location.line);
    var lastColumn = model.getLineMaxColumn(location.line);
    if (!(firstColumn > 0) || column <= firstColumn || column >= lastColumn) {
        return null;
    }

    return new monaco.Range(location.line, column, location.line, lastColumn);
}

/*
 * Breakpoints another client of the session owns. Scilab runs the union of
 * every client's list, so a line set from VS Code stops the execution here too:
 * it is shown, hollow, to explain a stop the user did not ask for, and it is
 * not ours to publish or to persist.
 */
function getDapForeignBreakpoints() {
    var state = window.SciNotesMonacoState;
    state.dapForeignBreakpoints = state.dapForeignBreakpoints || {};
    return state.dapForeignBreakpoints;
}

function applyDapForeignBreakpoint(body) {
    var breakpoint = body && body.breakpoint || {};
    var sourcePath = breakpoint.source && breakpoint.source.path;
    if (!sourcePath || !(breakpoint.id > 0)) {
        return;
    }

    var foreign = getDapForeignBreakpoints();
    var bySource = foreign[sourcePath] = foreign[sourcePath] || {};
    if (body.reason === "removed") {
        delete bySource[breakpoint.id];
    } else if (breakpoint.line > 0) {
        bySource[breakpoint.id] = breakpoint.line;
    }

    var documents = getDapDocuments();
    Object.keys(documents).forEach(function(documentId) {
        if (documents[documentId].sourcePath === sourcePath) {
            updateDapForeignDecorations(documentId);
        }
    });
}

/* The session is over: nobody else's breakpoints are installed any more. */
function clearDapForeignBreakpoints() {
    var state = window.SciNotesMonacoState;
    state.dapForeignBreakpoints = {};
    var documents = getDapDocuments();
    Object.keys(documents).forEach(updateDapForeignDecorations);
}

function updateDapForeignDecorations(documentId) {
    var entry = getDapDocuments()[documentId];
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = entry && registry && registry[documentId];
    var model = document && document.model;
    if (!model || typeof model.deltaDecorations !== "function") {
        return;
    }

    var lines = [];
    var bySource = getDapForeignBreakpoints()[entry.sourcePath] || {};
    Object.keys(bySource).forEach(function(id) {
        var line = bySource[id];
        if (!(line > 0) || findDapBreakpoint(entry, line) !== -1) {
            /*
             * A line we own carries our own glyph. Forget it rather than hide
             * it: otherwise removing our breakpoint later would uncover a
             * marker for a line we were never told about again.
             */
            delete bySource[id];
            return;
        }

        if (lines.indexOf(line) === -1) {
            lines.push(line);
        }
    });

    entry.foreignDecorations = model.deltaDecorations(entry.foreignDecorations || [],
        lines.map(function(line) {
            return {
                range: new monaco.Range(line, 1, line, 1),
                options: {
                    glyphMarginClassName:
                        "scinotes-breakpoint scinotes-breakpoint-external codicon-debug-breakpoint-unverified",
                    glyphMarginHoverMessage: { value: getDapText("breakpointForeign") }
                }
            };
        }));
}

/*
 * Monaco tracks decoration ranges through edits. Fold those live ranges back
 * into the browser's desired breakpoint list without publishing them: DAP
 * still describes the saved file until Java confirms a successful save.
 */
function captureDapBreakpointDecorationLines(documentId) {
    var entry = getDapDocuments()[documentId];
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = entry && registry && registry[documentId];
    var model = document && document.model;
    var breakpoints = entry && entry.breakpoints || [];
    var decorations = entry && entry.decorations || [];
    if (!model || typeof model.getDecorationRange !== "function"
            || breakpoints.length !== decorations.length) {
        return false;
    }

    var pairs = [];
    var changed = false;
    for (var i = 0; i < breakpoints.length; i++) {
        var range = model.getDecorationRange(decorations[i]);
        if (!range || !(range.startLineNumber > 0)) {
            changed = true;
            continue;
        }
        if (breakpoints[i].line !== range.startLineNumber) {
            breakpoints[i].line = range.startLineNumber;
            breakpoints[i].verified = false;
            breakpoints[i].message = "";
            delete breakpoints[i].actualLine;
            changed = true;
        }
        pairs.push({ breakpoint: breakpoints[i], decoration: decorations[i] });
    }

    pairs.sort(function(left, right) {
        return left.breakpoint.line - right.breakpoint.line;
    });
    var seenLines = {};
    var duplicateDecorations = [];
    entry.breakpoints = [];
    entry.decorations = [];
    pairs.forEach(function(pair) {
        if (seenLines[pair.breakpoint.line]) {
            duplicateDecorations.push(pair.decoration);
            changed = true;
            return;
        }
        seenLines[pair.breakpoint.line] = true;
        entry.breakpoints.push(pair.breakpoint);
        entry.decorations.push(pair.decoration);
    });
    if (duplicateDecorations.length && typeof model.deltaDecorations === "function") {
        model.deltaDecorations(duplicateDecorations, []);
    }
    return changed;
}

function isDapDocumentClean(documentId) {
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = registry && registry[documentId];
    return !document || document.clean !== false;
}

/*
 * Consulted by composeLineNumbers() in monaco-bridge.js for every rendered
 * line, so it stays a plain lookup on the active document. The marker itself
 * is drawn in CSS from the decoration class: a text glyph cannot be centred
 * reliably, its position depending on the font metrics and on Monaco's own
 * right alignment.
 */
function hasDapBreakpointOnLine(lineNumber) {
    return findDapBreakpoint(getDapActiveEntry(), lineNumber) >= 0;
}

/*
 * State colour for the marker. Decorations are attached to the model, not to
 * the editor, so they follow the document across tab switches instead of being
 * rebuilt or lost.
 */
function updateDapBreakpointDecorations(documentId) {
    var documents = getDapDocuments();
    var entry = documents[documentId];
    if (!entry) {
        return;
    }

    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = registry && registry[documentId];
    var model = document && document.model;
    if (!model || typeof model.deltaDecorations !== "function") {
        return;
    }

    var previousDecorations = entry.decorationModel === model ? entry.decorations || [] : [];
    entry.decorations = model.deltaDecorations(previousDecorations,
        (entry.breakpoints || []).map(function(breakpoint) {
            var status = "pending";
            if (!breakpoint.enabled) {
                status = "disabled";
            } else if (breakpoint.verified) {
                status = "verified";
            }
            var hover = breakpoint.message
                || getDapText(breakpoint.enabled ? "breakpoint" : "breakpointDisabled");

            return {
                // Collapsed, not whole-line: a glyph is drawn on every view
                // line its range covers, so a wrapped line would get one per row.
                range: new monaco.Range(breakpoint.line, 1, breakpoint.line, 1),
                options: {
                    // The codicon class is what gives the cell the codicon
                    // font (`.codicon[class*=codicon-]` in Monaco's CSS).
                    glyphMarginClassName: "scinotes-breakpoint scinotes-breakpoint-" + status
                        + " codicon-debug-breakpoint",
                    glyphMarginHoverMessage: { value: hover },
                    overviewRuler: {
                        color: "#d64a4a",
                        position: monaco.editor.OverviewRulerLane.Left
                    }
                }
            };
        }));
    entry.decorationModel = model;
    updateDapForeignDecorations(documentId);
}

/*
 * Repaint one document's markers. Breakpoints live in their own gutter, so
 * this is a decoration update only: the line-number margin is untouched.
 */
function refreshDapPresentation(documentId) {
    captureDapBreakpointDecorationLines(documentId);
    updateDapBreakpointDecorations(documentId);
    var state = window.SciNotesMonacoState;
    var entry = getDapDocuments()[documentId];
    var stopped = state.dapStoppedLocation;
    if (entry && stopped && stopped.sourcePath === entry.sourcePath) {
        // Recompose the marks if a breakpoint changed while paused.
        applyDapStoppedLocation();
    }
}

/* The margin columns: glyph margin, line numbers, line decorations. */
function isGutterTarget(targetType) {
    var types = monaco.editor.MouseTargetType;
    return targetType === types.GUTTER_GLYPH_MARGIN
        || targetType === types.GUTTER_LINE_NUMBERS
        || targetType === types.GUTTER_LINE_DECORATIONS;
}

/*
 * Ghost marker following the pointer down the gutter, so an empty gutter still
 * shows where a click would land. It is view-scoped and transient, unlike the
 * real breakpoints which belong to their model: each half of a split ghosts
 * the line its own pointer is on.
 *
 * @param lineNumber the line to ghost, 0 to clear
 * @param view the editor the pointer is in; the primary one by default
 */
function setDapHoverLine(lineNumber, view) {
    var state = window.SciNotesMonacoState;
    var editor = view || state.editor;
    if (!editor) {
        return;
    }

    // Never ghost a line that already carries a breakpoint.
    if (lineNumber && hasDapBreakpointOnLine(lineNumber)) {
        lineNumber = 0;
    }
    var key = editor === state.secondaryEditor ? "secondaryDapHover" : "dapHover";
    if (state[key + "Line"] === lineNumber) {
        return;
    }

    state[key + "Line"] = lineNumber;
    state[key + "Decorations"] = editor.deltaDecorations(state[key + "Decorations"] || [],
        lineNumber ? [{
            range: new monaco.Range(lineNumber, 1, lineNumber, 1),
            options: {
                glyphMarginClassName: "scinotes-breakpoint scinotes-breakpoint-hover codicon-debug-breakpoint"
            }
        }] : []);
}

/*
 * The line the debugger is stopped on. There is only ever one, but it belongs
 * to a model like the breakpoints do, so switching tabs neither loses it nor
 * leaves it behind on the wrong document.
 */
function clearDapStoppedDecorations() {
    var state = window.SciNotesMonacoState;
    var previous = state.dapStoppedDocumentId;
    if (previous) {
        var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
        var document = registry && registry[previous];
        if (document && document.model && typeof document.model.deltaDecorations === "function") {
            document.model.deltaDecorations(state.dapStoppedDecorations || [], []);
        }
    }
    state.dapStoppedDecorations = [];
    state.dapStoppedDocumentId = null;
}

function clearDapStopped() {
    var state = window.SciNotesMonacoState;
    // Dropped first: the breakpoint glyphs are composed against this location.
    state.dapStoppedLocation = null;
    clearDapStoppedDecorations();
    updateDapStaleNotice();
}

/*
 * Every text the editor shows, next to DAP_CLIENT_TEXTS of the client. Java
 * hands over the translated ones with the DAP configuration (SciNotesMessages);
 * these stand in until then.
 */
var DAP_TEXTS = {
    breakpoint: "Breakpoint",
    breakpointDisabled: "Breakpoint (disabled)",
    breakpointForeign: "Breakpoint owned by another debug client",
    breakpointNoCode: "Unreachable breakpoint",
    breakpointHitCount: "Invalid hit count",
    breakpointNoFile: "Unsaved file",
    stoppedHere: "Execution paused here",
    stoppedHereOnBreakpoint: "Execution paused here (breakpoint)",
    fileChangedSinceRun: "This file has changed since it was run: Scilab executes the code "
        + "as it was, and the lines shown may not match it."
};

function getDapText(key) {
    var config = window.SciNotesMonacoState.pendingDapConfig;
    var messages = config && config.messages;
    return messages && messages[key] || DAP_TEXTS[key] || DAP_CLIENT_TEXTS[key] || "";
}

function getDapDocumentModel(entry) {
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = entry && registry && registry[entry.id];
    return document && document.model || null;
}

/*
 * The text of each file as the running execution found it. Scilab runs what
 * it read when the run started, so once the file shown differs from it, the
 * lines of a stop no longer describe the code being executed. Called on every
 * state change of the session: a run starts at the first running or stopped
 * state after an idle one, and ends with it.
 */
function followDapRunState(sessionState, debugMode) {
    var state = window.SciNotesMonacoState;
    // Only a debugged execution can stop and show lines: what runs without
    // the debugger, each console line and callback included, costs nothing.
    if (sessionState === "stopped" || (sessionState === "running" && debugMode)) {
        if (!state.dapRunVersions) {
            state.dapRunVersions = {};
            var documents = getDapDocuments();
            Object.keys(documents).forEach(function(id) {
                noteDapRunVersion(documents[id]);
            });
        }
        return;
    }
    state.dapRunVersions = null;
    updateDapStaleNotice();
}

/* A file first opened during the run -- to show a stop -- is read as it is then. */
function noteDapRunVersion(entry) {
    var versions = window.SciNotesMonacoState.dapRunVersions;
    var model = getDapDocumentModel(entry);
    if (!versions || !entry || !entry.sourcePath || versions[entry.sourcePath]
            || !model || typeof model.getValue !== "function") {
        return;
    }
    versions[entry.sourcePath] = {
        model: model,
        version: typeof model.getAlternativeVersionId === "function"
            ? model.getAlternativeVersionId() : null,
        text: model.getValue()
    };
}

/*
 * Whether the file shown no longer holds the text the run started from,
 * saved or not. Undoing back to that text makes it current again.
 */
function isDapSourceChangedSinceRun(sourcePath) {
    var versions = window.SciNotesMonacoState.dapRunVersions;
    var run = versions && versions[sourcePath];
    var model = getDapDocumentModel(findDapDocumentBySource(sourcePath));
    if (!run || !model) {
        return false;
    }
    if (run.model === model && run.version !== null
            && model.getAlternativeVersionId() === run.version) {
        return false;
    }
    return model.getValue() !== run.text;
}

/*
 * Warn over the file shown when it is the one stopped in and it changed
 * since the run started. Followed through the edits of that file while
 * stopped, and through tab switches.
 */
function updateDapStaleNotice() {
    var state = window.SciNotesMonacoState;
    var stopped = state.dapStoppedLocation;
    var active = getDapDocuments()[state.activeDocumentId];
    var stoppedModel = stopped ? getDapDocumentModel(findDapDocumentBySource(stopped.sourcePath)) : null;

    if (state.dapStaleWatchedModel !== stoppedModel) {
        if (state.dapStaleWatcher) {
            state.dapStaleWatcher.dispose();
        }
        state.dapStaleWatcher = stoppedModel && typeof stoppedModel.onDidChangeContent === "function"
            ? stoppedModel.onDidChangeContent(updateDapStaleNotice) : null;
        state.dapStaleWatchedModel = stoppedModel;
    }

    var stale = !!stopped && !!active && active.sourcePath === stopped.sourcePath
        && isDapSourceChangedSinceRun(stopped.sourcePath);
    setDapStaleNotice(state.editor, stale);
    setDapStaleNotice(state.secondaryEditor, stale);
    if (stale !== !!state.dapStaleNoticeShown) {
        state.dapStaleNoticeShown = stale;
        if (stale && typeof notifyJava === "function") {
            notifyJava("infoBar:", getDapText("fileChangedSinceRun"));
        }
    }
}

/* Height of the banner, in CSS as well: the text above it moves down by as much. */
var DAP_STALE_NOTICE_HEIGHT = 22;

/*
 * A bar across the top of the view. The top padding grows by its height while
 * it is shown, so that it never hides the first line; further lines pass under
 * it when scrolling, as under Monaco's sticky scroll.
 */
function setDapStaleNotice(editor, visible) {
    if (!editor || typeof editor.addOverlayWidget !== "function") {
        return;
    }
    var widget = editor.__scinotesDapStaleNotice;
    if (visible && !widget) {
        var text = getDapText("fileChangedSinceRun");
        var node = document.createElement("div");
        node.className = "scinotes-dap-stale-notice";
        node.textContent = text;
        node.title = text;
        widget = {
            getId: function() {
                return "scinotes.dapStaleNotice";
            },
            getDomNode: function() {
                return node;
            },
            getPosition: function() {
                // Placed by its CSS: Monaco only offers corners and the center.
                return null;
            }
        };
        editor.addOverlayWidget(widget);
        editor.__scinotesDapStaleNotice = widget;
        var padding = editor.getOption(monaco.editor.EditorOption.padding) || {};
        editor.__scinotesDapStalePadding = padding;
        editor.updateOptions({ padding: {
            top: (padding.top || 0) + DAP_STALE_NOTICE_HEIGHT,
            bottom: padding.bottom || 0
        } });
    } else if (!visible && widget) {
        editor.removeOverlayWidget(widget);
        editor.__scinotesDapStaleNotice = null;
        var saved = editor.__scinotesDapStalePadding || {};
        editor.updateOptions({ padding: { top: saved.top || 0, bottom: saved.bottom || 0 } });
        editor.__scinotesDapStalePadding = null;
    }
}

/*
 * Find the open document showing one canonical source path. Only one whose
 * model is still in the registry can carry a mark.
 */
function findDapDocumentBySource(sourcePath) {
    var documents = getDapDocuments();
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var ids = Object.keys(documents);
    for (var i = 0; i < ids.length; i++) {
        var document = registry && registry[ids[i]];
        if (documents[ids[i]].sourcePath === sourcePath && document && document.model) {
            return documents[ids[i]];
        }
    }
    return null;
}

/*
 * Decorate the stopped line if its document is open. The source may only be
 * opened later, after Java has been asked for it, so the location is kept and
 * replayed when a document registers that path.
 */
/*
 * `reveal` is for arriving at a stop, not for repainting one: recomposing the
 * decorations when a breakpoint is toggled must not drag the view back to the
 * stopped line the user just navigated away from.
 */
function applyDapStoppedLocation(reveal) {
    var state = window.SciNotesMonacoState;
    var location = state.dapStoppedLocation;
    clearDapStoppedDecorations();
    if (!location || !location.sourcePath || !(location.line > 0)) {
        return;
    }

    var entry = findDapDocumentBySource(location.sourcePath);
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = entry && registry && registry[entry.id];
    var model = document && document.model;
    if (!model || typeof model.deltaDecorations !== "function") {
        return;
    }

    var isError = isDapStoppedOnError();
    var hover = location.text || (getDapBreakpointAtLine(entry, location.line)
        ? getDapText("stoppedHereOnBreakpoint")
        : getDapText("stoppedHere"));
    var instruction = getDapStoppedInstructionRange(model, location);
    var decorations = [{
        range: new monaco.Range(location.line, 1, location.line, 1),
        options: {
            isWholeLine: true,
            className: isError
                ? "scinotes-stopped-line scinotes-stopped-line-error"
                : "scinotes-stopped-line",
            // The instruction carries the same text when there is one; two
            // decorations saying it would stack two entries in one tooltip.
            hoverMessage: instruction ? undefined : { value: hover },
            overviewRuler: {
                color: isError ? "#d64a4a" : "#e8b339",
                position: monaco.editor.OverviewRulerLane.Full
            }
        }
    }, {
        // Apart from the highlight, which covers every row of a wrapped line:
        // the arrow belongs to its first row only.
        range: new monaco.Range(location.line, 1, location.line, 1),
        options: {
            glyphMarginClassName: getDapStoppedGlyphClass(isError)
        }
    }];

    var mark = getDapStoppedMarkRange(model, location);
    if (mark) {
        decorations.push({
            range: mark,
            options: {
                // Injected content, so it never enters the document: it is
                // neither editable nor copied with the code.
                before: {
                    content: " ",
                    inlineClassName: getDapStoppedMarkClass(isError)
                },
                showIfCollapsed: true
            }
        });
    }

    var errorLabel = isError ? getDapStoppedErrorLabel(location.text) : "";
    var lastColumn = typeof model.getLineMaxColumn === "function"
        ? model.getLineMaxColumn(location.line) : 0;
    if (errorLabel && lastColumn > 0) {
        // Injected text, so it never enters the document: the range is empty
        // and sits at the end of the line, past the last character.
        decorations.push({
            range: new monaco.Range(location.line, lastColumn, location.line, lastColumn),
            options: {
                after: {
                    content: "  " + errorLabel,
                    inlineClassName: "scinotes-stopped-error-text"
                },
                showIfCollapsed: true
            }
        });
    }

    if (instruction) {
        decorations.push({
            range: instruction,
            options: {
                className: isError
                    ? "scinotes-stopped-instruction scinotes-stopped-instruction-error"
                    : "scinotes-stopped-instruction",
                hoverMessage: { value: hover }
            }
        });
    }

    state.dapStoppedDocumentId = entry.id;
    state.dapStoppedDecorations = model.deltaDecorations([], decorations);
    updateDapStaleNotice();

    if (reveal && state.activeDocumentId === entry.id && state.editor) {
        state.editor.revealLineInCenterIfOutsideViewport(location.line);
    }
}

/* Add or remove a breakpoint on the active document, then publish it. */
function toggleDapBreakpoint(lineNumber) {
    var state = window.SciNotesMonacoState;
    var documentId = state.activeDocumentId;
    if (!documentId || !(lineNumber > 0)) {
        return false;
    }

    var documents = getDapDocuments();
    var entry = documents[documentId];
    if (!entry) {
        // Java pushes a document only once the DAP endpoint runs; a breakpoint
        // may be placed before that, and stays pending until a path arrives.
        entry = { id: String(documentId), uri: "", sourcePath: "", breakpoints: [] };
        documents[documentId] = entry;
    }
    entry.breakpoints = entry.breakpoints || [];
    captureDapBreakpointDecorationLines(documentId);

    var index = findDapBreakpoint(entry, lineNumber);
    if (index >= 0) {
        entry.breakpoints.splice(index, 1);
    } else {
        entry.breakpoints.push({
            line: lineNumber,
            enabled: true,
            condition: "",
            hitCondition: "",
            verified: false,
            message: ""
        });
        entry.breakpoints.sort(function(left, right) {
            return left.line - right.line;
        });
    }

    refreshDapPresentation(documentId);
    if (isDapDocumentClean(documentId)) {
        if (state.dapClient && entry.sourcePath) {
            state.dapClient.syncSource(entry.sourcePath);
        }
        persistDapBreakpoints(entry);
    }
    return true;
}

/*
 * The gutter interaction of one view. Only the dedicated glyph margin toggles
 * breakpoints, leaving line-number selection and anchors alone.
 *
 * @param editor the view to wire
 * @return the listeners, to dispose when that view goes away
 */
function attachDapGutterListeners(editor) {
    return [
        editor.onMouseDown(function(event) {
            var target = event && event.target;
            if (!target || target.type !== monaco.editor.MouseTargetType.GUTTER_GLYPH_MARGIN) {
                return;
            }
            if (event.event && event.event.rightButton) {
                return;
            }
            var position = target.position;
            if (!position) {
                return;
            }
            setDapHoverLine(0, editor);
            toggleDapBreakpoint(position.lineNumber);
        }),

        editor.onMouseMove(function(event) {
            var target = event && event.target;
            setDapHoverLine(target && target.position && isGutterTarget(target.type)
                ? target.position.lineNumber : 0, editor);
        }),

        editor.onMouseLeave(function() {
            setDapHoverLine(0, editor);
        })
    ];
}

/*
 * The gutter interaction of the split's second view. Its breakpoints need
 * nothing: they are model decorations. Called by attachSecondaryViewFeatures().
 */
function attachDapEditorView(editor) {
    var state = window.SciNotesMonacoState;
    if (!editor || state.secondaryDapGutterListeners) {
        return;
    }
    state.secondaryDapGutterListeners = attachDapGutterListeners(editor);
    setDapStaleNotice(editor, !!state.dapStaleNoticeShown);
}

/* That view is going away: forget its listeners and its ghost. */
function detachDapEditorView() {
    var state = window.SciNotesMonacoState;
    var listeners = state.secondaryDapGutterListeners || [];
    for (var i = 0; i < listeners.length; i++) {
        listeners[i].dispose();
    }
    state.secondaryDapGutterListeners = null;
    state.secondaryDapHoverDecorations = [];
    state.secondaryDapHoverLine = 0;
}

/*
 * Install the gutter interaction of the primary view once per page, and the
 * document switch the presentation follows.
 */
function configureDapEditor() {
    var state = window.SciNotesMonacoState;
    var editor = state.editor;
    if (!editor || state.dapGutterListener) {
        return;
    }

    var listeners = attachDapGutterListeners(editor);
    state.dapGutterListener = listeners[0];
    state.dapHoverListener = listeners[1];
    state.dapMouseLeaveListener = listeners[2];

    state.dapSelectionListener = function(domEvent) {
        var detail = domEvent && domEvent.detail || {};
        if (detail.id) {
            refreshDapPresentation(detail.id);
        }
        updateDapStaleNotice();
    };
    window.addEventListener("scinotes-monaco-document-selected", state.dapSelectionListener);

    if (!state.dapValueHoverProvider) {
        state.dapValueHoverProvider = monaco.languages.registerHoverProvider("scilab", {
            provideHover: provideDapValueHover
        });
    }
}

/*
 * What the hover evaluates: the selection when the mouse is over it (one line
 * only), else the name under the mouse with the fields before it: `s.name`
 * over `name`.
 */
function getDapHoverExpression(model, position) {
    var state = window.SciNotesMonacoState;
    var editors = [state.editor, state.secondaryEditor];
    for (var i = 0; i < editors.length; i++) {
        var editor = editors[i];
        var selection = editor && editor.getModel() === model ? editor.getSelection() : null;
        if (selection && !selection.isEmpty()
                && selection.startLineNumber === selection.endLineNumber
                && selection.containsPosition(position)) {
            var text = model.getValueInRange(selection).trim();
            if (text) {
                return {
                    expression: text,
                    startColumn: selection.startColumn,
                    endColumn: selection.endColumn
                };
            }
        }
    }

    var word = model.getWordAtPosition(position);
    if (!word || /^[0-9]/.test(word.word)) {
        return null;
    }

    var line = model.getLineContent(position.lineNumber);
    var start = word.startColumn - 1;
    var fields = /(?:[%$A-Za-z_!#?\u0080-\uffff][A-Za-z_0-9!#?$\u0080-\uffff]*\.)+$/.exec(line.substring(0, start));
    if (fields) {
        start -= fields[0].length;
    }
    return {
        expression: line.substring(start, word.endColumn - 1),
        startColumn: start + 1,
        endColumn: word.endColumn
    };
}

/* Whether a line is inside a function of the document, from its AST ranges. */
function isDapLineInFunction(model, lineNumber) {
    var entries = typeof getDocumentEntriesForModel === "function"
        ? getDocumentEntriesForModel(model) : [];
    var numbers = entries.length ? entries[0].astLineNumbers : null;
    return !!(numbers && numbers.levels && numbers.levels[lineNumber - 1] > 0);
}

/*
 * The value of what is under the mouse, read by the debug adapter. Stopped: in
 * the stopped frame, and only in the stopped file, whose names that frame
 * holds. Idle: in the global scope, and not inside a function, whose names
 * exist only while it runs. Running: nothing, the values are changing.
 */
function provideDapValueHover(model, position) {
    var state = window.SciNotesMonacoState;
    var client = state.dapClient;
    var sessionState = client && typeof client.getSessionState === "function"
        ? client.getSessionState() : null;
    var stopped = state.dapStoppedLocation;
    var args = { context: "hover" };
    if (sessionState === "stopped" && stopped) {
        var documents = getDapDocuments();
        var inStoppedSource = Object.keys(documents).some(function(id) {
            return documents[id].sourcePath === stopped.sourcePath
                && getDapDocumentModel(documents[id]) === model;
        });
        if (!inStoppedSource) {
            return null;
        }
        args.frameId = stopped.frameId;
    } else if (sessionState !== "idle" || isDapLineInFunction(model, position.lineNumber)) {
        return null;
    }

    var target = getDapHoverExpression(model, position);
    if (!target) {
        return null;
    }
    args.expression = target.expression;
    var generation = client.getRunStateGeneration();

    return client.request("evaluate", args).then(function(body) {
        if (!body || typeof body.result !== "string"
                || client.getRunStateGeneration() !== generation) {
            return null;
        }
        return {
            range: new monaco.Range(position.lineNumber, target.startColumn,
                position.lineNumber, target.endColumn),
            contents: [{ value: "```\n" + target.expression + " = " + body.result + "\n```" }]
        };
    }).catch(function() {
        return null;
    });
}

window.SciNotesMonacoToggleBreakpoint = function(lineNumber) {
    return toggleDapBreakpoint(lineNumber);
};

/* The caret, in the view of a split that has the focus. */
function getDapCaretPosition() {
    var state = window.SciNotesMonacoState;
    var secondary = state.secondaryEditor;
    var editor = secondary && secondary.hasTextFocus() ? secondary : state.editor;
    return editor && editor.getPosition() || null;
}

window.SciNotesMonacoToggleBreakpointAtCaret = function() {
    var position = getDapCaretPosition();
    return position ? toggleDapBreakpoint(position.lineNumber) : false;
};

/* A run that never starts (save cancelled, Scilab busy) gives its line up. */
var DAP_RUN_TO_CURSOR_START_DELAY = 10000;

/* Forget the line of a run to the cursor, and take it out of the session. */
function clearDapRunToCursor() {
    var state = window.SciNotesMonacoState;
    var request = state.dapRunToCursor;
    if (!request) {
        return;
    }
    state.dapRunToCursor = null;
    var entry = getDapDocuments()[request.documentId];
    if (entry && entry.sourcePath && state.dapClient) {
        state.dapClient.syncSource(entry.sourcePath);
    }
}

/*
 * The line of a run to the cursor lasts one run: from the moment the session
 * runs to its next stop, wherever that is, or to its end.
 */
function followDapRunToCursor(sessionState) {
    var request = window.SciNotesMonacoState.dapRunToCursor;
    if (!request) {
        return;
    }
    if (sessionState === "running") {
        request.started = true;
    } else if (request.started) {
        clearDapRunToCursor();
    }
}

/*
 * Run to the line of the caret, as with a breakpoint set there and removed
 * once the run has stopped. The line joins the list sent to the session
 * without being drawn nor stored.
 *
 * @param resume true to continue a stopped execution; false when Java starts
 *               the run itself, after saving the file
 */
window.SciNotesMonacoRunToCursor = function(resume) {
    var state = window.SciNotesMonacoState;
    var client = state.dapClient;
    var position = getDapCaretPosition();
    var documentId = state.activeDocumentId;
    if (!client || !position || !documentId) {
        return false;
    }

    var entry = getDapDocuments()[documentId];
    if (resume) {
        if (!entry || !entry.sourcePath) {
            return false;
        }
        // The lines of a modified file are no longer those of the code that runs.
        if (!isDapDocumentClean(documentId)) {
            if (typeof notifyJava === "function") {
                notifyJava("infoBar:", getDapText("fileChangedSinceRun"));
            }
            return false;
        }
    }

    clearDapRunToCursor();
    var request = { documentId: String(documentId), line: position.lineNumber, started: false };
    state.dapRunToCursor = request;
    setTimeout(function() {
        if (state.dapRunToCursor === request && !request.started) {
            clearDapRunToCursor();
        }
    }, DAP_RUN_TO_CURSOR_START_DELAY);

    if (!entry || !entry.sourcePath) {
        // an untitled document: its path, and its list, come with the save
        return true;
    }

    var sourcePath = entry.sourcePath;
    client.syncSource(sourcePath).then(function(body) {
        if (!resume || state.dapRunToCursor !== request) {
            return;
        }
        // the answers come in the order of the lines sent
        var lines = collectDapSourceBreakpoints(sourcePath).map(function(breakpoint) {
            return breakpoint.line;
        });
        var answer = body && Array.isArray(body.breakpoints) && body.breakpoints[lines.indexOf(request.line)];
        if (!answer) {
            clearDapRunToCursor();
            return;
        }
        if (answer.verified === false) {
            clearDapRunToCursor();
            if (typeof notifyJava === "function") {
                notifyJava("infoBar:", getDapBreakpointMessage(answer));
            }
            return;
        }
        client.command("continue");
    });
    return true;
};

/*
 * Called immediately before a save or Save As changes the Java-side identity.
 * The following registry refresh must preserve Monaco's tracked lines instead
 * of replacing them with the store's still-old saved positions.
 */
window.SciNotesMonacoPrepareDocumentBreakpointsForSave = function(id) {
    var entry = getDapDocuments()[id];
    if (!entry) {
        return false;
    }
    captureDapBreakpointDecorationLines(id);
    entry.keepTrackedBreakpointsOnNextSync = true;
    return true;
};

/* Refresh moved lines and their pending colour after a Monaco text edit. */
window.SciNotesMonacoTrackBreakpointLines = function(id) {
    if (!captureDapBreakpointDecorationLines(id)) {
        return false;
    }
    updateDapBreakpointDecorations(id);
    return true;
};

/*
 * Hand a document's breakpoints back to Java for persistence. Used when the
 * browser, not the store, holds the authoritative lines: an untitled document
 * that has just been saved for the first time.
 */
function persistDapBreakpoints(entry) {
    if (!entry || !entry.sourcePath || typeof notifyJava !== "function") {
        return;
    }

    notifyJava("dapBreakpoints:", JSON.stringify({
        sourcePath: entry.sourcePath,
        breakpoints: (entry.breakpoints || []).map(function(breakpoint) {
            return {
                line: breakpoint.line,
                enabled: breakpoint.enabled !== false,
                condition: breakpoint.condition || "",
                hitCondition: breakpoint.hitCondition || ""
            };
        })
    }));
}

/*
 * The DAP client of one SciNotes window: the shared client of dap-client.js,
 * plus what the editor makes of it -- its breakpoints, the stopped frame, and
 * the state of the SciNotes controls.
 */
function createSciNotesDapClient(config) {
    var state = window.SciNotesMonacoState;
    var activeDocumentId = state.activeDocumentId || null;
    var listeners = [];
    state.dapDebug = state.dapDebug || {};

    var client = createDapClient({
        url: config.url,
        sessionId: config.sessionId,
        clientID: "scinotes-monaco",
        clientName: "SciNotes Monaco",
        text: getDapText,
        debug: state.dapDebug,
        beforeConfigurationDone: syncAllSources,
        onSessionState: function(sessionState) {
            // Only a stop has a location to show.
            if (sessionState !== "stopped") {
                clearDapStopped();
            }
            followDapRunToCursor(sessionState);
            followDapRunState(sessionState, !!(client && client.isDebugMode && client.isDebugMode()));
        },
        onControlState: function(values) {
            if (typeof notifyJava === "function") {
                notifyJava("dapState:", JSON.stringify(values));
            }
        }
    });
    client.updateDebug({ activeDocumentId: activeDocumentId });

    /*
     * Send the complete desired list for one source. DAP replaces the list of
     * the requested source, so this is always a full send, never a delta.
     */
    function syncSource(sourcePath) {
        if (!client.isAttached() || !sourcePath) {
            return Promise.resolve(null);
        }

        var requested = collectDapSourceBreakpoints(sourcePath);
        return client.request("setBreakpoints", {
            source: { path: sourcePath },
            breakpoints: requested
        }).then(function(body) {
            applyDapVerification(sourcePath, requested, body && body.breakpoints);
            client.updateDebug({ lastBreakpointSync: { source: sourcePath, count: requested.length } });
            return body;
        }).catch(function(error) {
            client.updateDebug({ lastError: "setBreakpoints(" + sourcePath + "): " + error.message });
            return null;
        });
    }

    /* Reinstall every known source, after attach or after a reconnect. */
    function syncAllSources() {
        return Promise.all(getDapSourcePaths().map(syncSource));
    }

    /*
     * Standard waterfall on a stop: ask for the top frame, then show it. The
     * source is not necessarily open, so Java is asked to reveal it; the
     * decoration lands when the document registers, here or later.
     */
    function followStoppedFrame(stopBody) {
        var reason = stopBody && stopBody.reason || "";
        // Only an error has a text of its own; the hover of any other stop is ours.
        var text = reason === "exception" && stopBody.text || "";
        /*
         * Ask for the whole stack, not only its top: the innermost frame can be
         * a callable Scilab knows no file for - a compiled macro, or the
         * command that started the editor - and the frame to show is then the
         * first one below it that does carry a source.
         */
        var generation = client.getRunStateGeneration();
        client.request("stackTrace", { threadId: 1, startFrame: 0, levels: 20 })
            .then(function(body) {
                if (generation !== client.getRunStateGeneration()) {
                    return;
                }
                var frames = Array.isArray(body && body.stackFrames) ? body.stackFrames : [];
                var frame = null;
                for (var i = 0; i < frames.length; i++) {
                    if (frames[i] && frames[i].source && frames[i].source.path && frames[i].line > 0) {
                        frame = frames[i];
                        break;
                    }
                }
                var sourcePath = frame && frame.source && frame.source.path;
                if (!sourcePath || !(frame.line > 0)) {
                    clearDapStopped();
                    return;
                }

                state.dapStoppedLocation = {
                    frameId: frame.id,
                    sourcePath: sourcePath,
                    line: frame.line,
                    column: frame.column > 0 ? frame.column : 0,
                    reason: reason,
                    text: text
                };
                applyDapStoppedLocation(true);
                if (typeof notifyJava === "function") {
                    notifyJava("dapStopped:", encodeURIComponent(sourcePath) + ":" + frame.line);
                }
            })
            .catch(function(error) {
                client.updateDebug({ lastError: "stackTrace: " + error.message });
                if (generation === client.getRunStateGeneration()) {
                    clearDapStopped();
                }
            });
    }

    function notifyExecutionEnded() {
        if (typeof notifyJava === "function") {
            notifyJava("dapExecutionEnded:", "");
        }
    }

    listeners.push(client.on("stopped", function(body) {
        if (body.scilabExecutionDone) {
            notifyExecutionEnded();
        } else {
            followStoppedFrame(body);
        }
    }));
    listeners.push(client.on("breakpoint", applyDapForeignBreakpoint));
    listeners.push(client.on("terminated", function() {
        clearDapForeignBreakpoints();
        notifyExecutionEnded();
    }));

    var documentSelectionListener = function(event) {
        var detail = event && event.detail || {};
        activeDocumentId = detail.id || null;
        client.updateDebug({ activeDocumentId: activeDocumentId, activeDocumentUri: detail.uri || null });
        client.publishControlState();
    };
    window.addEventListener("scinotes-monaco-document-selected", documentSelectionListener);

    return {
        start: client.start,
        request: client.request,
        command: client.command,
        execFile: client.execFile,
        getSessionState: client.getSessionState,
        getRunStateGeneration: client.getRunStateGeneration,
        syncSource: syncSource,
        syncAllSources: syncAllSources,
        getActiveDocumentId: function() {
            // The initially adopted document does not raise a selection event.
            return activeDocumentId || state.activeDocumentId || null;
        },
        on: client.on,
        dispose: function() {
            window.removeEventListener("scinotes-monaco-document-selected", documentSelectionListener);
            listeners.forEach(function(listener) {
                listener.dispose();
            });
            listeners = [];
            client.dispose();
        }
    };
}

function configureDap(config) {
    var state = window.SciNotesMonacoState;
    state.pendingDapConfig = config || null;
    if (state.dapClient) {
        state.dapClient.dispose();
        state.dapClient = null;
    }
    if (!config || !state.editor) {
        if (typeof notifyJava === "function") {
            notifyJava("dapState:", JSON.stringify({
                state: "disconnected",
                pending: "",
                canContinue: false,
                canPause: false,
                canStepOver: false,
                canStepIn: false,
                canStepOut: false,
                canDisconnect: false,
                message: ""
            }));
        }
        return;
    }
    state.dapClient = createSciNotesDapClient(config);
    state.dapClient.start();
}

window.SciNotesMonacoDebugCommand = function(command) {
    var client = window.SciNotesMonacoState.dapClient;
    return !!(client && client.command(command));
};

/*
 * Run one file through the debug session instead of handing it to the
 * interpreter from Java, debugged or not: every participant view sees it
 * start.
 */
window.SciNotesMonacoExecuteFile = function(sourcePath, echo, debug) {
    var client = window.SciNotesMonacoState.dapClient;
    return !!(client && typeof client.execFile === "function"
        && client.execFile(sourcePath, echo === true, debug === true));
};

/*
 * Register or refresh one tab's canonical source path and desired breakpoints.
 * Java owns the durable list; this only mirrors it per model and replays the
 * complete per-source list to DAP.
 */
window.SciNotesMonacoSetDocumentBreakpoints = function(id, uri, sourcePath, breakpoints) {
    if (!id) {
        return false;
    }

    var documents = getDapDocuments();
    var entry = documents[id] || { id: String(id), breakpoints: [] };
    var previousSourcePath = entry.sourcePath || "";
    var nextSourcePath = sourcePath || "";
    entry.uri = uri || entry.uri || "";
    entry.sourcePath = nextSourcePath;

    /*
     * An untitled document keeps its breakpoints in memory only: Java has
     * nothing persisted under a path it did not have yet. On its first save the
     * browser is therefore the authority, and the stored list -- necessarily
     * empty -- must not erase it. The same applies immediately after a save:
     * Monaco's tracked decoration lines are newer than the still-old store.
     * Any other push is a refresh from the durable Java state.
     */
    var promoted = !previousSourcePath && nextSourcePath
        && entry.breakpoints && entry.breakpoints.length > 0;
    /*
     * A tab that gets its first path with nothing tracked has nothing newer
     * than the store: a split or its removal opens the file again that way,
     * and keeping its empty list would erase the file's breakpoints.
     */
    var trackedAfterSave = entry.keepTrackedBreakpointsOnNextSync === true
        && (!!previousSourcePath || promoted);
    entry.keepTrackedBreakpointsOnNextSync = false;
    if (!promoted && !trackedAfterSave) {
        entry.breakpoints = normalizeDapBreakpoints(breakpoints);
    }
    documents[entry.id] = entry;
    noteDapRunVersion(entry);

    var client = window.SciNotesMonacoState.dapClient;
    if (client) {
        // After a Save As the old path has no document left, so recomputing it
        // sends the empty list that retires it from the session.
        if (previousSourcePath && previousSourcePath !== nextSourcePath) {
            client.syncSource(previousSourcePath);
        }
        client.syncSource(nextSourcePath);
    }
    /* The list was selected above; do not recapture obsolete decoration IDs. */
    updateDapBreakpointDecorations(entry.id);
    var stopped = window.SciNotesMonacoState.dapStoppedLocation;
    if (stopped && stopped.sourcePath === entry.sourcePath) {
        // Reveal only when this document was not carrying the marker yet: the
        // file opening for the stop, not a plain refresh of its list.
        var wasMarked = window.SciNotesMonacoState.dapStoppedDocumentId === entry.id;
        applyDapStoppedLocation(!wasMarked);
    } else if (stopped && previousSourcePath && stopped.sourcePath === previousSourcePath
            && !findDapDocumentBySource(previousSourcePath)) {
        /*
         * Saved under another name while stopped: Scilab still runs the file
         * it started, which no tab shows any more. Open it again, as for a
         * stop in a file that was not open.
         */
        clearDapStoppedDecorations();
        if (typeof notifyJava === "function") {
            notifyJava("dapStopped:", encodeURIComponent(stopped.sourcePath) + ":" + stopped.line);
        }
    }
    if (promoted || trackedAfterSave) {
        persistDapBreakpoints(entry);
    }
    return true;
};

/*
 * Forget a closed tab. The breakpoints stay installed in the debug session and
 * persisted by Java: closing an editor is not removing a breakpoint.
 */
window.SciNotesMonacoRemoveDocumentBreakpoints = function(id) {
    var documents = getDapDocuments();
    var entry = documents[id];
    if (!entry) {
        return false;
    }

    /*
     * Tabs on one file share a model, and a split or its removal replaces the
     * tab on the buffer with another one: marks left behind on that model
     * would stay on screen, owned by nobody.
     */
    var state = window.SciNotesMonacoState;
    var stoppedHere = state.dapStoppedDocumentId === entry.id;
    if (stoppedHere) {
        clearDapStoppedDecorations();
    }
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    var document = registry && registry[entry.id];
    var model = document && document.model;
    if (model && typeof model.deltaDecorations === "function") {
        if (entry.decorationModel === model) {
            model.deltaDecorations(entry.decorations || [], []);
        }
        model.deltaDecorations(entry.foreignDecorations || [], []);
    }
    delete documents[id];
    if (stoppedHere) {
        applyDapStoppedLocation();
    }
    return true;
};

/*
 * Java hands over the whole durable store, so the session covers every file it
 * knows and not only the open tabs. Sources are resynchronised, since a file
 * nobody shows has no document to carry its lines.
 */
window.SciNotesMonacoSetStoredBreakpoints = function(stored) {
    var state = window.SciNotesMonacoState;
    state.dapStoredBreakpoints = stored && typeof stored === "object" ? stored : {};
    if (state.dapClient && typeof state.dapClient.syncAllSources === "function") {
        state.dapClient.syncAllSources();
    }
    return true;
};

window.SciNotesMonacoConfigureDapEditor = function() {
    configureDapEditor();
};

window.SciNotesMonacoDapDebug = function() {
    var state = window.SciNotesMonacoState;
    if (!state) {
        return {};
    }

    var documents = getDapDocuments();
    var debugValues = state.dapDebug || {};
    var snapshot = {};
    Object.keys(debugValues).forEach(function(name) {
        snapshot[name] = debugValues[name];
    });
    var registry = typeof getDocumentRegistry === "function" ? getDocumentRegistry() : null;
    snapshot.documents = Object.keys(documents).map(function(id) {
        var document = registry && registry[id];
        return {
            id: id,
            uri: documents[id].uri,
            sourcePath: documents[id].sourcePath,
            breakpoints: documents[id].breakpoints,
            modelFound: !!(document && document.model),
            decorationCount: (documents[id].decorations || []).length
        };
    });
    snapshot.documentRegistryReachable = !!registry;
    snapshot.sourcePaths = getDapSourcePaths();
    snapshot.activeDocumentId = state.activeDocumentId;
    // A line another client of the session installed is drawn hollow and is
    // not ours to remove: the first thing to check when a dot will not go.
    snapshot.foreignBreakpoints = state.dapForeignBreakpoints || {};
    snapshot.storedBreakpoints = state.dapStoredBreakpoints || {};
    snapshot.stopped = state.dapStoppedLocation;
    snapshot.stoppedDocumentId = state.dapStoppedDocumentId;
    return snapshot;
};
