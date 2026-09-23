/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*
 * Keeps the undo history of one file identical in every SciNotes window that
 * shows it. Each window is its own page with its own Monaco model, and Monaco
 * cannot export an undo step, so the window where an edit happens sends:
 * - its edits as they happen;
 * - the points where its model closes an undo step (pushStackElement);
 * - undo and redo as commands, never as edits.
 * A receiving model puts the edits in its own history and closes a step only
 * when told to, so both histories have the same steps and an undo gives the
 * same text everywhere; Java only relays the messages in order.
 *
 * SciNotesSharedHistory has no SciNotes dependency, so that a test page can
 * run it on bare Monaco models.
 */

var SciNotesSharedHistory = (function() {

    function digest(model) {
        var text = model.getValue();
        var hash = 0;
        for (var i = 0; i < text.length; i++) {
            hash = (hash * 31 + text.charCodeAt(i)) | 0;
        }
        return text.length + ":" + hash;
    }

    function toEdits(changes) {
        return changes.map(function(change) {
            return { range: change.range, text: change.text };
        });
    }

    /*
     * A window that joined a file after the other one has a shorter history:
     * an undo reaching past its start is applied as a plain edit. That edit
     * would shift the positions stored in the steps still to redo, so the
     * history left behind it is dropped. Monaco has no public way to do so.
     */
    function clearHistory(model) {
        if (model._undoRedoService && typeof model._undoRedoService.removeElements === "function") {
            model._undoRedoService.removeElements(model.uri);
        } else if (model._commandManager && typeof model._commandManager.clear === "function") {
            model._commandManager.clear();
        }
    }

    /**
     * Relay the history of a model through send(message).
     * Idempotent: a model shared by several tabs is installed once.
     */
    function install(model, send) {
        if (model.__scinotesSharedHistory) {
            return model.__scinotesSharedHistory;
        }

        var sync = {
            remote: 0,
            // Nothing is relayed, and no digest computed, while the file is
            // shown in this window only: SciNotes turns this on when another
            // window starts showing the file.
            enabled: false,
            // Monaco closes an undo step far more often than it opens one;
            // only a close that follows an edit, local or received, changes
            // a receiving history.
            openStep: false,
            push: model.pushStackElement.bind(model),
            pop: model.popStackElement.bind(model)
        };
        model.__scinotesSharedHistory = sync;

        model.pushStackElement = function() {
            if (sync.remote > 0) {
                // Received edits make their own steps.
                return;
            }
            sync.push();
            if (!sync.enabled) {
                return;
            }
            if (sync.openStep) {
                sync.openStep = false;
                send({ kind: "stop", digest: digest(model) });
            }
        };
        model.popStackElement = function() {
            if (sync.remote > 0) {
                return;
            }
            sync.pop();
            if (!sync.enabled) {
                return;
            }
            sync.openStep = true;
            send({ kind: "reopen" });
        };

        model.onDidChangeContent(function(e) {
            // A full replacement (setValue) is not a step: SciNotes pushes
            // it to each window itself.
            if (sync.remote > 0 || !sync.enabled || e.isFlush) {
                return;
            }
            var message = { kind: e.isUndoing ? "undo" : (e.isRedoing ? "redo" : "edit"),
                changes: toEdits(e.changes) };
            if (message.kind === "edit") {
                sync.openStep = true;
                message.length = model.getValueLength();
            } else {
                message.digest = digest(model);
            }
            send(message);
        });
        return sync;
    }

    /**
     * Start or stop relaying this buffer.
     *
     * Starting also closes the current undo step, without telling anyone: the
     * window that just opened the file has an empty history, so the edits
     * typed from now on must not extend a step it does not have.
     */
    function setEnabled(model, enabled) {
        var sync = model.__scinotesSharedHistory;
        if (!sync) {
            return false;
        }
        sync.enabled = !!enabled;
        if (sync.enabled) {
            sync.push();
            sync.openStep = false;
        }
        return true;
    }

    function isApplyingRemote(model) {
        return !!(model.__scinotesSharedHistory && model.__scinotesSharedHistory.remote > 0);
    }

    /*
     * A model remembers the blank lines its last edit indented automatically
     * and strips that indentation within its next pushEditOperations. The
     * sender strips its own and sends the result; a received edit must not
     * strip lines on the receiving side's stale record (internal field).
     */
    function pushReceivedEdit(model, selections, edit) {
        model._trimAutoWhitespaceLines = null;
        model.pushEditOperations(selections || null, [edit], function() { return null; });
    }

    function applyPlain(model, changes) {
        changes.forEach(function(change) {
            model.applyEdits([change]);
        });
        clearHistory(model);
    }

    /**
     * Apply a message sent by another window.
     * @return null, or a description of the difference when the text no
     *         longer matches the sender's
     */
    function applyRemote(model, message, selections) {
        var sync = install(model, function() {});
        sync.remote++;
        try {
            if (message.kind === "edit") {
                sync.openStep = true;
                message.changes.forEach(function(change) {
                    pushReceivedEdit(model, selections, change);
                });
            } else if (message.kind === "stop") {
                sync.openStep = false;
                sync.push();
            } else if (message.kind === "reopen") {
                sync.openStep = true;
                sync.pop();
            } else if (message.kind === "undo") {
                if (model.canUndo()) {
                    model.undo();
                } else {
                    applyPlain(model, message.changes);
                }
            } else if (message.kind === "redo") {
                if (model.canRedo()) {
                    model.redo();
                } else {
                    applyPlain(model, message.changes);
                }
            }
        } finally {
            sync.remote--;
        }

        if (message.length !== undefined && model.getValueLength() !== message.length) {
            return message.kind + ": length " + model.getValueLength() + " instead of " + message.length;
        }
        if (message.digest !== undefined) {
            var actual = digest(model);
            if (actual !== message.digest) {
                return message.kind + ": digest " + actual + " instead of " + message.digest;
            }
        }
        return null;
    }

    /**
     * Put this text in the buffer as the smallest edit that gets there, as
     * one step of its history: carets, scroll position and history are kept,
     * and the change can be undone. Used to repair a window whose text went
     * wrong, and to reload a file changed on disk.
     *
     * @return true when the buffer was not already this text
     */
    function applyContentAsEdit(model, text) {
        var current = model.getValue();
        if (current === text) {
            return false;
        }
        var prefix = 0;
        var max = Math.min(current.length, text.length);
        while (prefix < max && current.charCodeAt(prefix) === text.charCodeAt(prefix)) {
            prefix++;
        }
        var suffix = 0;
        while (suffix < max - prefix
                && current.charCodeAt(current.length - 1 - suffix) === text.charCodeAt(text.length - 1 - suffix)) {
            suffix++;
        }
        var start = model.getPositionAt(prefix);
        var end = model.getPositionAt(current.length - suffix);
        var sync = install(model, function() {});
        sync.remote++;
        try {
            sync.push();
            pushReceivedEdit(model, null, {
                range: new monaco.Range(start.lineNumber, start.column, end.lineNumber, end.column),
                text: text.substring(prefix, text.length - suffix)
            });
            sync.push();
        } finally {
            sync.remote--;
        }
        return true;
    }

    return {
        digest: digest,
        install: install,
        setEnabled: setEnabled,
        applyContentAsEdit: applyContentAsEdit,
        resetHistory: clearHistory,
        isApplyingRemote: isApplyingRemote,
        applyRemote: applyRemote
    };
})();

