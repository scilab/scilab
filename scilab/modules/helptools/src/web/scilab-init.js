/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 * Scilab Help Browser - highlight.js initializer
 *
 * In JCEF mode, fetches the keyword lists of getscilabkeywords() via the
 * bridge and adds them to the stock grammar:
 *   - commands (if, for, function...) -> keyword (Structure color)
 *   - predefined variables (%pi, SCI...) -> literal (Constantes color)
 *   - primitives (C-level) -> built_in (Primitive color)
 *   - macros (Scilab-level, loaded toolboxes) -> type (Macro color)
 * and the syntax highlighting colors of Preferences > Colors (shared with
 * SciNotes), applied as CSS variables read by scilab.min.css. They are
 * re-applied live when the preferences change ("syntaxColorsChanged").
 * In standalone mode, highlights with the stock grammar and theme only.
 */
(function () {
    "use strict";

    /**
     * Preferences > Colors items applied to the page. The value is the CSS
     * variable prefix used by scilab.min.css; each item sets
     * --sci-<prefix>-color / -weight / -style. Underline / strike-through
     * are ignored: underlined code would read as links in a help page.
     */
    var PREF_ITEMS = {
        "Default": "default",
        "Structure": "structure",       // hljs-keyword (Control / FunctionKwd need a finer grammar)
        "Primitive": "primitive",       // hljs-built_in
        "Macro": "macro",               // hljs-type / hljs-meta
        "MacroInFile": "macroinfile",   // hljs-title
        "InputOutputArgs": "ioargs",    // hljs-params
        "Constantes": "constantes",     // hljs-literal
        "Number": "number",
        "String": "string",
        "Comment": "comment",
        "Operator": "operator",
        "OpenClose": "openclose",       // hljs-punctuation
        "Special": "special",           // hljs-symbol
        "Identifier": "identifier"      // hljs-variable
    };

    function applySyntaxColors(response) {
        var colors;
        try {
            colors = JSON.parse(response);
        } catch (e) {
            return;
        }
        var root = document.documentElement.style;
        for (var name in PREF_ITEMS) {
            var prefix = "--sci-" + PREF_ITEMS[name] + "-";
            var item = colors[name];
            if (item && /^#[0-9a-fA-F]{6}$/.test(item.color)) {
                root.setProperty(prefix + "color", item.color);
                root.setProperty(prefix + "weight", item.bold ? "bold" : "normal");
                root.setProperty(prefix + "style", item.italic ? "italic" : "normal");
            } else {
                root.removeProperty(prefix + "color");
                root.removeProperty(prefix + "weight");
                root.removeProperty(prefix + "style");
            }
        }
    }

    function fetchSyntaxColors(bridge, done) {
        bridge({
            request: JSON.stringify({type: "getSyntaxColors"}),
            onSuccess: function (response) {
                if (response) applySyntaxColors(response);
                if (done) done();
            },
            onFailure: function () {
                if (done) done();
            }
        });
    }

    function compareVersions(a, b) {
        var pa = a.split(".").map(Number);
        var pb = b.split(".").map(Number);
        for (var i = 0; i < Math.max(pa.length, pb.length); i++) {
            var na = pa[i] || 0;
            var nb = pb[i] || 0;
            if (na !== nb) return na - nb;
        }
        return 0;
    }

    /**
     * Syntax keywords of the stock grammar (control flow, function...). In
     * highlight.js the last category listing a name wins, so the primitives
     * of getscilabkeywords() would turn these into functions (pause, abort,
     * resume, clear, exit... are also gateways). They stay keywords, like
     * the control keywords of SciNotes; the other overlaps (clc, pwd, what,
     * who) are colored as primitives, as in SciNotes.
     */
    var stockKeywords = null;

    function enrichGrammar(category, names) {
        if (typeof hljs === "undefined") return;
        var lang = hljs.getLanguage("scilab");
        if (!lang || !lang.keywords) return;
        if (stockKeywords === null) {
            stockKeywords = " " + (typeof lang.keywords.keyword === "string" ? lang.keywords.keyword : "") + " ";
        }
        if (category !== "keyword") {
            names = names.split(" ").filter(function (n) {
                return n !== "" && stockKeywords.indexOf(" " + n + " ") < 0;
            }).join(" ");
            if (names === "") return;
        }
        if (typeof lang.keywords[category] === "string") {
            lang.keywords[category] += " " + names;
        } else {
            lang.keywords[category] = names;
        }
    }

    /**
     * InputOutputArgs in function bodies, as in SciNotes: a name used in a
     * function body that is an output or input of the innermost enclosing
     * function gets the color of the header arguments. highlight.js does not
     * track scopes, so this runs on its output: the block structure is
     * rebuilt from the keyword tokens (an "end" inside brackets is an index,
     * a(end)) and the names of the plain text are wrapped. Keywords, strings,
     * comments, functions... keep their own color, as in SciNotes.
     */
    var BLOCK_OPENERS = {"if": 1, "for": 1, "while": 1, "select": 1, "switch": 1, "try": 1};
    var CLASSDEF_SECTIONS = {"properties": 1, "methods": 1, "enumeration": 1};
    var IDENTIFIER = /[a-zA-Z_%#!$?][\w#!$?]*/g;

    function colorFunctionArgs(code) {
        var stack = [];   // open blocks: {kind: "function" | "classdef" | "block", args}
        var depth = 0;    // bracket depth of the plain text
        var prev = "";    // last character before the current token

        function functionArgs() {
            for (var i = stack.length - 1; i >= 0; i--) {
                if (stack[i].kind === "function") return stack[i].args;
            }
            return null;
        }

        function popFunction() {
            while (stack.length && stack.pop().kind !== "function") { /* inner blocks */ }
        }

        function text(node) {
            var s = node.nodeValue;
            var args = functionArgs();
            if (args) {
                var frag = null, last = 0, m;
                IDENTIFIER.lastIndex = 0;
                while ((m = IDENTIFIER.exec(s)) !== null) {
                    var before = m.index > 0 ? s.charAt(m.index - 1) : prev;
                    if (!args.hasOwnProperty(m[0]) || /[\w#!$?%.]/.test(before)) continue;
                    frag = frag || document.createDocumentFragment();
                    frag.appendChild(document.createTextNode(s.substring(last, m.index)));
                    var span = document.createElement("span");
                    span.className = "hljs-params";
                    span.textContent = m[0];
                    frag.appendChild(span);
                    last = m.index + m[0].length;
                }
                if (frag) {
                    frag.appendChild(document.createTextNode(s.substring(last)));
                    node.parentNode.replaceChild(frag, node);
                }
            }
            for (var i = 0; i < s.length; i++) {
                var c = s.charAt(i);
                if (c === "(" || c === "[" || c === "{") depth++;
                else if ((c === ")" || c === "]" || c === "}") && depth > 0) depth--;
            }
            if (s) prev = s.slice(-1);
        }

        function keyword(el) {
            var word = el.textContent;
            var top = stack.length ? stack[stack.length - 1] : null;
            if (BLOCK_OPENERS[word]) {
                stack.push({kind: "block"});
            } else if (word === "classdef") {
                stack.push({kind: "classdef"});
            } else if (CLASSDEF_SECTIONS[word] && top && top.kind === "classdef") {
                stack.push({kind: "block"});
            } else if (word === "arguments") {
                // a block only when alone on its line (arguments(...) is a call)
                var next = el.nextSibling;
                if (!next || (next.nodeType === 3 && /^[ \t]*(\n|$|\/\/)/.test(next.nodeValue))) {
                    stack.push({kind: "block"});
                }
            } else if (word === "end" && depth === 0) {
                stack.pop();
            } else if (word === "endfunction") {
                popFunction();
            }
        }

        function walk(parent) {
            var child = parent.firstChild;
            while (child) {
                var next = child.nextSibling; // text() may replace child
                if (child.nodeType === 3) {
                    text(child);
                } else if (child.nodeType === 1) {
                    var cls = child.classList;
                    if (cls.contains("hljs-function")) {
                        var args = {};
                        var params = child.querySelectorAll(".hljs-params");
                        for (var i = 0; i < params.length; i++) {
                            args[params[i].textContent] = true;
                        }
                        stack.push({kind: "function", args: args});
                    } else if (cls.contains("hljs-keyword")) {
                        keyword(child);
                    } else if (!/(^|\s)hljs-/.test(child.className)) {
                        walk(child);
                    }
                    if (child.textContent) prev = child.textContent.slice(-1);
                }
                child = next;
            }
        }

        walk(code);
    }

    /**
     * Strip common leading whitespace from <pre> blocks.
     */
    function dedentPreBlocks() {
        var pres = document.querySelectorAll("pre.synopsis, pre.programlisting, pre.screen");
        for (var i = 0; i < pres.length; i++) {
            var text = pres[i].textContent;
            var lines = text.split("\n");
            // Remove leading/trailing empty lines
            while (lines.length && lines[0].trim() === "") lines.shift();
            while (lines.length && lines[lines.length - 1].trim() === "") lines.pop();
            if (lines.length === 0) continue;
            // Remove trailing whitespace on each line
            for (var j = 0; j < lines.length; j++) {
                lines[j] = lines[j].replace(/\s+$/, "");
            }
            // Synopsis: trim each line (no relative indent to preserve)
            // Code blocks: dedent by common minimum
            var isSynopsis = pres[i].classList.contains("synopsis");
            if (isSynopsis) {
                for (var j = 0; j < lines.length; j++) {
                    lines[j] = lines[j].replace(/^\s+/, "");
                }
            } else {
                var minIndent = Infinity;
                for (var j = 0; j < lines.length; j++) {
                    if (lines[j] === "") continue;
                    var indent = lines[j].match(/^(\s*)/)[1].length;
                    if (indent < minIndent) minIndent = indent;
                }
                if (minIndent > 0 && minIndent < Infinity) {
                    for (var j = 0; j < lines.length; j++) {
                        lines[j] = lines[j].substring(minIndent);
                    }
                }
            }
            pres[i].textContent = lines.join("\n");
        }
    }

    function highlight() {
        dedentPreBlocks();
        // Highlighting is optional: keep going if highlight.js failed to load
        if (typeof hljs !== "undefined") {
            // Wrap content in <code> for hljs (it requires pre > code)
            var pres = document.querySelectorAll("pre.language-scilab");
            for (var i = 0; i < pres.length; i++) {
                var code = document.createElement("code");
                code.className = "language-scilab";
                code.textContent = pres[i].textContent;
                pres[i].textContent = "";
                pres[i].appendChild(code);
            }
            hljs.addPlugin({
                "after:highlightElement": function (data) {
                    if (data.result && data.result.language === "scilab") {
                        colorFunctionArgs(data.el);
                    }
                }
            });
            hljs.highlightAll();
        }

        // Sort revision history by version number (descending)
        var revTables = document.querySelectorAll("table.revhistory");
        for (var i = 0; i < revTables.length; i++) {
            var rows = Array.from(revTables[i].querySelectorAll("tr:not(.title)"));
            if (rows.length < 2) continue;
            rows.sort(function (a, b) {
                var va = (a.querySelector(".revnumber") || {}).textContent || "";
                var vb = (b.querySelector(".revnumber") || {}).textContent || "";
                return compareVersions(vb, va); // descending
            });
            rows.forEach(function (row) { revTables[i].appendChild(row); });
        }

        // Convert serialized MathML spans into real MathML for MathJax
        var mathSpans = document.querySelectorAll(".math-mml");
        for (var i = 0; i < mathSpans.length; i++) {
            var mml = mathSpans[i].textContent;
            // Wrap in <math> with namespace
            var wrapper = document.createElement("span");
            wrapper.innerHTML = '<math xmlns="http://www.w3.org/1998/Math/MathML">' + mml + '</math>';
            mathSpans[i].parentNode.replaceChild(wrapper, mathSpans[i]);
        }
    }

    // Find the bridge: either directly available or via parent frame (iframe context)
    var bridge = (typeof helpBrowserQuery === "function") ? helpBrowserQuery
               : (parent && typeof parent.helpBrowserQuery === "function") ? parent.helpBrowserQuery
               : null;

    if (bridge) {
        // Colors first (a plain CSS variable update, no re-highlight needed),
        // then keywords, then highlight
        fetchSyntaxColors(bridge, function () {
            bridge({
                request: JSON.stringify({type: "getScilabKeywords"}),
                onSuccess: function (response) {
                    if (response) {
                        try {
                            var data = JSON.parse(response);
                            // Later categories win in highlight.js: a name
                            // that is both a command and a primitive (clc,
                            // pwd...) is colored as a primitive
                            if (data.keyword) enrichGrammar("keyword", data.keyword);
                            if (data.literal) enrichGrammar("literal", data.literal);
                            if (data.built_in) enrichGrammar("built_in", data.built_in);
                            if (data.type) enrichGrammar("type", data.type);
                        } catch (e) {
                            enrichGrammar("built_in", response);
                        }
                    }
                    highlight();
                },
                onFailure: function () {
                    highlight();
                }
            });
        });

        // Shell -> page: Preferences > Colors changed while the page is displayed
        window.addEventListener("message", function (e) {
            if (e.data && e.data.type === "syntaxColorsChanged") {
                fetchSyntaxColors(bridge, null);
            }
        });
    } else {
        highlight();
    }
})();
