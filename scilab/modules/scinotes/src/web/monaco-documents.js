/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*
 * Owns the Monaco text models and view states used by the single editor in a
 * SciNotes window. Document identity is independent from the browser, which
 * also gives the LSP and future DAP clients a stable URI when tabs are switched.
 */


function getDocumentRegistry() {
    var state = window.SciNotesMonacoState;
    state.documents = state.documents || {};
    return state.documents;
}

function getActiveDocumentEntry() {
    var state = window.SciNotesMonacoState;
    return state.activeDocumentId && getDocumentRegistry()[state.activeDocumentId] || null;
}
/*
 * Monaco keeps LF internally so that the offsets exchanged with the Java
 * replica mean the same thing on both sides. setValue() rebuilds the buffer
 * with the model's creation EOL, which is CRLF on Windows, so the end of line
 * has to be forced again after every full replacement.
 */
function useLfEndOfLine(model) {
    if (model) {
        model.setEOL(monaco.editor.EndOfLineSequence.LF);
    }
}


/*
 * Monaco keys its models by URI, so all the tabs showing one file share a
 * single model: they are several views of the same buffer. Anything derived
 * from a model therefore belongs to all of them, and the model outlives any
 * one of them.
 */
function getDocumentEntriesForModel(model) {
    var registry = getDocumentRegistry();
    return Object.keys(registry).map(function(id) {
        return registry[id];
    }).filter(function(entry) {
        return entry.model === model;
    });
}

/*
 * Only an unnamed buffer is Scilab by construction; a named file takes the
 * language Monaco derives from its path, so opening a .xml in SciNotes does
 * not color it, nor hand it to the Scilab LSP, as Scilab code.
 */
function languageForUri(uri) {
    return uri && uri.scheme === "untitled" ? "scilab" : undefined;
}

function isModelStillReferenced(model) {
    return getDocumentEntriesForModel(model).length > 0;
}

/*
 * A tab joining a buffer another tab already shows adopts what that model
 * carries: the local numbering already derived from it, which Monaco will not
 * recompute for an unchanged model, and the buffer itself, which the tab's
 * Java replica does not hold yet.
 */
function adoptSharedModel(entry, replicaText) {
    var siblings = getDocumentEntriesForModel(entry.model);
    var sibling = null;
    for (var i = 0; i < siblings.length; i++) {
        if (siblings[i] !== entry) {
            sibling = siblings[i];
            if (sibling.astLineNumbers) {
                entry.astLineNumbers = sibling.astLineNumbers;
                break;
            }
        }
    }

    /*
     * The undo history and the version last saved to disk belong to the
     * buffer, not to the tab. A tab joining a buffer modified elsewhere took
     * the current version as its own saved reference, so it showed clean
     * while the file was modified, and modified once an undo had brought the
     * buffer back to what is on disk.
     */
    if (sibling) {
        entry.savedAlternativeVersionId = sibling.savedAlternativeVersionId;
        entry.clean = sibling.clean;
    }

    var buffer = entry.model.getValue();
    var replica = replicaText || "";
    if (typeof notifyJavaForDocument !== "function") {
        return;
    }

    if (buffer !== replica) {
        var expected = entry.revision;
        entry.revision = expected + 1;
        notifyJavaForDocument(entry.id, "edit:",
            expected + ":" + entry.revision + ":0:" + replica.length + ":" + buffer);
    }
    notifyJavaForDocument(entry.id, "modified:", String(!entry.clean));
}


function saveActiveDocumentState() {
    var state = window.SciNotesMonacoState;
    var entry = getActiveDocumentEntry();
    if (!entry || !state.editor || state.editor.getModel() !== entry.model) {
        return;
    }

    entry.viewState = state.editor.saveViewState();
    entry.revision = state.documentRevision;
    entry.savedAlternativeVersionId = state.savedAlternativeVersionId;
    entry.clean = state.pendingClean;
    entry.readOnly = state.pendingReadOnly;
    entry.undoDepth = state.undoDepth;
    entry.redoDepth = state.redoDepth;
}