/*
 * SciNotes side: every tab showing a model keeps its own Java replica, and
 * only a named file is shared between windows.
 */
function installDocumentModel(model) {
    if (!model || model.__scinotesReplicaListener) {
        return;
    }
    model.__scinotesReplicaListener = true;

    SciNotesSharedHistory.install(model, function(message) {
        if (model.uri.scheme === "untitled") {
            return;
        }
        var entries = getDocumentEntriesForModel(model);
        if (!entries.length) {
            return;
        }
        var active = getActiveDocumentEntry();
        var sender = entries.indexOf(active) >= 0 ? active : entries[0];
        notifyJavaForDocument(sender.id, "history:", JSON.stringify(message));
    });

    model.onDidChangeContent(function(e) {
        var state = window.SciNotesMonacoState;
        if (state.applyingJavaValue) {
            return;
        }
        /*
         * Every tab on this model must get the edit, with its own revision,
         * whether it was typed here, received from another window, or made
         * by Java in the replica of one of them (which already has it).
         */
        var entries = getDocumentEntriesForModel(model);
        var javaOrigin = model.__scinotesJavaOrigin;
        for (var i = 0; i < e.changes.length; i++) {
            var c = e.changes[i];
            var edit = ":" + c.rangeOffset + ":" + c.rangeLength + ":" + c.text;
            entries.forEach(function(entry) {
                if (entry.id === javaOrigin) {
                    return;
                }
                var expected = entry.revision;
                entry.revision = expected + 1;
                entry.clean = false;
                notifyJavaForDocument(entry.id, "edit:", expected + ":" + entry.revision + edit);
            });
        }
        var activeDocument = getActiveDocumentEntry();
        if (activeDocument && activeDocument.model === model) {
            state.documentRevision = activeDocument.revision;
        }
        /*
         * Every tab on this model reports whether it still matches its saved
         * version, and after the edits above: an edit received by a Java
         * replica marks it modified, so an undo back to the saved version
         * must be followed by the clean state, not preceded by it.
         */
        var version = model.getAlternativeVersionId();
        entries.forEach(function(entry) {
            var saved = entry === activeDocument
                ? state.savedAlternativeVersionId : entry.savedAlternativeVersionId;
            var modified = saved === null || version !== saved;
            entry.clean = !modified;
            if (entry === activeDocument) {
                state.pendingClean = entry.clean;
            }
            notifyJavaForDocument(entry.id, "modified:", String(modified));
        });
    });
}

