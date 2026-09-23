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
 * Adds the LaTeX preview historically provided by SciNotes to Monaco. The
 * browser detects $...$ expressions and uses Scilab's bundled MathJax to
 * produce an SVG data URI for the standard Monaco hover.
 */

function configureLatexPreview() {
    if (typeof monaco === "undefined" || !monaco.languages) {
        return;
    }

    monaco.languages.registerHoverProvider("scilab", {
        provideHover: function(model, position) {
            var expression = latexExpressionAt(model, position);
            if (!expression) {
                return null;
            }

            return renderLatexSvg(expression.value).then(function(svg) {
                var imageUri = latexSvgDataUri(svg);
                if (!imageUri) {
                    return null;
                }

                return {
                    range: new monaco.Range(position.lineNumber, expression.start + 1,
                        position.lineNumber, expression.end + 1),
                    contents: [{
                        value: "![" + escapeMarkdownAlt(expression.value) + "](" + imageUri + ")"
                    }]
                };
            });
        }
    });
    configurePersistentLatexPreview(window.SciNotesMonacoState.editor);
}

function latexExpressionAt(model, position) {
    var line = model.getLineContent(position.lineNumber);
    var offset = position.column - 1;
    var tokenLines = monaco.editor.tokenize(model.getValue(), "scilab");
    var tokens = tokenLines[position.lineNumber - 1] || [];
    for (var index = 0; index < tokens.length; index++) {
        var token = tokens[index];
        var end = index + 1 < tokens.length ? tokens[index + 1].offset : line.length;
        if (token.offset <= offset && offset < end
                && (token.type.indexOf("latex.comment") === 0
                    || token.type.indexOf("latex.string") === 0)) {
            var text = line.substring(token.offset, end);
            var opening = text.indexOf("$");
            var closing = text.lastIndexOf("$");
            if (opening < 0 || closing <= opening) {
                return null;
            }
            return {
                start: token.offset + opening,
                end: token.offset + closing + 1,
                value: text.substring(opening + 1, closing)
            };
        }
    }
    return null;
}

function renderLatexSvg(expression) {
    if (typeof MathJax === "undefined" || !MathJax.startup) {
        return Promise.resolve("");
    }

    return MathJax.startup.promise.then(function() {
        MathJax.texReset();
        return MathJax.tex2svgPromise(expression, { display: false });
    }).then(function(node) {
        var svg = node.querySelector("svg");
        if (!svg) {
            return "";
        }
        var result = svg.outerHTML;
        MathJax.startup.document.clear();
        MathJax.startup.document.updateDocument();
        return result;
    }).catch(function() {
        return "";
    });
}

function latexSvgDataUri(svg) {
    if (!svg) {
        return "";
    }
    return "data:image/svg+xml;base64,"
        + window.btoa(unescape(encodeURIComponent(svg)));
}

function configurePersistentLatexPreview(editor) {
    if (!editor) {
        return;
    }

    var widget = createLatexWidget(editor);
    var decoration = editor.createDecorationsCollection();
    var renderTimer = 0;
    var renderVersion = 0;

    function closePreview() {
        renderVersion++;
        if (renderTimer) {
            clearTimeout(renderTimer);
            renderTimer = 0;
        }
        decoration.clear();
        widget.hide();
        editor.layoutContentWidget(widget);
    }

    function renderTrackedExpression() {
        renderTimer = 0;
        var version = ++renderVersion;
        var range = decoration.length ? decoration.getRange(0) : null;
        if (!range) {
            closePreview();
            return;
        }

        var text = editor.getModel().getValueInRange(range);
        var firstDollar = text.indexOf("$");
        var lastDollar = text.lastIndexOf("$");
        if (firstDollar < 0 || lastDollar <= firstDollar) {
            closePreview();
            return;
        }

        renderLatexSvg(text.substring(firstDollar + 1, lastDollar)).then(function(svg) {
            if (!decoration.length || version !== renderVersion) {
                return;
            }
            widget.setPreview(svg);
            editor.layoutContentWidget(widget);
        });
    }

    function scheduleRender() {
        if (renderTimer) {
            clearTimeout(renderTimer);
        }
        renderTimer = setTimeout(renderTrackedExpression, 150);
    }

    function openPreview(expression, lineNumber) {
        decoration.set([{
            range: new monaco.Range(lineNumber, expression.start + 1,
                lineNumber, expression.end + 1),
            options: {
                stickiness: monaco.editor.TrackedRangeStickiness.AlwaysGrowsWhenTypingAtEdges
            }
        }]);
        widget.show();
        widget.setPosition(lineNumber, expression.end + 1);
        editor.layoutContentWidget(widget);
        renderTrackedExpression();
    }

    widget.onClose(closePreview);
    editor.addContentWidget(widget);
    editor.onMouseDown(function(event) {
        if (!event.target.position
                || event.event.rightButton
                || event.event.middleButton) {
            return;
        }
        var expression = latexExpressionAt(editor.getModel(), event.target.position);
        if (expression) {
            openPreview(expression, event.target.position.lineNumber);
        }
    });
    editor.onDidChangeModelContent(function() {
        if (decoration.length) {
            var range = decoration.getRange(0);
            if (range) {
                widget.setPosition(range.endLineNumber, range.endColumn);
            }
            scheduleRender();
        }
    });
    editor.onKeyDown(function(event) {
        if (decoration.length && event.keyCode === monaco.KeyCode.Escape) {
            event.preventDefault();
            event.stopPropagation();
            closePreview();
        }
    });
}

function createLatexWidget(editor) {
    var domNode = document.createElement("div");
    domNode.className = "scinotes-latex-widget";

    var preview = document.createElement("div");
    preview.className = "scinotes-latex-widget-preview";
    domNode.appendChild(preview);

    var close = document.createElement("button");
    close.className = "scinotes-latex-widget-close";
    close.type = "button";
    close.title = "Close LaTeX preview";
    close.textContent = "\u00d7";
    domNode.appendChild(close);

    var position = null;
    var closeHandler = function() {};
    close.addEventListener("click", function(event) {
        event.preventDefault();
        event.stopPropagation();
        closeHandler();
        editor.focus();
    });

    return {
        getId: function() {
            return "scinotes.latex.preview";
        },
        getDomNode: function() {
            return domNode;
        },
        getPosition: function() {
            return position ? {
                position: position,
                preference: [
                    monaco.editor.ContentWidgetPositionPreference.BELOW,
                    monaco.editor.ContentWidgetPositionPreference.ABOVE
                ]
            } : null;
        },
        setPosition: function(lineNumber, column) {
            position = { lineNumber: lineNumber, column: column };
        },
        setPreview: function(svg) {
            preview.innerHTML = svg || "";
        },
        show: function() {
            domNode.classList.add("visible");
        },
        hide: function() {
            domNode.classList.remove("visible");
            preview.textContent = "";
            position = null;
        },
        onClose: function(handler) {
            closeHandler = handler;
        }
    };
}

function escapeMarkdownAlt(value) {
    return String(value || "").replace(/[\[\]\\\\]/g, "\\$&");
}