/*
 * The sticky lines (the enclosing function shown at the top) are recomputed
 * asynchronously after a model change: the previous document's stayed on
 * screen meanwhile. Monaco has no API for it; _resetState() empties the widget,
 * and nothing happens should that internal method disappear.
 */
function clearStickyScroll(editor) {
    var sticky = editor && editor.getContribution("store.contrib.stickyScrollController");
    if (sticky && typeof sticky._resetState === "function") {
        sticky._resetState();
    }
}

function selectDocumentEntry(entry, presentation) {
    var state = window.SciNotesMonacoState;
    if (!entry || !state.editor) {
        return false;
    }

    // Focusing the editor can make Java select the tab that is already active.
    // Restoring that model's view state while Monaco is handling the input
    // cancels the pending mouse/keyboard operation (including a right click).
    if (state.activeDocumentId === entry.id && state.editor.getModel() === entry.model) {
        return true;
    }

    saveActiveDocumentState();
    state.activeDocumentId = entry.id;
    state.documentRevision = entry.revision;
    state.savedAlternativeVersionId = entry.savedAlternativeVersionId;
    state.pendingClean = entry.clean;
    state.pendingReadOnly = entry.readOnly;
    state.undoDepth = entry.undoDepth;
    state.redoDepth = entry.redoDepth;
    state.astLocalLineNumbersActive = false;
    state.applyingJavaValue = true;
    state.editor.setModel(entry.model);
    state.applyingJavaValue = false;
    clearStickyScroll(state.editor);

    presentation = presentation || {};
    if (presentation.options && typeof applyEditorOptions === "function") {
        state.pendingOptions = presentation.options;
        applyEditorOptions(presentation.options);
    }
    state.editor.updateOptions({ readOnly: entry.readOnly });
    if (entry.astLineNumbers && presentation.options
            && presentation.options.lineNumbers
            && presentation.options.lineNumbers.mode === "local"
            && typeof window.SciNotesMonacoApplyStoredAstLineNumbers === "function") {
        window.SciNotesMonacoApplyStoredAstLineNumbers(entry.astLineNumbers);
    }
    if (Array.isArray(presentation.anchors)
            && typeof window.SciNotesMonacoSetAnchors === "function") {
        window.SciNotesMonacoSetAnchors(presentation.anchors);
    }
    if (entry.viewState) {
        state.editor.restoreViewState(entry.viewState);
    }
    state.editor.layout();
    notifySelection();
    notifyUndoStateSoon();

    window.dispatchEvent(new CustomEvent("scinotes-monaco-document-selected", {
        detail: { id: entry.id, uri: entry.uri }
    }));
    return true;
}

window.SciNotesMonacoRegisterInitialDocument = function() {
    var state = window.SciNotesMonacoState;
    if (!state.editor || state.activeDocumentId) {
        return;
    }

    var model = state.editor.getModel();
    if (!model) {
        return;
    }
    installDocumentModel(model);
    var entry = {
        id: "initial",
        uri: model.uri.toString(),
        model: model,
        viewState: null,
        revision: state.documentRevision,
        savedAlternativeVersionId: state.savedAlternativeVersionId,
        clean: state.pendingClean,
        readOnly: state.pendingReadOnly,
        undoDepth: state.undoDepth,
        redoDepth: state.redoDepth,
        astLineNumbers: null
    };
    getDocumentRegistry()[entry.id] = entry;
    state.activeDocumentId = entry.id;
};

