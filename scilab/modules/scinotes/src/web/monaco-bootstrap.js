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
 * This file loads Monaco, creates the editor instance, and connects Monaco
 * events to the SciNotes Java bridge. It is the entry point once monaco.html
 * has loaded the shared state, bridge, shortcuts, and language helpers.
 */

function showError(message) {
    var status = document.getElementById("status");
    status.style.display = "block";
    status.className = "error";
    status.textContent = "Monaco failed to load:\n" + message;
}

var loadTimeout = setTimeout(function() {
    showError("timeout after 20s (asset blocked or missing?)");
}, 20000);

function getVsBase() {
    var raw = new URLSearchParams(location.search).get("vs");
    if (!raw) {
        return null;
    }
    return decodeURIComponent(raw.replace(/\+/g, "%20"));
}

function keepDefinitionLinkPointerOnly() {
    Array.prototype.forEach.call(document.styleSheets, function(styleSheet) {
        var rules;
        try {
            rules = styleSheet.cssRules;
        } catch (e) {
            return;
        }
        Array.prototype.forEach.call(rules || [], function(rule) {
            if (rule.selectorText && rule.selectorText.indexOf(".goto-definition-link") >= 0) {
                rule.style.removeProperty("color");
                rule.style.removeProperty("text-decoration");
            }
        });
    });
}

function startMonaco(vsBase) {
    require.onError = function(err) {
        clearTimeout(loadTimeout);
        showError((err && (err.message || err)) + "\n" + (err && err.requireModules ? err.requireModules : ""));
    };

    require.config({ paths: { vs: vsBase } });

    require(["vs/editor/editor.main"], function() {
        var state = window.SciNotesMonacoState;
        clearTimeout(loadTimeout);
        try {
            registerScilabLanguage();
            keepDefinitionLinkPointerOnly();
        } catch (e) {
            showError("Scilab language registration failed:\n" + (e && (e.stack || e.message) || e));
            return;
        }

        state.editor = monaco.editor.create(document.getElementById("editor"), {
            value: state.pendingValue,
            language: "scilab",
            theme: "scinotes-light",
            automaticLayout: true,
            glyphMargin: false,
            lineDecorationsWidth: 4,
            lineNumbersMinChars: 3,
            contextmenu: false,
            selectionHighlightMultiline: true,
            // The LSP answers everywhere, including on an empty position, and
            // it scans the document itself. Monaco's own word harvesting would
            // only add what the Scilab word pattern accepts as a word, a lone
            // "!" or "#" included.
            wordBasedSuggestions: "off",
            minimap: { enabled: false },
            hover: { delay: 500 },
            padding: { top: 2, bottom: 2 },
            scrollBeyondLastLine: false,
            readOnly: state.pendingReadOnly,
            fontSize: 13,
            autoIndent: "full",
            renderWhitespace: "none",
            suggest: {
                showInlineDetails: true,
                showStatusBar: true,
                // By default Monaco stops completing as you type in a snippet.
                snippetsPreventQuickSuggestions: false
            },
            "semanticHighlighting.enabled": true
        });

        applyEditorOptions(state.pendingOptions);
        state.editor.getModel().setEOL(monaco.editor.EndOfLineSequence.LF);
        updateSavedAlternativeVersion();
        window.SciNotesMonacoRegisterInitialDocument();
        configureShortcuts(state.pendingShortcutConfig);
        configureLatexPreview();
        configureLsp(state.pendingLspConfig);
        configureContextMenu();
        configureFocusHandling();
        configureOpenUrlHandling();
        configureFileDrop();

        state.editor.onDidFocusEditorText(function() {
            state.secondaryViewActive = false;
            notifyJava("focus:", "in");
            notifySelection();
            notifyUndoState();
        });

        state.editor.onDidChangeCursorPosition(function() {
            notifySelection();
        });

        state.editor.onDidChangeCursorSelection(function() {
            notifySelection();
        });

        state.editor.onDidChangeModelContent(function(e) {
            if (state.applyingJavaValue) {
                return;
            }
            recordUndoableEdit(e);
            // The Java replicas get the edit from the model itself, see
            // installDocumentModel() in monaco-shared-history.js.
            notifyModifiedState();
            notifyUndoStateSoon();
        });

        document.getElementById("status").style.display = "none";
        notifyUndoStateSoon();

        // editor.main is loaded asynchronously, long after CEF's onLoadEnd.
        // Tell Java the editor now exists so it can run the per-tab model
        // creation that would silently no-op while `monaco` was undefined.
        notifyJava("monacoReady", "");
    });
}

(function bootstrap() {
    var vsBase = getVsBase();
    if (!vsBase) {
        showError("missing ?vs= base path (Monaco thirdparty location not provided by SciNotes)");
        return;
    }
    var loader = document.createElement("script");
    loader.src = vsBase + "/loader.js";
    loader.onload = function() {
        startMonaco(vsBase);
    };
    loader.onerror = function() {
        clearTimeout(loadTimeout);
        showError("cannot load loader.js from\n" + vsBase);
    };
    document.head.appendChild(loader);
})();
