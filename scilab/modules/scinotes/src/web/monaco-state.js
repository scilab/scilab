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
 * This file owns the shared mutable state for the browser-side Monaco
 * integration. Other files read and update this object instead of keeping
 * duplicated editor, shortcut, anchor, and undo/redo state.
 */

window.SciNotesMonacoState = {
    editor: null,
    // Second editor VIEW mounted in #editor-secondary while a Monaco tab is
    // split (see monaco-split.js). Always bound to the same model as
    // `editor`'s current one; never owns a model of its own.
    secondaryEditor: null,
    // True while the split's second view is the one the user last worked
    // in: view-level requests (shortcuts, selection, find, ...) go to it,
    // see getViewEditor() in monaco-split.js.
    secondaryViewActive: false,
    // Registry id of the document the second view is mounted on, so leaving
    // the tab can store that view's state on it, see SciNotesMonacoDisableSplit.
    splitDocumentId: null,
    // Last bounds given to SciNotesMonacoLayoutSplit, reused to drag the
    // separator between the two views.
    splitLayout: null,
    // Set while that separator is being dragged, see configureSplitHandle().
    splitDrag: null,
    documents: {},
    activeDocumentId: null,
    pendingValue: "",
    pendingReadOnly: false,
    pendingOptions: null,
    appliedThemeSignature: null,
    pendingShortcutConfig: null,
    pendingLspConfig: null,
    lspClient: null,
    contextMenuDocumentListener: null,
    contextMenuMouseDownListener: null,
    contextMenuMouseDownNode: null,
    pendingDapConfig: null,
    dapClient: null,
    dapDocuments: {},
    dapGutterListener: null,
    dapSelectionListener: null,
    dapHoverListener: null,
    dapMouseLeaveListener: null,
    dapHoverDecorations: [],
    dapHoverLine: 0,
    // The ghost of the split's second view, and the listeners of its gutter
    // (see setViewDecorations() for the "secondary" naming).
    secondaryDapHoverDecorations: [],
    secondaryDapHoverLine: 0,
    secondaryDapGutterListeners: null,
    dapStoppedLocation: null,
    dapStoppedDocumentId: null,
    dapStoppedDecorations: [],
    anchorLines: {},
    lineNumbersBase: undefined,
    shortcutKeydownListener: null,
    shortcutKeydownTarget: null,
    anchorDecorations: [],
    localFunctionDecorations: [],
    // The same two, in the split's second view (see setViewDecorations()).
    secondaryAnchorDecorations: [],
    secondaryLocalFunctionDecorations: [],
    // Last anchors Java sent, to show them in a second view mounted later.
    lastAnchors: [],
    undoDepth: 0,
    redoDepth: 0,
    savedAlternativeVersionId: null,
    pendingClean: true,
    documentRevision: 0,
    applyingJavaValue: false
};