window.SciNotesMonacoApplySharedHistory = function(id, json) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return false;
    }
    var active = state.editor && state.editor.getModel() === entry.model;
    var mismatch = SciNotesSharedHistory.applyRemote(entry.model, JSON.parse(json),
        active ? state.editor.getSelections() : null);
    if (mismatch) {
        notifyJavaForDocument(id, "historyMismatch:", mismatch);
    }
    if (active) {
        notifyUndoStateSoon();
    }
    return !mismatch;
};

window.SciNotesMonacoRepairSharedDocument = function(id, text) {
    var entry = getDocumentRegistry()[id];
    return !!entry && SciNotesSharedHistory.applyContentAsEdit(entry.model, text || "");
};

/*
 * The file changed on disk while nothing was being edited. Its contents come
 * as an ordinary edit so that the reload can be undone, and every tab on the
 * buffer takes the new version as its saved one.
 */
window.SciNotesMonacoApplyContentAsEdit = function(id, text, clean) {
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return false;
    }

    SciNotesSharedHistory.applyContentAsEdit(entry.model, text || "");
    if (clean !== false) {
        markModelSaved(entry.model);
    }
    return true;
};

/*
 * This window just opened a file another window is editing: it takes that
 * text and starts its history there. Keeping the step that brought it here
 * would let an undo in this window ask the others to undo a step of their
 * own, which is not the same one.
 */
window.SciNotesMonacoAdoptSharedContent = function(id, text) {
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return false;
    }

    SciNotesSharedHistory.applyContentAsEdit(entry.model, text || "");
    SciNotesSharedHistory.resetHistory(entry.model);
    return true;
};

/*
 * The file is gone from the disk: no version of this buffer is saved any
 * more, so every tab on it is modified until it is written somewhere.
 */
window.SciNotesMonacoForgetSavedVersion = function(id) {
    var state = window.SciNotesMonacoState;
    var entry = getDocumentRegistry()[id];
    if (!entry) {
        return false;
    }

    getDocumentEntriesForModel(entry.model).forEach(function(other) {
        other.savedAlternativeVersionId = null;
        other.clean = false;
        if (other.id === state.activeDocumentId) {
            state.savedAlternativeVersionId = null;
            state.pendingClean = false;
        }
        notifyJavaForDocument(other.id, "modified:", "true");
    });
    return true;
};

function markModelSaved(model) {
    var state = window.SciNotesMonacoState;
    var version = model.getAlternativeVersionId();
    getDocumentEntriesForModel(model).forEach(function(entry) {
        entry.savedAlternativeVersionId = version;
        entry.clean = true;
        if (entry.id === state.activeDocumentId) {
            state.savedAlternativeVersionId = version;
            state.pendingClean = true;
        }
        notifyJavaForDocument(entry.id, "modified:", "false");
    });
}

window.SciNotesMonacoSetSharedDocument = function(id, shared) {
    var entry = getDocumentRegistry()[id];
    return !!entry && SciNotesSharedHistory.setEnabled(entry.model, shared);
};
