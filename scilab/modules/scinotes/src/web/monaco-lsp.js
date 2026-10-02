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
 * This file owns the lightweight JSON-RPC WebSocket client used to connect the
 * Monaco model to the Scilab LSP endpoint. It provides the transport and model
 * synchronization; Monaco language providers are added on top of this client.
 */

function createSciNotesLspClient(config) {
    var state = window.SciNotesMonacoState;
    var requestId = 1;
    var initializeRequestId = 0;
    var socket = null;
    var initialized = false;
    var documents = {};
    var modelLifecycleListeners = [];
    var reconnectTimer = null;
    var reconnectDelay = 250;
    var reconnectAttempts = 0;
    var maxReconnectAttempts = 120;
    var hoverProvider = null;
    var completionProvider = null;
    var signatureHelpProvider = null;
    var documentSymbolProvider = null;
    var foldingRangeProvider = null;
    var formattingProvider = null;
    var rangeFormattingProvider = null;
    var definitionProvider = null;
    var definitionEditorOpener = null;
    // One entry per editor view showing the pointer: the primary editor,
    // plus the split's second view while it is mounted (monaco-split.js).
    // Each holds { editor, decoration, listeners, mousePosition }.
    var definitionPointerViews = [];
    var definitionPointerListeners = [];
    var semanticMacroRanges = {};
    var semanticTokensProvider = null;
    var semanticTokensChangeListeners = [];
    var pendingRequests = {};
    var diagnosticsOwner = "scinotes-lsp";
    var scilabHelpCommand = null;
    var hoverContextMenuBlocker = null;

    function updateDebug(values) {
        state.lspDebug = state.lspDebug || {};
        for (var name in values) {
            if (Object.prototype.hasOwnProperty.call(values, name)) {
                state.lspDebug[name] = values[name];
            }
        }
    }

    function debugLog(message, detail) {
        updateDebug({
            lastMessage: message,
            lastDetail: detail || null,
            lastUpdate: new Date().toISOString()
        });
        if (window.console && typeof window.console.debug === "function") {
            window.console.debug("[SciNotes LSP] " + message, detail || "");
        }
    }

    function send(message) {
        if (!socket || socket.readyState !== WebSocket.OPEN) {
            debugLog("drop message while socket is not open", message && message.method);
            return;
        }
        socket.send(JSON.stringify(message));
    }

    function request(method, params) {
        var id = requestId++;
        send({
            jsonrpc: "2.0",
            id: id,
            method: method,
            params: params || {}
        });
        return id;
    }

    function requestPromise(method, params) {
        return new Promise(function(resolve, reject) {
            var id = request(method, params);
            pendingRequests[id] = {
                resolve: resolve,
                reject: reject
            };
        });
    }

    function notify(method, params) {
        send({
            jsonrpc: "2.0",
            method: method,
            params: params || {}
        });
    }

    function getModel() {
        return state.editor && state.editor.getModel();
    }

    function modelUri(model) {
        return model && model.uri ? model.uri.toString() : null;
    }

    function documentForModel(model) {
        var uri = modelUri(model);
        return uri ? documents[uri] || null : null;
    }

    function isModelOpened(model) {
        var document = documentForModel(model);
        return initialized && !!document && document.opened;
    }

    function registerModel(model) {
        var uri = modelUri(model);
        if (!uri || documents[uri]) {
            return;
        }

        // This server only knows Scilab; a buffer opened in another language
        // must not be parsed nor diagnosed as Scilab code.
        if (model.getLanguageId() !== (config.languageId || "scilab")) {
            return;
        }

        var document = {
            uri: uri,
            model: model,
            version: 1,
            opened: false,
            foldingVersion: 0,
            foldingRanges: null,
            foldingPromise: null,
            foldingTimer: null,
            listener: model.onDidChangeContent(function() {
                changeDocument(model);
            })
        };
        documents[uri] = document;
        if (initialized) {
            openDocument(model);
        }
    }

    function unregisterModel(model) {
        var document = documentForModel(model);
        if (!document) {
            return;
        }
        closeDocument(model);
        if (document.listener) {
            document.listener.dispose();
        }
        if (document.foldingTimer) {
            clearTimeout(document.foldingTimer);
        }
        delete documents[document.uri];
        delete semanticMacroRanges[document.uri];
    }

    function openDocument(model) {
        var document = documentForModel(model);
        if (!document || document.opened || !initialized) {
            return;
        }

        notify("textDocument/didOpen", {
            textDocument: {
                uri: document.uri,
                languageId: config.languageId || "scilab",
                version: document.version,
                text: model.getValue()
            }
        });
        document.opened = true;
        requestFoldingRanges(model);
        updateDebug({
            opened: true,
            documentUri: document.uri,
            openedDocuments: Object.keys(documents).filter(function(uri) {
                return documents[uri].opened;
            }).length,
            modelLanguageId: model.getLanguageId ? model.getLanguageId() : null
        });
    }

    function changeDocument(model) {
        var document = documentForModel(model);
        if (!document || !document.opened) {
            return;
        }

        document.version++;
        // Monaco adjusts its semantic-token store as the model changes.  The
        // macro-navigation cache is separate, so do not keep its old ranges.
        delete semanticMacroRanges[document.uri];
        document.foldingVersion = 0;
        document.foldingRanges = null;
        document.foldingPromise = null;
        // Every tab showing this model loses the numbering derived from it.
        if (typeof getDocumentEntriesForModel === "function") {
            getDocumentEntriesForModel(model).forEach(function(entry) {
                entry.astLineNumbers = null;
            });
        }
        notify("textDocument/didChange", {
            textDocument: {
                uri: document.uri,
                version: document.version
            },
            contentChanges: [{
                text: model.getValue()
            }]
        });
        if (document.foldingTimer) {
            clearTimeout(document.foldingTimer);
        }
        document.foldingTimer = setTimeout(function() {
            document.foldingTimer = null;
            requestFoldingRanges(model);
        }, 150);
    }

    function closeDocument(model) {
        var document = documentForModel(model);
        if (!document || !document.opened) {
            return;
        }

        notify("textDocument/didClose", {
            textDocument: {
                uri: document.uri
            }
        });
        document.opened = false;
        if (document.foldingTimer) {
            clearTimeout(document.foldingTimer);
            document.foldingTimer = null;
        }
        updateDebug({ openedDocuments: Object.keys(documents).filter(function(uri) {
            return documents[uri].opened;
        }).length });
    }

    function attachModelListeners() {
        if (modelLifecycleListeners.length || typeof monaco === "undefined" || !monaco.editor) {
            return;
        }
        monaco.editor.getModels().forEach(registerModel);
        modelLifecycleListeners.push(monaco.editor.onDidCreateModel(function(model) {
            registerModel(model);
        }));
        modelLifecycleListeners.push(monaco.editor.onWillDisposeModel(function(model) {
            unregisterModel(model);
        }));
    }

    function detachModelListeners() {
        modelLifecycleListeners.forEach(function(listener) {
            listener.dispose();
        });
        modelLifecycleListeners = [];
    }

    /*
     * The calling forms of a hover -- the syntax block of a help page, or the
     * prototype of a macro, which opens the hover -- get the prototype
     * languages of monaco-scilab-language.js, which color them as the editor
     * would; the name takes the macro color when the hovered word is one.
     * Keywords keep the plain Scilab block: their "syntax" is a code sample.
     */
    function colorHelpSyntax(contents, model, position) {
        var value = contents && typeof contents === "object" && !Array.isArray(contents)
            ? contents.value : null;
        if (typeof value !== "string" || typeof registerScilabPrototypeLanguage !== "function"
                || (value.indexOf("(scilab-help://") < 0 && value.indexOf("```scilab\n") !== 0)) {
            return contents;
        }
        var word = model.getWordAtPosition(position);
        if (!word || isScilabKeyword(word.word)) {
            return contents;
        }
        // A hover opening on a prototype is a macro's, documented by no help page.
        var macro = value.indexOf("```scilab\n") === 0 || isMacroPosition(position);
        var language = macro ? "scilab-proto-macro" : "scilab-proto";
        return {
            kind: contents.kind,
            value: value.replace(/^```scilab$/m, "```" + language)
        };
    }

    function convertMarkupContent(contents) {
        if (!contents) {
            return null;
        }

        if (typeof contents === "string") {
            return createMarkdownString(contents);
        }

        if (Array.isArray(contents)) {
            return contents.map(convertMarkupContent).filter(function(entry) {
                return entry && entry.value;
            });
        }

        if (contents.value) {
            return createMarkdownString(contents.value);
        }

        return null;
    }

    function createMarkdownString(value) {
        var markdown = rewriteScilabHelpLinks(value);
        return {
            value: markdown.value,
            supportHtml: false,
            isTrusted: markdown.hasCommand ? {
                enabledCommands: ["scinotes.openHelp"]
            } : false
        };
    }

    function rewriteScilabHelpLinks(value) {
        var hasCommand = false;
        var rewritten = String(value || "").replace(/\]\(scilab-help:\/\/([^)]+)\)/g, function(match, target) {
            var page = decodeScilabHelpTarget(target);
            hasCommand = true;
            return "](" + getScilabHelpCommandUri(page) + " \"Scilab doc: "
                + escapeMarkdownTitle(page) + "\")";
        });
        return {
            value: rewritten,
            hasCommand: hasCommand
        };
    }

    function getScilabHelpCommandUri(target) {
        var args = encodeURIComponent(JSON.stringify([target]));
        return "command:scinotes.openHelp?" + args;
    }

    function escapeMarkdownTitle(value) {
        return String(value).replace(/\\/g, "\\\\").replace(/\"/g, "\\\"");
    }

    function decodeScilabHelpTarget(value) {
        value = String(value || "").replace(/^\/+/, "");
        try {
            return decodeURIComponent(value);
        } catch (e) {
            return value;
        }
    }

    function registerScilabHelpCommand() {
        if (scilabHelpCommand || typeof monaco === "undefined" || !monaco.editor
                || typeof monaco.editor.addCommand !== "function") {
            return;
        }

        scilabHelpCommand = monaco.editor.addCommand({
            id: "scinotes.openHelp",
            run: function(accessor, target) {
                openScilabHelp(target);
            }
        });
    }

    function openScilabHelp(target) {
        if (!target) {
            return;
        }
        request("workspace/executeCommand", {
            command: "scilab.execute",
            arguments: ["doc('" + escapeScilabString(target) + "')"]
        });
    }

    function escapeScilabString(value) {
        return String(value).replace(/'/g, "''");
    }

    function detachScilabHelpCommand() {
        if (scilabHelpCommand) {
            scilabHelpCommand.dispose();
            scilabHelpCommand = null;
        }
    }

    function attachHoverContextMenuBlocker() {
        if (hoverContextMenuBlocker) {
            return;
        }

        hoverContextMenuBlocker = function(event) {
            var node = event.target;
            while (node && node !== document.body) {
                if (node.classList && node.classList.contains("monaco-hover")) {
                    event.preventDefault();
                    event.stopPropagation();
                    return false;
                }
                node = node.parentNode;
            }
            return true;
        };
        document.addEventListener("contextmenu", hoverContextMenuBlocker, true);
    }

    function detachHoverContextMenuBlocker() {
        if (hoverContextMenuBlocker) {
            document.removeEventListener("contextmenu", hoverContextMenuBlocker, true);
            hoverContextMenuBlocker = null;
        }
    }

    function convertRange(range) {
        if (!range) {
            return null;
        }

        return new monaco.Range(
            range.start.line + 1,
            range.start.character + 1,
            range.end.line + 1,
            range.end.character + 1
        );
    }

    function convertDiagnosticSeverity(severity) {
        var markerSeverity = monaco.MarkerSeverity;
        switch (severity) {
        case 1:
            return markerSeverity.Error;
        case 2:
            return markerSeverity.Warning;
        case 3:
            return markerSeverity.Info;
        case 4:
            return markerSeverity.Hint;
        default:
            return markerSeverity.Error;
        }
    }

    function convertDiagnostic(diagnostic) {
        var range = convertRange(diagnostic && diagnostic.range);
        if (!range) {
            return null;
        }
        if (range.isEmpty && range.isEmpty()) {
            range = new monaco.Range(
                range.startLineNumber,
                range.startColumn,
                range.startLineNumber,
                range.startColumn + 1
            );
        }

        return {
            severity: convertDiagnosticSeverity(diagnostic.severity),
            message: diagnostic.message || "",
            source: diagnostic.source || "Scilab",
            startLineNumber: range.startLineNumber,
            startColumn: range.startColumn,
            endLineNumber: range.endLineNumber,
            endColumn: range.endColumn
        };
    }

    function applyDiagnostics(params) {
        if (!params || !params.uri) {
            return;
        }
        var document = documents[params.uri] || null;
        var uri = convertUri(params.uri);
        var model = document ? document.model : (uri ? monaco.editor.getModel(uri) : null);
        if (!model) {
            return;
        }

        var diagnostics = Array.isArray(params.diagnostics) ? params.diagnostics : [];
        var markers = diagnostics.map(convertDiagnostic).filter(function(marker) {
            return marker !== null;
        });
        monaco.editor.setModelMarkers(model, diagnosticsOwner, markers);
        updateDebug({
            diagnosticsReceived: diagnostics.length,
            diagnosticsApplied: markers.length,
            lastDiagnostic: diagnostics.length ? diagnostics[0] : null
        });
    }

    function clearDiagnostics() {
        if (typeof monaco !== "undefined" && monaco.editor) {
            Object.keys(documents).forEach(function(uri) {
                monaco.editor.setModelMarkers(documents[uri].model, diagnosticsOwner, []);
            });
        }
        updateDebug({
            diagnosticsReceived: 0,
            diagnosticsApplied: 0,
            lastDiagnostic: null
        });
    }

    function convertUri(uri) {
        if (!uri) {
            return null;
        }

        try {
            return monaco.Uri.parse(uri);
        } catch (e) {
            return null;
        }
    }

    function convertSymbolKind(kind) {
        var kinds = monaco.languages.SymbolKind;
        var byLspKind = {};
        byLspKind[1] = kinds.File;
        byLspKind[2] = kinds.Module;
        byLspKind[3] = kinds.Namespace;
        byLspKind[4] = kinds.Package;
        byLspKind[5] = kinds.Class;
        byLspKind[6] = kinds.Method;
        byLspKind[7] = kinds.Property;
        byLspKind[8] = kinds.Field;
        byLspKind[9] = kinds.Constructor;
        byLspKind[10] = kinds.Enum;
        byLspKind[11] = kinds.Interface;
        byLspKind[12] = kinds.Function;
        byLspKind[13] = kinds.Variable;
        byLspKind[14] = kinds.Constant;
        byLspKind[15] = kinds.String;
        byLspKind[16] = kinds.Number;
        byLspKind[17] = kinds.Boolean;
        byLspKind[18] = kinds.Array;
        byLspKind[19] = kinds.Object;
        byLspKind[20] = kinds.Key;
        byLspKind[21] = kinds.Null;
        byLspKind[22] = kinds.EnumMember;
        byLspKind[23] = kinds.Struct;
        byLspKind[24] = kinds.Event;
        byLspKind[25] = kinds.Operator;
        byLspKind[26] = kinds.TypeParameter;
        return byLspKind[kind] || kinds.Variable;
    }

    function convertDocumentSymbol(symbol) {
        var range = convertRange(symbol.range);
        var selectionRange = convertRange(symbol.selectionRange) || range;
        if (!range || !selectionRange) {
            return null;
        }

        var converted = {
            name: symbol.name || "",
            detail: symbol.detail || "",
            kind: convertSymbolKind(symbol.kind),
            range: range,
            selectionRange: selectionRange
        };

        if (Array.isArray(symbol.children)) {
            converted.children = symbol.children.map(convertDocumentSymbol).filter(function(child) {
                return child !== null;
            });
        }

        return converted;
    }

    var semanticTokenLegend = {
        tokenTypes: [
            "namespace", "type", "class", "enum", "interface", "struct", "typeParameter",
            "parameter", "variable", "property", "enumMember", "event", "function",
            "method", "macro", "keyword", "modifier", "comment", "string", "number",
            "regexp", "operator", "decorator", "operatorSpecial", "functionKeyword",
            "controlKeyword"
        ],
        tokenModifiers: [
            "declaration", "definition", "readonly", "static", "deprecated", "abstract",
            "async", "modification", "documentation", "defaultLibrary", "scilabRuntime"
        ]
    };

    var semanticKeywordType = 15;
    var semanticControlKeywordType = 25;
    var semanticControlKeywords = {
        abort: true,
        break: true,
        quit: true,
        return: true,
        resume: true,
        pause: true,
        continue: true,
        exit: true
    };

    function normalizeSemanticTokenData(result, lines) {
        if (!result || !Array.isArray(result.data)) {
            return [];
        }

        var data = result.data.slice();
        var line = 0;
        var start = 0;
        for (var index = 0; lines && index + 4 < data.length; index += 5) {
            line += data[index];
            start = data[index] === 0 ? start + data[index + 1] : data[index + 1];
            if (data[index + 3] !== semanticKeywordType) {
                continue;
            }

            var content = lines[line] || "";
            var word = content.substr(start, data[index + 2]);
            if (Object.prototype.hasOwnProperty.call(semanticControlKeywords, word)) {
                data[index + 3] = semanticControlKeywordType;
            }
        }
        return data;
    }

    function convertDefinitionLocation(location) {
        if (!location) {
            return null;
        }

        var uri = convertUri(location.uri || location.targetUri);
        var range = convertRange(location.range || location.targetSelectionRange || location.targetRange);
        if (!uri || !range) {
            return null;
        }

        return {
            uri: uri,
            range: range
        };
    }

    function requestDefinitionLocations(model, position) {
        if (!isModelOpened(model)) {
            return Promise.resolve([]);
        }

        return requestPromise("textDocument/definition", {
            textDocument: {
                uri: modelUri(model)
            },
            position: {
                line: position.lineNumber - 1,
                character: position.column - 1
            }
        }).then(function(result) {
            refreshSemanticTokens();
            if (Array.isArray(result)) {
                return result.map(convertDefinitionLocation).filter(function(location) {
                    return location !== null;
                });
            }

            var location = convertDefinitionLocation(result);
            return location ? [location] : [];
        }, function() {
            refreshSemanticTokens();
            return [];
        });
    }

    function convertCompletionKind(kind) {
        var kinds = monaco.languages.CompletionItemKind;
        switch (kind) {
            case 1: return kinds.Text;
            case 2: return kinds.Method;
            case 3: return kinds.Function;
            case 4: return kinds.Constructor;
            case 5: return kinds.Field;
            case 6: return kinds.Variable;
            case 7: return kinds.Class;
            case 8: return kinds.Interface;
            case 9: return kinds.Module;
            case 10: return kinds.Property;
            case 11: return kinds.Unit;
            case 12: return kinds.Value;
            case 13: return kinds.Enum;
            case 14: return kinds.Keyword;
            case 15: return kinds.Snippet;
            case 16: return kinds.Color;
            case 17: return kinds.File;
            case 18: return kinds.Reference;
            case 19: return kinds.Folder;
            case 20: return kinds.EnumMember;
            case 21: return kinds.Constant;
            case 22: return kinds.Struct;
            case 23: return kinds.Event;
            case 24: return kinds.Operator;
            case 25: return kinds.TypeParameter;
            default: return kinds.Text;
        }
    }

    function convertCompletionDocumentation(documentation) {
        var contents = convertMarkupContent(documentation);
        if (!contents) {
            return undefined;
        }

        if (Array.isArray(contents)) {
            return {
                value: contents.map(function(entry) {
                    return entry.value || "";
                }).join("\n\n"),
                supportHtml: false,
                isTrusted: false
            };
        }

        return contents;
    }

    function getDefaultCompletionRange(model, position) {
        var word = model.getWordUntilPosition(position);
        return new monaco.Range(
            position.lineNumber,
            word.startColumn,
            position.lineNumber,
            word.endColumn
        );
    }

    function convertTextEditRange(textEdit, fallbackRange) {
        if (!textEdit || !textEdit.range) {
            return fallbackRange;
        }

        return convertRange(textEdit.range) || fallbackRange;
    }

    function convertCompletionItem(item, model, position) {
        var textEdit = item.textEdit || null;
        var fallbackRange = getDefaultCompletionRange(model, position);
        /*
         * A plain string label makes Monaco show the detail of a row only
         * while that row is focused ("string-label" in its suggest widget
         * stylesheet). Carried by the structured label instead, the
         * provenance stays visible on every line.
         */
        var label = item.detail
            ? { label: item.label || "", description: item.detail }
            : (item.label || "");
        var suggestion = {
            label: label,
            kind: convertCompletionKind(item.kind),
            insertText: item.insertText || (textEdit && textEdit.newText) || item.label || "",
            range: convertTextEditRange(textEdit, fallbackRange),
            sortText: item.sortText,
            filterText: item.filterText,
            detail: item.detail,
            documentation: convertCompletionDocumentation(item.documentation),
            __lspItem: item
        };

        if (textEdit && textEdit.newText) {
            suggestion.insertText = textEdit.newText;
        }

        if (item.insertTextFormat === 2) {
            suggestion.insertTextRules = monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet;
        }

        if (Array.isArray(item.tags) && item.tags.indexOf(1) !== -1 && monaco.languages.CompletionItemTag) {
            suggestion.tags = [monaco.languages.CompletionItemTag.Deprecated];
        }

        return suggestion;
    }

    function normalizeCompletionResult(result) {
        if (!result) {
            return [];
        }

        if (Array.isArray(result)) {
            return result;
        }

        if (Array.isArray(result.items)) {
            return result.items;
        }

        return [];
    }

    function attachCompletionProvider() {
        if (completionProvider || typeof monaco === "undefined" || !monaco.languages) {
            return;
        }

        completionProvider = monaco.languages.registerCompletionItemProvider(config.languageId || "scilab", {
            triggerCharacters: ["."],
            provideCompletionItems: function(model, position) {
                if (!isModelOpened(model)) {
                    debugLog("completion skipped", {
                        initialized: initialized,
                        opened: !!(documentForModel(model) && documentForModel(model).opened),
                        modelLanguageId: model && model.getLanguageId ? model.getLanguageId() : null
                    });
                    return null;
                }

                updateDebug({
                    completionCalls: (state.lspDebug && state.lspDebug.completionCalls || 0) + 1,
                    modelLanguageId: model.getLanguageId ? model.getLanguageId() : null,
                    lastCompletionPosition: {
                        line: position.lineNumber,
                        column: position.column
                    }
                });
                debugLog("completion request", state.lspDebug.lastCompletionPosition);
                return requestPromise("textDocument/completion", {
                    textDocument: {
                        uri: modelUri(model)
                    },
                    position: {
                        line: position.lineNumber - 1,
                        character: position.column - 1
                    }
                }).then(function(result) {
                    var items = normalizeCompletionResult(result);
                    updateDebug({ lastCompletionItems: items.length });
                    debugLog("completion response", { items: items.length });
                    return {
                        suggestions: items.map(function(item) {
                            return convertCompletionItem(item, model, position);
                        })
                    };
                }, function() {
                    debugLog("completion failed");
                    return { suggestions: [] };
                });
            },
            resolveCompletionItem: function(item) {
                if (!initialized || !item.__lspItem) {
                    return item;
                }

                return requestPromise("completionItem/resolve", item.__lspItem).then(function(result) {
                    if (result) {
                        item.detail = result.detail || item.detail;
                        item.documentation = convertCompletionDocumentation(result.documentation) || item.documentation;
                        item.__lspItem = result;
                    }
                    return item;
                }, function() {
                    return item;
                });
            }
        });
        updateDebug({ completionProviderAttached: true });
        debugLog("completion provider attached", config.languageId || "scilab");
    }

    function detachCompletionProvider() {
        if (completionProvider) {
            completionProvider.dispose();
            completionProvider = null;
            updateDebug({ completionProviderAttached: false });
        }
    }

    function convertSignatureDocumentation(documentation) {
        return convertCompletionDocumentation(documentation);
    }

    function convertSignatureHelp(result) {
        if (!result || !Array.isArray(result.signatures)) {
            return {
                signatures: [],
                activeSignature: 0,
                activeParameter: 0
            };
        }

        return {
            signatures: result.signatures.map(function(signature) {
                return {
                    label: signature.label || "",
                    documentation: convertSignatureDocumentation(signature.documentation),
                    parameters: Array.isArray(signature.parameters) ? signature.parameters.map(function(parameter) {
                        return {
                            label: parameter.label,
                            documentation: convertSignatureDocumentation(parameter.documentation)
                        };
                    }) : []
                };
            }),
            activeSignature: result.activeSignature || 0,
            activeParameter: result.activeParameter || 0
        };
    }

    function attachSignatureHelpProvider() {
        if (signatureHelpProvider || !config.enableSignatureHelp || typeof monaco === "undefined" || !monaco.languages) {
            return;
        }

        signatureHelpProvider = monaco.languages.registerSignatureHelpProvider(config.languageId || "scilab", {
            signatureHelpTriggerCharacters: ["("],
            signatureHelpRetriggerCharacters: [","],
            provideSignatureHelp: function(model, position, token, context) {
                if (!isModelOpened(model)) {
                    debugLog("signatureHelp skipped", {
                        initialized: initialized,
                        opened: !!(documentForModel(model) && documentForModel(model).opened)
                    });
                    return null;
                }

                updateDebug({
                    signatureHelpCalls: (state.lspDebug && state.lspDebug.signatureHelpCalls || 0) + 1,
                    lastSignatureHelpPosition: {
                        line: position.lineNumber,
                        column: position.column
                    }
                });
                debugLog("signatureHelp request", state.lspDebug.lastSignatureHelpPosition);
                return requestPromise("textDocument/signatureHelp", {
                    textDocument: {
                        uri: modelUri(model)
                    },
                    position: {
                        line: position.lineNumber - 1,
                        character: position.column - 1
                    },
                    context: {
                        isRetrigger: !!(context && context.isRetrigger),
                        activeSignatureHelp: context ? context.activeSignatureHelp || null : null
                    }
                }).then(function(result) {
                    debugLog("signatureHelp response");
                    return {
                        value: convertSignatureHelp(result),
                        dispose: function() { }
                    };
                }, function() {
                    debugLog("signatureHelp failed");
                    return null;
                });
            }
        });
        updateDebug({ signatureHelpProviderAttached: true });
        debugLog("signatureHelp provider attached", config.languageId || "scilab");
    }

    function detachSignatureHelpProvider() {
        if (signatureHelpProvider) {
            signatureHelpProvider.dispose();
            signatureHelpProvider = null;
            updateDebug({ signatureHelpProviderAttached: false });
        }
    }

    function attachDocumentSymbolProvider() {
        if (documentSymbolProvider || typeof monaco === "undefined" || !monaco.languages) {
            return;
        }

        documentSymbolProvider = monaco.languages.registerDocumentSymbolProvider(config.languageId || "scilab", {
            provideDocumentSymbols: function(model) {
                if (!isModelOpened(model)) {
                    return null;
                }

                return requestPromise("textDocument/documentSymbol", {
                    textDocument: {
                        uri: modelUri(model)
                    }
                }).then(function(result) {
                    if (!Array.isArray(result)) {
                        return [];
                    }

                    return result.map(convertDocumentSymbol).filter(function(symbol) {
                        return symbol !== null;
                    });
                }, function() {
                    return [];
                });
            }
        });
    }

    function detachDocumentSymbolProvider() {
        if (documentSymbolProvider) {
            documentSymbolProvider.dispose();
            documentSymbolProvider = null;
        }
    }

    function attachFoldingRangeProvider() {
        if (foldingRangeProvider || typeof monaco === "undefined" || !monaco.languages
                || typeof monaco.languages.registerFoldingRangeProvider !== "function") {
            return;
        }

        foldingRangeProvider = monaco.languages.registerFoldingRangeProvider(config.languageId || "scilab", {
            provideFoldingRanges: function(model) {
                // Answering nothing, where an empty list would do, leaves Monaco
                // its indentation folding: a buffer the parser cannot read keeps folds.
                if (!isModelOpened(model)) {
                    return null;
                }
                return requestFoldingRanges(model).then(function(ranges) {
                    if (!ranges.length) {
                        return null;
                    }

                    return ranges.map(function(range) {
                        return {
                            start: range.startLine + 1,
                            end: range.endLine + 1
                        };
                    });
                }, function() {
                    return null;
                });
            }
        });
    }

    function requestFoldingRanges(model) {
        var document = documentForModel(model);
        if (!document || !document.opened) {
            return Promise.resolve([]);
        }

        var requestedVersion = model.getVersionId();
        if (document.foldingVersion === requestedVersion
                && Array.isArray(document.foldingRanges)) {
            return Promise.resolve(document.foldingRanges);
        }
        if (document.foldingPromise && document.foldingVersion === requestedVersion) {
            return document.foldingPromise;
        }

        document.foldingVersion = requestedVersion;
        document.foldingPromise = requestPromise("textDocument/foldingRange", {
            textDocument: { uri: modelUri(model) }
        }).then(function(ranges) {
            if (!Array.isArray(ranges) || model.getVersionId() !== requestedVersion) {
                return [];
            }
            document.foldingRanges = ranges;
            document.foldingPromise = null;
            if (typeof window.SciNotesMonacoApplyAstFunctionRanges === "function") {
                window.SciNotesMonacoApplyAstFunctionRanges(ranges.filter(function(range) {
                    return range.scilabRole === "function";
                }), model);
            }
            return ranges;
        }, function() {
            document.foldingPromise = null;
            return [];
        });
        return document.foldingPromise;
    }

    function detachFoldingRangeProvider() {
        if (foldingRangeProvider) {
            foldingRangeProvider.dispose();
            foldingRangeProvider = null;
        }
    }

    /*
     * The server only rewrites the whitespace at the start of lines, from the
     * AST. It refuses a document that does not parse: its message goes to the
     * SciNotes info bar and the buffer is left as it is.
     */
    function requestFormattingEdits(model, method, options, range) {
        if (!isModelOpened(model)) {
            return Promise.resolve([]);
        }

        var params = {
            textDocument: { uri: modelUri(model) },
            options: {
                tabSize: options.tabSize,
                insertSpaces: options.insertSpaces
            }
        };
        if (range) {
            params.range = {
                start: { line: range.startLineNumber - 1, character: range.startColumn - 1 },
                end: { line: range.endLineNumber - 1, character: range.endColumn - 1 }
            };
        }

        return requestPromise(method, params).then(function(edits) {
            if (!Array.isArray(edits)) {
                return [];
            }
            return edits.map(function(edit) {
                return {
                    range: new monaco.Range(
                        edit.range.start.line + 1, edit.range.start.character + 1,
                        edit.range.end.line + 1, edit.range.end.character + 1),
                    text: edit.newText
                };
            });
        }, function(error) {
            if (error && error.message && typeof notifyJava === "function") {
                notifyJava("infoBar:", error.message);
            }
            return [];
        });
    }

    function attachFormattingProviders() {
        if (formattingProvider || typeof monaco === "undefined" || !monaco.languages) {
            return;
        }

        var languageId = config.languageId || "scilab";
        formattingProvider = monaco.languages.registerDocumentFormattingEditProvider(languageId, {
            provideDocumentFormattingEdits: function(model, options) {
                return requestFormattingEdits(model, "textDocument/formatting", options, null);
            }
        });
        rangeFormattingProvider = monaco.languages.registerDocumentRangeFormattingEditProvider(languageId, {
            provideDocumentRangeFormattingEdits: function(model, range, options) {
                return requestFormattingEdits(model, "textDocument/rangeFormatting", options, range);
            }
        });
    }

    function detachFormattingProviders() {
        if (formattingProvider) {
            formattingProvider.dispose();
            formattingProvider = null;
        }
        if (rangeFormattingProvider) {
            rangeFormattingProvider.dispose();
            rangeFormattingProvider = null;
        }
    }

    function attachDefinitionProvider() {
        if (definitionProvider || typeof monaco === "undefined" || !monaco.languages) {
            return;
        }

        definitionProvider = monaco.languages.registerDefinitionProvider(config.languageId || "scilab", {
            provideDefinition: function(model, position) {
                return requestDefinitionLocations(model, position).then(function(locations) {
                    return locations.length ? locations[0] : null;
                });
            }
        });

        attachDefinitionPointer(state.editor);
        if (state.secondaryEditor) {
            attachDefinitionPointer(state.secondaryEditor);
        }
        definitionPointerListeners.push(addDefinitionPointerDomListener(document, "keyup", function(event) {
            if ((event.key === "Control" || event.key === "Meta")
                    && !event.ctrlKey && !event.metaKey) {
                clearDefinitionPointer();
            }
        }));
        definitionPointerListeners.push(addDefinitionPointerDomListener(window, "blur", function() {
            clearDefinitionPointer();
        }));

        if (!definitionEditorOpener && monaco.editor
                && typeof monaco.editor.registerEditorOpener === "function") {
            definitionEditorOpener = monaco.editor.registerEditorOpener({
                openCodeEditor: function(sourceEditor, resource, selection) {
                    var sourceModel = sourceEditor && sourceEditor.getModel();
                    if (!resource || !selection || !sourceModel
                            || sourceModel.getLanguageId() !== (config.languageId || "scilab")) {
                        return false;
                    }

                    // Let Monaco navigate within the current model. SciNotes
                    // only needs to handle definitions located in another file.
                    if (sourceModel.uri.toString() === resource.toString()) {
                        return false;
                    }

                    var lineNumber = selection.startLineNumber || selection.lineNumber;
                    var column = selection.startColumn || selection.column;
                    if (!lineNumber || !column) {
                        return false;
                    }

                    notifyJava("openDefinition:", encodeURIComponent(resource.toString()) + ":"
                        + lineNumber + ":" + column);
                    return true;
                }
            });
        }
    }

    function attachDefinitionPointer(editor) {
        if (!definitionProvider || !editor || findDefinitionPointerView(editor)) {
            return;
        }

        var view = {
            editor: editor,
            decoration: editor.createDecorationsCollection(),
            listeners: [],
            mousePosition: null
        };
        view.listeners.push(editor.onMouseMove(function(event) {
            view.mousePosition = event.target && event.target.position;
            if (event.event && (event.event.ctrlKey || event.event.metaKey)
                    && isMacroPosition(view.mousePosition)) {
                updateDefinitionPointer(view, view.mousePosition);
            } else {
                clearDefinitionPointer();
            }
        }));
        view.listeners.push(editor.onMouseLeave(function() {
            view.mousePosition = null;
            clearDefinitionPointer();
        }));
        view.listeners.push(editor.onKeyDown(function(event) {
            if ((event.ctrlKey || event.metaKey) && isMacroPosition(view.mousePosition)) {
                updateDefinitionPointer(view, view.mousePosition);
            }
        }));
        view.listeners.push(editor.onKeyUp(function(event) {
            if (!event.ctrlKey && !event.metaKey) {
                clearDefinitionPointer();
            }
        }));
        definitionPointerViews.push(view);
    }

    function detachDefinitionPointer(editor) {
        var view = findDefinitionPointerView(editor);
        if (!view) {
            return;
        }
        view.decoration.clear();
        view.listeners.forEach(function(listener) {
            listener.dispose();
        });
        definitionPointerViews.splice(definitionPointerViews.indexOf(view), 1);
    }

    function findDefinitionPointerView(editor) {
        for (var i = 0; i < definitionPointerViews.length; i++) {
            if (definitionPointerViews[i].editor === editor) {
                return definitionPointerViews[i];
            }
        }
        return null;
    }

    function clearDefinitionPointer() {
        definitionPointerViews.forEach(function(view) {
            view.decoration.clear();
        });
    }

    function addDefinitionPointerDomListener(target, type, listener) {
        target.addEventListener(type, listener, true);
        return {
            dispose: function() {
                target.removeEventListener(type, listener, true);
            }
        };
    }

    function isMacroPosition(position) {
        if (!position) {
            return false;
        }

        var uri = modelUri(getModel());
        var rangesByLine = uri && semanticMacroRanges[uri] || {};
        var ranges = rangesByLine[position.lineNumber] || [];
        return ranges.some(function(range) {
            return position.column >= range.startColumn && position.column < range.endColumn;
        });
    }

    function cacheMacroSemanticTokens(model, data) {
        var uri = modelUri(model);
        if (!uri) {
            return;
        }
        var rangesByLine = {};
        semanticMacroRanges[uri] = rangesByLine;
        var line = 0;
        var column = 0;
        for (var index = 0; index + 4 < data.length; index += 5) {
            line += data[index];
            column = data[index] === 0 ? column + data[index + 1] : data[index + 1];
            if (data[index + 3] === 14) { // "macro" in semanticTokenLegend
                var lineNumber = line + 1;
                rangesByLine[lineNumber] = rangesByLine[lineNumber] || [];
                rangesByLine[lineNumber].push({
                    startColumn: column + 1,
                    endColumn: column + data[index + 2] + 1
                });
            }
        }
    }

    function updateDefinitionPointer(view, position) {
        clearDefinitionPointer();
        var model = view.editor.getModel();
        if (!model) {
            return;
        }

        var word = model.getWordAtPosition(position);
        if (word) {
            view.decoration.set([{
                range: new monaco.Range(position.lineNumber, word.startColumn,
                    position.lineNumber, word.endColumn),
                options: {
                    description: "definition-pointer",
                    inlineClassName: "scinotes-definition-pointer"
                }
            }]);
        }
    }

    function detachDefinitionProvider() {
        definitionPointerViews.slice().forEach(function(view) {
            detachDefinitionPointer(view.editor);
        });
        definitionPointerListeners.forEach(function(listener) {
            listener.dispose();
        });
        definitionPointerListeners = [];
        if (definitionProvider) {
            definitionProvider.dispose();
            definitionProvider = null;
        }
        if (definitionEditorOpener) {
            definitionEditorOpener.dispose();
            definitionEditorOpener = null;
        }
    }

    function attachSemanticTokensProvider() {
        if (semanticTokensProvider || typeof monaco === "undefined" || !monaco.languages
                || typeof monaco.languages.registerDocumentSemanticTokensProvider !== "function") {
            return;
        }

        semanticTokensProvider = monaco.languages.registerDocumentSemanticTokensProvider(config.languageId || "scilab", {
            onDidChange: function(listener) {
                semanticTokensChangeListeners.push(listener);
                return {
                    dispose: function() {
                        var index = semanticTokensChangeListeners.indexOf(listener);
                        if (index >= 0) {
                            semanticTokensChangeListeners.splice(index, 1);
                        }
                    }
                };
            },
            getLegend: function() {
                return semanticTokenLegend;
            },
            provideDocumentSemanticTokens: function(model) {
                if (!isModelOpened(model)) {
                    return null;
                }

                var uri = modelUri(model);
                var modelVersion = model.getVersionId();
                var modelLines = model.getLinesContent();

                return requestPromise("textDocument/semanticTokens/full", {
                    textDocument: {
                        uri: uri
                    }
                }).then(function(result) {
                    // Normalize against the same text snapshot as the request.
                    // Monaco applies edits made while the request was pending
                    // before installing the returned semantic tokens.
                    var data = normalizeSemanticTokenData(result, modelLines);
                    if (isModelOpened(model) && model.getVersionId() === modelVersion) {
                        cacheMacroSemanticTokens(model, data);
                    }
                    updateDebug({ lastSemanticTokens: data.length / 5 });
                    return {
                        data: new Uint32Array(data),
                        resultId: null
                    };
                }, function(error) {
                    if (error && error.code === -32802) {
                        // Keep this as an error: Monaco retains and adjusts the
                        // semantic tokens already installed in the model.  A
                        // successful response containing old absolute ranges
                        // would overwrite that native adjustment.
                        throw error;
                    }
                    delete semanticMacroRanges[uri];
                    return {
                        data: new Uint32Array(0),
                        resultId: null
                    };
                });
            },
            releaseDocumentSemanticTokens: function() { }
        });

        updateDebug({ semanticTokensProviderAttached: true });
    }

    function detachSemanticTokensProvider() {
        if (semanticTokensProvider) {
            semanticTokensProvider.dispose();
            semanticTokensProvider = null;
            semanticTokensChangeListeners = [];
            semanticMacroRanges = {};
            updateDebug({ semanticTokensProviderAttached: false });
        }
    }

    function refreshSemanticTokens() {
        var listeners = semanticTokensChangeListeners.slice();
        for (var i = 0; i < listeners.length; i++) {
            listeners[i]();
        }
    }

    function attachHoverProvider() {
        if (hoverProvider || typeof monaco === "undefined" || !monaco.languages) {
            return;
        }

        registerScilabHelpCommand();
        attachHoverContextMenuBlocker();
        hoverProvider = monaco.languages.registerHoverProvider(config.languageId || "scilab", {
            provideHover: function(model, position) {
                if (!isModelOpened(model)) {
                    return null;
                }

                if (typeof limitHoverHeight === "function") {
                    var editors = window.SciNotesMonacoState;
                    limitHoverHeight(editors.editor);
                    limitHoverHeight(editors.secondaryEditor);
                }
                return requestPromise("textDocument/hover", {
                    textDocument: {
                        uri: modelUri(model)
                    },
                    position: {
                        line: position.lineNumber - 1,
                        character: position.column - 1
                    }
                }).then(function(result) {
                    refreshSemanticTokens();
                    if (!result || !result.contents) {
                        return null;
                    }

                    var contents = convertMarkupContent(
                        colorHelpSyntax(result.contents, model, position));
                    if (!contents) {
                        return null;
                    }
                    if (!Array.isArray(contents)) {
                        contents = [contents];
                    }
                    if (contents.length === 0) {
                        return null;
                    }

                    var hover = { contents: contents };
                    var range = convertRange(result.range);
                    if (range) {
                        hover.range = range;
                    }
                    return hover;
                }, function() {
                    refreshSemanticTokens();
                    return null;
                });
            }
        });
    }

    function detachHoverProvider() {
        if (hoverProvider) {
            hoverProvider.dispose();
            hoverProvider = null;
        }
        detachHoverContextMenuBlocker();
    }

    function initialize() {
        initializeRequestId = request("initialize", {
            processId: null,
            // The server cds to a root path: SciNotes shares the console's
            // Scilab, whose current directory is the user's.
            rootPath: null,
            capabilities: {
                textDocument: {
                    completion: {
                        completionItem: {
                            snippetSupport: true
                        }
                    }
                },
                workspace: {
                    semanticTokens: {
                        refreshSupport: true
                    }
                }
            }
        });
    }

    function connect() {
        if (!config || !config.url || !state.editor) {
            return;
        }

        socket = new WebSocket(config.url);
        updateDebug({
            url: config.url,
            documentUri: config.documentUri || null,
            modelLanguageId: getModel() && getModel().getLanguageId ? getModel().getLanguageId() : null,
            initialized: false,
            opened: false,
            connected: false
        });
        socket.onopen = function() {
            reconnectAttempts = 0;
            reconnectDelay = 250;
            initialized = false;
            updateDebug({ connected: true, initialized: false, opened: false });
            debugLog("socket open");
            initialize();
        };
        socket.onmessage = function(event) {
            var message;
            try {
                message = JSON.parse(event.data);
            } catch (e) {
                return;
            }

            if (message.id !== undefined && pendingRequests[message.id]) {
                var pending = pendingRequests[message.id];
                delete pendingRequests[message.id];
                if (message.error) {
                    pending.reject(message.error);
                } else {
                    pending.resolve(message.result);
                }
                return;
            }

            if (message.method === "textDocument/publishDiagnostics") {
                applyDiagnostics(message.params);
                return;
            }

            if (message.method === "workspace/semanticTokens/refresh") {
                refreshSemanticTokens();
                if (message.id !== undefined) {
                    send({
                        jsonrpc: "2.0",
                        id: message.id,
                        result: null
                    });
                }
                return;
            }

            if (message.id === initializeRequestId && message.result) {
                initialized = true;
                updateDebug({ initialized: true });
                debugLog("initialized", message.result.capabilities || null);
                notify("initialized", {});
                Object.keys(documents).forEach(function(uri) {
                    openDocument(documents[uri].model);
                });
                attachHoverProvider();
                attachCompletionProvider();
                attachSignatureHelpProvider();
                attachDocumentSymbolProvider();
                attachFoldingRangeProvider();
                attachFormattingProviders();
                attachDefinitionProvider();
                attachSemanticTokensProvider();
            }
        };
        socket.onclose = function() {
            clearDiagnostics();
            Object.keys(documents).forEach(function(uri) {
                documents[uri].opened = false;
            });
            detachHoverProvider();
            detachCompletionProvider();
            detachSignatureHelpProvider();
            detachDocumentSymbolProvider();
            detachFoldingRangeProvider();
            detachFormattingProviders();
            detachDefinitionProvider();
            detachSemanticTokensProvider();
            detachScilabHelpCommand();
            initialized = false;
            updateDebug({ connected: false, initialized: false });
            debugLog("socket closed");
            scheduleReconnect();
        };
        socket.onerror = function() {
            debugLog("socket error");
            if (socket) {
                socket.close();
            }
        };
    }

    function scheduleReconnect() {
        if (reconnectTimer || reconnectAttempts >= maxReconnectAttempts) {
            return;
        }

        reconnectAttempts++;
        reconnectTimer = setTimeout(function() {
            reconnectTimer = null;
            reconnectDelay = Math.min(reconnectDelay * 2, 2000);
            connect();
        }, reconnectDelay);
    }

    return {
        start: function() {
            attachModelListeners();
            connect();
        },
        // The split's second view, mounted and disposed by monaco-split.js.
        attachEditorView: function(editor) {
            attachDefinitionPointer(editor);
        },
        detachEditorView: function(editor) {
            detachDefinitionPointer(editor);
        },
        dispose: function() {
            if (reconnectTimer) {
                clearTimeout(reconnectTimer);
                reconnectTimer = null;
            }
            detachHoverProvider();
            detachCompletionProvider();
            detachSignatureHelpProvider();
            detachDocumentSymbolProvider();
            detachFoldingRangeProvider();
            detachFormattingProviders();
            detachDefinitionProvider();
            detachSemanticTokensProvider();
            detachScilabHelpCommand();
            if (socket && socket.readyState === WebSocket.OPEN) {
                Object.keys(documents).forEach(function(uri) {
                    closeDocument(documents[uri].model);
                });
            }
            detachModelListeners();
            Object.keys(documents).forEach(function(uri) {
                var document = documents[uri];
                if (document.listener) {
                    document.listener.dispose();
                }
            });
            documents = {};
            pendingRequests = {};
            if (socket) {
                socket.close();
                socket = null;
            }
        }
    };
}