window.SciNotesMonacoAdoptInitialDocument = function(id, uri, revision, clean, readOnly) {
    var state = window.SciNotesMonacoState;
    var registry = getDocumentRegistry();
    var entry = getActiveDocumentEntry();
    if (!entry || entry.id !== "initial" || !id) {
        return false;
    }

    delete registry.initial;
    entry.id = String(id);
    /*
     * The page created its editor without a model, so the buffer shown here
     * carries an "inmemory:" URI. Take the document's own URI below, through
     * the rename path: everything keyed by model URI -- tabs of this window
     * sharing one buffer, the LSP, the other windows -- must see the file,
     * not the page's anonymous first model.
     */
    var initialUri = entry.model.uri.toString();
    entry.uri = initialUri;
    entry.revision = Number.isFinite(revision) ? revision : entry.revision;
    entry.clean = clean !== false;
    entry.readOnly = !!readOnly;
    entry.savedAlternativeVersionId = entry.clean
        ? entry.model.getAlternativeVersionId() : null;
    entry.undoDepth = state.undoDepth;
    entry.redoDepth = state.redoDepth;
    registry[entry.id] = entry;
    state.activeDocumentId = entry.id;
    state.documentRevision = entry.revision;
    state.pendingClean = entry.clean;
    state.pendingReadOnly = entry.readOnly;
    state.savedAlternativeVersionId = entry.savedAlternativeVersionId;
    if (uri && uri !== initialUri) {
        window.SciNotesMonacoRenameDocument(entry.id, uri);
    }
    return true;
};

window.SciNotesMonacoCreateDocument = function(id, uri, value, revision, clean, readOnly) {
    if (!id || !uri || typeof monaco === "undefined") {
        return false;
    }

    var registry = getDocumentRegistry();
    if (registry[id]) {
        return selectDocumentEntry(registry[id]);
    }

    var parsedUri = monaco.Uri.parse(uri);
    var joined = monaco.editor.getModel(parsedUri);
    var model = joined
        || monaco.editor.createModel(value || "", languageForUri(parsedUri), parsedUri);
    useLfEndOfLine(model);
    installDocumentModel(model);
    var entry = {
        id: String(id),
        uri: uri,
        model: model,
        viewState: null,
        revision: Number.isFinite(revision) ? revision : 0,
        savedAlternativeVersionId: clean === false ? null : model.getAlternativeVersionId(),
        clean: clean !== false,
        readOnly: !!readOnly,
        undoDepth: 0,
        redoDepth: 0,
        astLineNumbers: null
    };
    registry[entry.id] = entry;
    if (joined) {
        adoptSharedModel(entry, value);
    }
    return true;
};

window.SciNotesMonacoSelectDocument = function(id, presentation) {
    return selectDocumentEntry(getDocumentRegistry()[id], presentation);
};

window.SciNotesMonacoApplyDocumentEdit = function(id, expectedRevision, newRevision,
        offset, length, text) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    if (!entry || entry.revision !== expectedRevision) {
        return false;
    }

    /*
     * An edit made by Java in this tab's replica is a step of the undo
     * history like any other: applied outside of it, it would shift the
     * positions the history stores. The other tabs on this model, and the
     * other windows showing the file, still have to receive it.
     */
    var model = entry.model;
    var start = model.getPositionAt(offset);
    var end = model.getPositionAt(offset + length);
    // Inside a group (SciNotesMonacoBeginEditGroup), the edits share one
    // undo step: the group closes the step itself
    var grouped = entry.javaEditGroup > 0;
    entry.revision = newRevision;
    model.__scinotesJavaOrigin = entry.id;
    try {
        if (!grouped) {
            model.pushStackElement();
        }
        // Stripping the indentation of a blank line here would be an edit
        // the Java replica of this tab never receives.
        model._trimAutoWhitespaceLines = null;
        model.pushEditOperations(null, [{
            range: new monaco.Range(start.lineNumber, start.column, end.lineNumber, end.column),
            text: text || "",
            forceMoveMarkers: true
        }], function() { return null; });
        if (!grouped) {
            model.pushStackElement();
        }
    } finally {
        model.__scinotesJavaOrigin = null;
    }
    entry.astLineNumbers = null;
    if (state.activeDocumentId === entry.id) {
        state.documentRevision = newRevision;
    }
    return true;
};

