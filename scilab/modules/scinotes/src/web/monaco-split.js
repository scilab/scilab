/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*
 * There is exactly one native browser per SciNotes window (see
 * MonacoEditorPane's SharedBrowser on the Java side), so splitting a Monaco
 * tab cannot give the second half its own Monaco document the normal way.
 * Instead the "other" half mounts a second EDITOR VIEW, bound to the same
 * model as the primary editor's current one -- exactly how the classic
 * Swing editor shows two JTextPane views of one Document on split. Monaco
 * keeps a shared model's content and undo stack in sync across every editor
 * instance showing it on its own; this file only owns the second DOM
 * container, its Monaco instance, its bounds, and the separator the user
 * drags between the two views.
 *
 * Java only ever talks to the split's primary pane. What acts on a VIEW
 * rather than on the document -- shortcuts, the selection Java mirrors, find,
 * the context menu -- must follow the view the user last worked in, the way
 * the Swing editor's getTextPane() follows the focused JTextPane: that is
 * getViewEditor().
 */

/**
 * @return the editor view that view-level requests apply to: the split's
 *         second view when the user last worked in it, the primary one
 *         otherwise
 */
function getViewEditor() {
    var state = window.SciNotesMonacoState;
    if (state.secondaryViewActive && state.secondaryEditor) {
        return state.secondaryEditor;
    }
    return state.editor;
}

/**
 * Mount (or rebind) the secondary editor view on the model currently held by
 * the document `primaryDocumentId` names, and reveal its container.
 * @param {string} primaryDocumentId id of the split's primary document
 */
window.SciNotesMonacoEnableSplit = function(primaryDocumentId) {
    var state = window.SciNotesMonacoState;
    var registry = getDocumentRegistry();
    var entry = registry[primaryDocumentId];
    var container = document.getElementById("editor-secondary");
    if (!entry || !state.editor || !container) {
        return;
    }

    if (!state.secondaryEditor) {
        state.secondaryEditor = monaco.editor.create(container, {
            model: entry.model,
            automaticLayout: true,
            glyphMargin: false,
            lineDecorationsWidth: 4,
            lineNumbersMinChars: 3,
            contextmenu: false,
            selectionHighlightMultiline: true,
            wordBasedSuggestions: "off",
            minimap: { enabled: false },
            hover: { delay: 500 },
            padding: { top: 2, bottom: 2 },
            scrollBeyondLastLine: false,
            readOnly: !!entry.readOnly,
            fontSize: 13,
            autoIndent: "full",
            renderWhitespace: "none",
            "semanticHighlighting.enabled": true
        });
        // Same focus and selection reports as the primary editor
        // (monaco-bootstrap.js and configureFocusHandling()): Java hands the
        // keyboard over on focus, and mirrors the selection of the view in
        // use for its own actions (evaluate selection, comment, ...).
        state.secondaryEditor.onDidFocusEditorText(function() {
            state.secondaryViewActive = true;
            notifyJava("focus:", "in");
            notifySelection();
        });
        state.secondaryEditor.getDomNode().addEventListener("mousedown", function() {
            state.secondaryViewActive = true;
            notifyJava("focus:", "in");
        }, true);
        state.secondaryEditor.onDidChangeCursorSelection(function() {
            if (state.secondaryViewActive) {
                notifySelection();
            }
        });
        attachSecondaryViewFeatures(state.secondaryEditor);
    } else if (state.secondaryEditor.getModel() !== entry.model) {
        state.secondaryEditor.setModel(entry.model);
    }

    // Coming back to a split tab: the second view was disposed when another
    // tab was selected, so give it back its caret and scroll, and the
    // keyboard if it had it -- Java focuses getViewEditor() right after.
    var restoredViewState = null;
    var refocusSecondary = false;
    if (state.splitDocumentId !== entry.id) {
        state.splitDocumentId = entry.id;
        restoredViewState = entry.secondaryViewState;
        state.secondaryViewActive = !!entry.secondaryViewActive;
        refocusSecondary = state.secondaryViewActive;
    }

    // The create() options above are only the static ones; the user's
    // display preferences (word wrap, current-line highlight, font, line
    // numbers, ...) live in state.pendingOptions and are otherwise only
    // pushed to state.editor. Without this the secondary view keeps Monaco
    // defaults -- a stray horizontal scrollbar, a highlighted caret line the
    // primary does not have, etc.
    if (typeof applyEditorOptions === "function" && state.pendingOptions) {
        applyEditorOptions(state.pendingOptions);
    }

    // A new second view starts with plain numbering and no decorations. When
    // the primary already shows this document (splitting the current tab),
    // nothing else will push them; when it does not (coming back to a split
    // tab), selecting the document right after re-applies them to both views.
    if (state.editor.getModel() === entry.model) {
        if (state.astLocalLineNumbersActive && entry.astLineNumbers) {
            applyStoredAstLineNumbers(entry.astLineNumbers);
        }
        window.SciNotesMonacoSetAnchors(state.lastAnchors);
    }

    container.classList.remove("scinotes-split-hidden");

    // Restored only once the container is displayed: the view was created
    // hidden, with no size, and a scroll position applied then is lost.
    if (restoredViewState) {
        state.secondaryEditor.layout();
        state.secondaryEditor.restoreViewState(restoredViewState);
    }

    if (refocusSecondary) {
        // The page does not have the native focus yet, and its focused
        // element is still the primary view, which kept it while the other
        // tab was shown. When Java's native focus request lands, Chromium
        // refocuses that element and the primary view would take the
        // keyboard back. Make the second view that element now -- once its
        // container is displayed, a hidden element cannot take the focus.
        state.secondaryEditor.focus();
    }
};

/**
 * Drop the secondary editor view and hide its container. The model itself
 * is untouched -- it is owned by the document registry, not by this view.
 *
 * @param {boolean} forget true when the tab is no longer split; otherwise
 *        another tab is being selected, and the second view's caret, scroll
 *        and focus are kept on the primary document's registry entry for
 *        SciNotesMonacoEnableSplit to restore
 */
window.SciNotesMonacoDisableSplit = function(forget) {
    var state = window.SciNotesMonacoState;
    var entry = state.splitDocumentId ? getDocumentRegistry()[state.splitDocumentId] : null;
    if (entry) {
        entry.secondaryViewState = !forget && state.secondaryEditor
            ? state.secondaryEditor.saveViewState() : null;
        entry.secondaryViewActive = !forget && state.secondaryViewActive;
    }
    state.splitDocumentId = null;
    if (state.secondaryEditor) {
        if (state.lspClient && state.lspClient.detachEditorView) {
            state.lspClient.detachEditorView(state.secondaryEditor);
        }
        state.secondaryEditor.dispose();
        state.secondaryEditor = null;
    }
    state.secondaryViewActive = false;
    state.secondaryAnchorDecorations = [];
    state.secondaryLocalFunctionDecorations = [];
    state.splitLayout = null;

    // SciNotesMonacoLayoutSplit pinned #editor to a small pixel-sized box
    // matching its half of the split; clear that inline style so the
    // #editor CSS rule (100% x 100%) fills the whole overlay again now that
    // Java is about to shrink the overlay back down to a single target.
    var primary = document.getElementById("editor");
    if (primary) {
        primary.style.left = "";
        primary.style.top = "";
        primary.style.width = "";
        primary.style.height = "";
    }

    var container = document.getElementById("editor-secondary");
    if (container) {
        container.classList.remove("scinotes-split-col", "scinotes-split-row");
        container.classList.add("scinotes-split-hidden");
    }

    var handle = document.getElementById("split-handle");
    if (handle) {
        handle.classList.remove("scinotes-split-col", "scinotes-split-row");
        handle.classList.add("scinotes-split-hidden");
    }
};

/**
 * Give the second view the per-editor behaviours the primary one gets at
 * startup (monaco-bootstrap.js): Ctrl+click on URLs, the persistent LaTeX
 * preview, and the LSP go-to-definition pointer. Everything else (hover,
 * completion, definition itself, file drop) is registered page- or
 * language-wide and already reaches both views.
 */
function attachSecondaryViewFeatures(editor) {
    var state = window.SciNotesMonacoState;
    if (typeof attachOpenUrlHandling === "function") {
        attachOpenUrlHandling(editor);
    }
    if (typeof configurePersistentLatexPreview === "function") {
        configurePersistentLatexPreview(editor);
    }
    if (state.lspClient && state.lspClient.attachEditorView) {
        state.lspClient.attachEditorView(editor);
    }
}

/**
 * Follow a model swap in the secondary view. A model's URI is immutable, so
 * renaming a document (first save of an Untitled buffer, Save As, the LSP
 * bootstrap URI fixup) means creating a replacement model and disposing the
 * old one. The secondary view holds its own reference to that model and
 * would otherwise keep rendering a disposed one -- a blank white half.
 *
 * @param oldModel the model being replaced or disposed
 * @param newModel its replacement, or null when the document is going away
 */
function syncSplitModelReplacement(oldModel, newModel) {
    var state = window.SciNotesMonacoState;
    if (!state.secondaryEditor || state.secondaryEditor.getModel() !== oldModel) {
        return;
    }

    var viewState = state.secondaryEditor.saveViewState();
    state.secondaryEditor.setModel(newModel);
    if (newModel && viewState) {
        // The secondary view scrolls and moves its caret independently of the
        // primary one; a rename must not reset it to the top of the file.
        state.secondaryEditor.restoreViewState(viewState);
    }
}

/**
 * Position #editor and #editor-secondary to match the two native Swing
 * placeholders the single browser window is currently stretched across.
 * Called from Java (SharedBrowser.activateSplit()) on every relayout of the
 * split (window resize, tab strip settling, divider moved, ...), and locally
 * while the separator is dragged. automaticLayout on both editor instances
 * picks the resulting container resize up on its own.
 */
window.SciNotesMonacoLayoutSplit = function(px, py, pw, ph, sx, sy, sw, sh) {
    var state = window.SciNotesMonacoState;
    if (state.splitDrag) {
        // Java relays the layout it had before the drag started; the drag
        // owns the bounds until it is committed.
        return;
    }
    applySplitLayout(px, py, pw, ph, sx, sy, sw, sh);
};

function applySplitLayout(px, py, pw, ph, sx, sy, sw, sh) {
    var primary = document.getElementById("editor");
    var secondary = document.getElementById("editor-secondary");
    if (!primary || !secondary) {
        return;
    }
    primary.style.left = px + "px";
    primary.style.top = py + "px";
    primary.style.width = pw + "px";
    primary.style.height = ph + "px";
    secondary.style.left = sx + "px";
    secondary.style.top = sy + "px";
    secondary.style.width = sw + "px";
    secondary.style.height = sh + "px";

    // Orientation drives which edge of the secondary view gets the separator
    // border (see monaco.html): to its left when the split is by column,
    // above it when the split is by row.
    var col = sx > px + 1;
    var row = sy > py + 1;
    secondary.classList.toggle("scinotes-split-col", col && !row);
    secondary.classList.toggle("scinotes-split-row", row && !col);

    window.SciNotesMonacoState.splitLayout = {
        px: px, py: py, pw: pw, ph: ph, sx: sx, sy: sy, sw: sw, sh: sh,
        col: col && !row, row: row && !col
    };
    placeSplitHandle();
}

/*
 * The native browser covers both halves of a split, so the JSplitPane
 * divider underneath can never be grabbed (SciNotes.splitTab() gives it no
 * size). The separator is dragged here instead: the views follow the pointer
 * locally, and the final size of the primary view goes to Java on release,
 * which moves the real divider -- its relayout then keeps that size across
 * window resizes and tab switches.
 */
var SPLIT_HANDLE_THICKNESS = 6;
var SPLIT_MIN_VIEW_SIZE = 40;

function placeSplitHandle() {
    var handle = document.getElementById("split-handle");
    var layout = window.SciNotesMonacoState.splitLayout;
    if (!handle) {
        return;
    }
    if (!layout || (!layout.col && !layout.row)) {
        handle.classList.add("scinotes-split-hidden");
        return;
    }

    // Centered on the 2px border the secondary view draws on its edge.
    var offset = Math.floor((SPLIT_HANDLE_THICKNESS - 2) / 2);
    if (layout.col) {
        handle.style.left = (layout.sx - offset) + "px";
        handle.style.top = layout.sy + "px";
        handle.style.width = SPLIT_HANDLE_THICKNESS + "px";
        handle.style.height = layout.sh + "px";
    } else {
        handle.style.left = layout.sx + "px";
        handle.style.top = (layout.sy - offset) + "px";
        handle.style.width = layout.sw + "px";
        handle.style.height = SPLIT_HANDLE_THICKNESS + "px";
    }
    handle.classList.toggle("scinotes-split-col", layout.col);
    handle.classList.toggle("scinotes-split-row", layout.row);
    handle.classList.remove("scinotes-split-hidden");
}

function configureSplitHandle() {
    var handle = document.getElementById("split-handle");
    if (!handle) {
        return;
    }
    var state = window.SciNotesMonacoState;

    handle.addEventListener("pointerdown", function(event) {
        var layout = state.splitLayout;
        if (event.button !== 0 || !layout) {
            return;
        }
        event.preventDefault();
        event.stopPropagation();
        handle.setPointerCapture(event.pointerId);
        state.splitDrag = {
            layout: layout,
            origin: layout.col ? event.clientX : event.clientY,
            primarySize: layout.col ? layout.pw : layout.ph
        };
    });

    handle.addEventListener("pointermove", function(event) {
        var drag = state.splitDrag;
        if (!drag) {
            return;
        }
        var l = drag.layout;
        var total = l.col ? (l.sx + l.sw - l.px) : (l.sy + l.sh - l.py);
        var pointer = l.col ? event.clientX : event.clientY;
        var size = drag.primarySize + pointer - drag.origin;
        size = Math.max(SPLIT_MIN_VIEW_SIZE, Math.min(total - SPLIT_MIN_VIEW_SIZE, size));
        drag.size = size;
        if (l.col) {
            applySplitLayout(l.px, l.py, size, l.ph, l.px + size, l.sy, total - size, l.sh);
        } else {
            applySplitLayout(l.px, l.py, l.pw, size, l.sx, l.py + size, l.sw, total - size);
        }
    });

    var endDrag = function(event) {
        var drag = state.splitDrag;
        if (!drag) {
            return;
        }
        state.splitDrag = null;
        if (handle.hasPointerCapture(event.pointerId)) {
            handle.releasePointerCapture(event.pointerId);
        }
        if (typeof drag.size === "number" && drag.size !== drag.primarySize) {
            notifyJava("splitSize:", String(Math.round(drag.size)));
        }
        var editor = getViewEditor();
        if (editor) {
            editor.focus();
        }
    };
    handle.addEventListener("pointerup", endDrag);
    handle.addEventListener("pointercancel", endDrag);
}

configureSplitHandle();