function configureLsp(config) {
    var state = window.SciNotesMonacoState;
    state.pendingLspConfig = config || null;

    if (state.lspClient) {
        state.lspClient.dispose();
        state.lspClient = null;
    }

    if (!config || !state.editor) {
        return;
    }

    // Monaco creates the first model before Java can provide the document URI.
    // Give that bootstrap model its stable URI once; additional window models
    // are created directly with their final URI by monaco-documents.js.
    var currentModel = state.editor.getModel();
    if (currentModel && config.documentUri
            && currentModel.uri.toString() !== config.documentUri
            && monaco.editor.getModels().length === 1) {
        var replacement = monaco.editor.createModel(currentModel.getValue(),
            currentModel.getLanguageId(), monaco.Uri.parse(config.documentUri));
        replacement.setEOL(monaco.editor.EndOfLineSequence.LF);
        // Its edits must reach the Java replica, as for any document model.
        if (typeof installDocumentModel === "function") {
            installDocumentModel(replacement);
        }
        state.editor.setModel(replacement);
        if (state.activeDocumentId && state.documents[state.activeDocumentId]) {
            state.documents[state.activeDocumentId].model = replacement;
            state.documents[state.activeDocumentId].uri = config.documentUri;
            state.documents[state.activeDocumentId].savedAlternativeVersionId =
                replacement.getAlternativeVersionId();
        }
        if (typeof syncSplitModelReplacement === "function") {
            syncSplitModelReplacement(currentModel, replacement);
        }
        currentModel.dispose();
        updateSavedAlternativeVersion();
    }

    state.lspClient = createSciNotesLspClient(config);
    state.lspClient.start();
}

window.SciNotesMonacoLspDebug = function() {
    return window.SciNotesMonacoState && window.SciNotesMonacoState.lspDebug || {};
};