/*
 * A group of Java edits undone in one step: an edit of the assistant is a
 * removal plus an insertion (or several replacements), which must not be
 * undone half by half. Groups may nest; the outermost one closes the step.
 */
window.SciNotesMonacoBeginEditGroup = function(id) {
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return;
    }
    if (!entry.javaEditGroup) {
        entry.model.pushStackElement();
    }
    entry.javaEditGroup = (entry.javaEditGroup || 0) + 1;
};

window.SciNotesMonacoEndEditGroup = function(id) {
    var entry = getDocumentRegistry()[id];
    if (!entry || !entry.javaEditGroup) {
        return;
    }
    entry.javaEditGroup--;
    if (!entry.javaEditGroup) {
        entry.model.pushStackElement();
    }
};

window.SciNotesMonacoSetDocumentValue = function(id, value, revision, clean, readOnly) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return false;
    }

    /*
     * Replacing the buffer throws its undo history away, so only do it when
     * the text really differs. Java pushes the whole document again on paths
     * where it did not change -- splitting a tab rebuilds its panes around
     * the same document -- and that used to cost the history.
     */
    var text = value || "";
    if (entry.model.getValue() !== text) {
        state.applyingJavaValue = true;
        entry.model.setValue(text);
        useLfEndOfLine(entry.model);
        state.applyingJavaValue = false;
        entry.astLineNumbers = null;
        entry.clean = clean !== false;
        // A fresh buffer: this text is the saved one when Java says the
        // document is clean, and nothing in it is saved otherwise.
        entry.savedAlternativeVersionId = entry.clean
            ? entry.model.getAlternativeVersionId() : null;
    } else {
        /*
         * The buffer did not change, so neither did the version of it that
         * is on disk. Recomputing it here would tell a tab that its modified
         * text is the saved one, or -- for a document Java calls modified --
         * that no version of it is saved at all, which no undo can then
         * clear. A tab already on this buffer knows; this entry may be new,
         * as when a tab is split.
         */
        var sibling = null;
        getDocumentEntriesForModel(entry.model).forEach(function(other) {
            if (other !== entry && sibling === null) {
                sibling = other;
            }
        });
        if (sibling) {
            entry.savedAlternativeVersionId = sibling.savedAlternativeVersionId;
            entry.clean = sibling.clean;
        } else {
            entry.clean = entry.savedAlternativeVersionId !== null
                && entry.model.getAlternativeVersionId() === entry.savedAlternativeVersionId;
        }
    }
    entry.revision = Number.isFinite(revision) ? revision : entry.revision;
    entry.readOnly = !!readOnly;
    entry.undoDepth = 0;
    entry.redoDepth = 0;
    if (state.activeDocumentId === entry.id) {
        state.documentRevision = entry.revision;
        state.pendingClean = entry.clean;
        state.pendingReadOnly = entry.readOnly;
        state.savedAlternativeVersionId = entry.savedAlternativeVersionId;
        state.editor.updateOptions({ readOnly: entry.readOnly });
        resetUndoRedoState();
        notifyModifiedState();
        notifyUndoStateSoon();
    }
    return true;
};

window.SciNotesMonacoMarkDocumentSaved = function(id, digest) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    /*
     * The buffer is named by its contents: a tab can be one edit behind the
     * page while the file was written from another tab on the same buffer,
     * and a revision number would then reject a version that is the saved
     * one. What matters is whether Monaco shows what was written.
     */
    var saved = !!entry && (typeof digest !== "string"
                            || digest === SciNotesSharedHistory.digest(entry.model));
    if (!saved) {
        return false;
    }
    entry.clean = true;
    entry.savedAlternativeVersionId = entry.model.getAlternativeVersionId();
    if (state.activeDocumentId === entry.id) {
        state.pendingClean = true;
        state.savedAlternativeVersionId = entry.savedAlternativeVersionId;
        notifyModifiedState();
    } else {
        /*
         * A tab in the background reports it too: an edit Java has just
         * made in its replica (a tab moved from another window gets its
         * text that way) sent "modified" first, and would win otherwise.
         */
        notifyJavaForDocument(entry.id, "modified:", "false");
    }
    return true;
};

window.SciNotesMonacoSetDocumentReadOnly = function(id, readOnly) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return false;
    }
    entry.readOnly = !!readOnly;
    if (state.activeDocumentId === entry.id) {
        state.pendingReadOnly = entry.readOnly;
        state.editor.updateOptions({ readOnly: entry.readOnly });
    }
    return true;
};

window.SciNotesMonacoRenameDocument = function(id, uri) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    if (!entry || !uri || entry.uri === uri) {
        return !!entry;
    }

    var oldModel = entry.model;
    var replicaText = oldModel.getValue();
    /*
     * A tab dragged into a window that already shows this file is created
     * unnamed, then renamed: the name it takes can already have a model, and
     * createModel would reject an URI Monaco has handed out. Join it instead.
     */
    var parsedUri = monaco.Uri.parse(uri);
    var joined = monaco.editor.getModel(parsedUri);
    var replacement = joined
        || monaco.editor.createModel(replicaText, languageForUri(parsedUri), parsedUri);
    useLfEndOfLine(replacement);
    installDocumentModel(replacement);
    entry.model = replacement;
    entry.uri = uri;
    entry.astLineNumbers = null;
    entry.savedAlternativeVersionId = entry.clean
        ? replacement.getAlternativeVersionId() : null;
    if (state.activeDocumentId === entry.id) {
        state.applyingJavaValue = true;
        state.editor.setModel(replacement);
        state.applyingJavaValue = false;
        state.savedAlternativeVersionId = entry.savedAlternativeVersionId;
        if (entry.viewState) {
            state.editor.restoreViewState(entry.viewState);
        }
    }
    if (typeof syncSplitModelReplacement === "function") {
        syncSplitModelReplacement(oldModel, replacement);
    }
    if (!isModelStillReferenced(oldModel)) {
        oldModel.dispose();
    }
    if (joined) {
        adoptSharedModel(entry, replicaText);
    }
    return true;
};

window.SciNotesMonacoCloseDocument = function(id) {
    var state = window.SciNotesMonacoState;
    var registry = getDocumentRegistry();
    var entry = registry[id];
    if (!entry) {
        return false;
    }

    if (state.activeDocumentId === entry.id) {
        saveActiveDocumentState();
        state.activeDocumentId = null;
    }
    delete registry[id];
    if (state.editor.getModel() === entry.model) {
        /*
         * Another tab on the same buffer first: the editor keeps showing this
         * file. Falling back to whatever document came first in the registry
         * put another file under the tab -- what happens when a split is
         * undone, since the pane that replaces the split is on this buffer.
         */
        var successor = null;
        Object.keys(registry).forEach(function(other) {
            if (successor === null || (registry[other].model === entry.model
                                       && successor.model !== entry.model)) {
                successor = registry[other];
            }
        });
        if (successor) {
            /*
             * Java re-selects the tab it makes current, but that call returns
             * early when it lands on the one picked here: the display options
             * have to be reapplied now, or the local line numbers computed for
             * the closed document stay on screen.
             */
            selectDocumentEntry(successor, { options: state.pendingOptions });
        } else {
            state.editor.setModel(null);
        }
    }
    // Only the last view of a buffer disposes it; the other tabs are still
    // showing it.
    if (!isModelStillReferenced(entry.model)) {
        if (typeof syncSplitModelReplacement === "function") {
            syncSplitModelReplacement(entry.model, null);
        }
        entry.model.dispose();
    }
    return true;
};

window.SciNotesMonacoDocumentDebug = function() {
    var state = window.SciNotesMonacoState;
    return {
        activeDocumentId: state.activeDocumentId,
        documents: Object.keys(getDocumentRegistry()).map(function(id) {
            var entry = getDocumentRegistry()[id];
            return { id: id, uri: entry.uri, revision: entry.revision };
        })
    };
};
