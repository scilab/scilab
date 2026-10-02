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
 * This file registers the lightweight Scilab language definition used by
 * Monaco before the LSP integration is connected. It provides tokenization and
 * editor language metadata, not semantic analysis.
 */

/*
 * SciNotes owns the extensions of the Scilab language (see
 * SciNotes.getScilabExtensions()), so that one list decides both the language
 * a model gets here and the documents the host indexes. It sends them once
 * the page has signalled monacoReady, that is after registerScilabLanguage()
 * below; Monaco merges this second registration into the language already
 * declared. Only the initial model is created in between, and it is given the
 * Scilab language explicitly rather than by extension.
 */
window.SciNotesMonacoRegisterScilabExtensions = function(extensions) {
    monaco.languages.register({
        id: "scilab",
        extensions: Array.isArray(extensions) ? extensions : []
    });
};

function registerScilabLanguage() {
    var blockOpenPattern = /^\s*\b(function|if|else|elseif|select|case|otherwise|for|while|try|catch|arguments|classdef|properties|methods|enumeration)\b(?!.*\.\.\.\s*$).*$/;
    var increaseIndentPattern = blockOpenPattern;
    var decreaseIndentPattern = /^\s*\b(end\w*|catch|else|elseif|case|otherwise)\b/;

    monaco.languages.register({
        id: "scilab",
        aliases: ["Scilab", "scilab"]
    });
    monaco.languages.setLanguageConfiguration("scilab", {
        // Keep Scilab identifiers such as %duration, $foo and names using
        // !, # or ? as a single word for completion, hover and navigation.
        wordPattern: /[%$A-Za-z_!#?\u0080-\uFFFF][A-Za-z_0-9!#?$\u0080-\uFFFF]*/g,
        comments: {
            lineComment: "//",
            blockComment: ["/*", "*/"]
        },
        brackets: [
            ["{", "}"],
            ["[", "]"],
            ["(", ")"]
        ],
        autoClosingPairs: [
            { open: "{", close: "}" },
            { open: "[", close: "]" },
            { open: "(", close: ")" },
            { open: "\"", close: "\"" },
            { open: "'", close: "'" }
        ],
        surroundingPairs: [
            { open: "{", close: "}" },
            { open: "[", close: "]" },
            { open: "(", close: ")" },
            { open: "\"", close: "\"" },
            { open: "'", close: "'" }
        ],
        indentationRules: {
            increaseIndentPattern: increaseIndentPattern,
            decreaseIndentPattern: decreaseIndentPattern,
            indentNextLinePattern: blockOpenPattern
        },
        onEnterRules: [
            {
                beforeText: blockOpenPattern,
                action: { indentAction: monaco.languages.IndentAction.Indent }
            }
        ]
    });
    registerScilabTokenizer();
    registerScilabPrototypeLanguage("scilab-proto", "function.defaultLibrary");
    registerScilabPrototypeLanguage("scilab-proto-macro", "macro");
    defineScilabTheme();
}

function defineScilabTheme(themeConfig) {
    var theme = normalizeScilabTheme(themeConfig);
    var colors = theme.colors;
    var styles = theme.styles;
    var ui = theme.ui;
    monaco.editor.defineTheme("scinotes-light", {
        base: "vs",
        inherit: true,
        semanticHighlighting: theme.semanticHighlighting,
        rules: [
            tokenRule("", colors.defaultText, styles.defaultText),
            tokenRule("keyword", colors.structure, styles.structure),
            tokenRule("keyword.definition", colors.functionKeyword, styles.functionKeyword),
            tokenRule("keyword.function", colors.functionKeyword, styles.functionKeyword),
            tokenRule("keyword.control", colors.control, styles.control),
            tokenRule("keyword.structure", colors.structure, styles.structure),
            tokenRule("functionKeyword", colors.functionKeyword, styles.functionKeyword),
            tokenRule("functionKeyword.definition", colors.functionKeyword, styles.functionKeyword),
            tokenRule("support.function", colors.primitive, styles.primitive),
            tokenRule("type.identifier", colors.inputOutputArgs, styles.inputOutputArgs),
            tokenRule("constant", colors.constants, styles.constants),
            tokenRule("number", colors.number, styles.number),
            tokenRule("string", colors.string, styles.string),
            tokenRule("string.escape", colors.string, styles.string),
            tokenRule("string.invalid", colors.string, styles.string),
            tokenRule("comment", colors.comment, styles.comment),
            tokenRule("comment.author", colors.author, styles.author),
            tokenRule("comment.todo", colors.todo, styles.todo),
            tokenRule("url", colors.url, mergeFontStyle(styles.url, "underline")),
            tokenRule("mail", colors.mail, mergeFontStyle(styles.mail, "underline")),
            tokenRule("latex.comment", colors.latexComment, styles.latexComment),
            tokenRule("latex.string", colors.latexString, styles.latexString),
            tokenRule("operator", colors.operator, styles.operator),
            tokenRule("operator.special", colors.special, styles.special),
            tokenRule("operatorSpecial", colors.special, styles.special),
            tokenRule("controlKeyword", colors.control, styles.control),
            tokenRule("delimiter", colors.openClose, styles.openClose),
            tokenRule("invalid", colors.functionKeyword, mergeFontStyle(styles.functionKeyword, "strikethrough")),
            tokenRule("function", colors.primitive, styles.primitive),
            tokenRule("function.defaultLibrary", colors.primitive, styles.primitive),
            tokenRule("function.deprecated", colors.functionKeyword, mergeFontStyle(styles.functionKeyword, "strikethrough")),
            tokenRule("function.defaultLibrary.deprecated", colors.functionKeyword, mergeFontStyle(styles.functionKeyword, "strikethrough")),
            tokenRule("method", colors.primitive, styles.primitive),
            tokenRule("macro", colors.macro, mergeFontStyle(styles.macro, "underline")),
            // A macro declaration is the LSP equivalent of SciNotes' MacroInFile token.
            tokenRule("macro.definition", colors.macroInFile, mergeFontStyle(styles.macroInFile, "underline")),
            tokenRule("macro.inFile", colors.macroInFile, mergeFontStyle(styles.macroInFile, "underline")),
            tokenRule("macro.deprecated", colors.functionKeyword, mergeFontStyle(styles.functionKeyword, "underline strikethrough")),
            tokenRule("variable", colors.identifier, styles.identifier),
            tokenRule("variable.readonly", colors.constants, styles.constants),
            tokenRule("variable.readonly.scilabRuntime", colors.constants, styles.constants),
            tokenRule("variable.deprecated", colors.functionKeyword, mergeFontStyle(styles.functionKeyword, "strikethrough")),
            tokenRule("parameter", colors.inputOutputArgs, styles.inputOutputArgs),
            tokenRule("property", colors.field, styles.field),
            tokenRule("identifier", colors.identifier, styles.identifier),
            // The "..." closing a help syntax cut short (registerScilabPrototypeLanguage).
            tokenRule("elision", colors.defaultText, mergeFontStyle(styles.defaultText, "bold"))
        ],
        colors: {
            "editor.background": "#" + ui.background,
            "editor.foreground": "#" + colors.defaultText,
            "editorCursor.foreground": "#" + ui.caret,
            // Framed rather than filled, as VS Code does.
            "editor.lineHighlightBackground": "#00000000",
            "editor.lineHighlightBorder": "#DCDCDC",
            "editorLineNumber.foreground": "#315B62",
            "editorLineNumber.activeForeground": "#B01813",
            "editor.selectionBackground": "#ADD6FF",
            "editor.inactiveSelectionBackground": "#E5EBF1",
            "editorWhitespace.foreground": "#DCDCDC"
        }
    });
}
function applyScilabTheme(themeConfig) {
    defineScilabTheme(themeConfig);
    monaco.editor.setTheme("scinotes-light");
}

function normalizeScilabTheme(themeConfig) {
    var defaults = {
        colors: {
            defaultText: "000000",
            special: "FFAA00",
            operator: "5C5C5C",
            primitive: "32B9B9",
            macro: "AE5CB0",
            macroInFile: "000000",
            string: "BC8F8F",
            inputOutputArgs: "834310",
            identifier: "000000",
            comment: "64AE64",
            field: "AAAAAA",
            functionKeyword: "B01813",
            url: "0000FF",
            mail: "0000FF",
            number: "BC8F8F",
            control: "5F9EA0",
            structure: "A020F0",
            constants: "DA70D6",
            openClose: "4A55DB",
            todo: "F46700",
            author: "64AE64",
            latexComment: "8B2252",
            latexString: "8B2252"
        },
        styles: {
            defaultText: "",
            comment: "italic",
            macro: "underline",
            inputOutputArgs: "bold"
        },
        ui: {
            background: "FFFFFF",
            caret: "000000"
        },
        syntaxHighlighting: true,
        semanticHighlighting: true
    };

    var config = themeConfig || {};
    var colors = {};
    var styles = {};
    var tokens = config.tokens || {};
    Object.keys(defaults.colors).forEach(function(name) {
        var token = tokens[name] || {};
        colors[name] = normalizeHex(token.foreground || defaults.colors[name], defaults.colors[name]);
        styles[name] = typeof token.fontStyle === "string" ? token.fontStyle : (defaults.styles[name] || "");
    });

    var ui = config.ui || {};
    var theme = {
        colors: colors,
        styles: styles,
        ui: {
            background: normalizeHex(ui.background || defaults.ui.background, defaults.ui.background),
            caret: normalizeHex(ui.caret || defaults.ui.caret, defaults.ui.caret)
        },
        syntaxHighlighting: config.syntaxHighlighting !== false,
        semanticHighlighting: config.semanticHighlighting !== false
    };

    if (!theme.syntaxHighlighting) {
        Object.keys(theme.colors).forEach(function(name) {
            theme.colors[name] = theme.colors.defaultText;
            theme.styles[name] = "";
        });
    }

    return theme;
}

function normalizeHex(value, fallback) {
    var hex = String(value || "").replace(/^#/, "").toUpperCase();
    return /^[0-9A-F]{6}$/.test(hex) ? hex : fallback;
}

function tokenRule(token, foreground, fontStyle) {
    var rule = { token: token, foreground: foreground };
    if (fontStyle) {
        rule.fontStyle = fontStyle;
    }
    return rule;
}

function mergeFontStyle(base, extra) {
    var styles = {};
    String(base || "").split(/\s+/).forEach(function(style) {
        if (style) {
            styles[style] = true;
        }
    });
    String(extra || "").split(/\s+/).forEach(function(style) {
        if (style) {
            styles[style] = true;
        }
    });
    return Object.keys(styles).join(" ");
}

var scilabKeywords = {
    functionKeywords: ["function", "endfunction"],
    controlKeywords: ["abort", "break", "quit", "return", "resume", "pause", "continue", "exit"],
    structureKeywords: ["then", "do", "catch", "case", "otherwise", "elseif", "else"],
    openCloseStructureKeywords: ["if", "for", "while", "try", "select", "switch", "arguments", "classdef", "enumeration", "properties", "methods", "end"]
};

function isScilabKeyword(word) {
    return Object.keys(scilabKeywords).some(function(group) {
        return scilabKeywords[group].indexOf(word) >= 0;
    });
}

function registerScilabTokenizer() {
    monaco.languages.setMonarchTokensProvider("scilab", {
        functionKeywords: scilabKeywords.functionKeywords,
        controlKeywords: scilabKeywords.controlKeywords,
        structureKeywords: scilabKeywords.structureKeywords,
        openCloseStructureKeywords: scilabKeywords.openCloseStructureKeywords,
        tokenizer: {
            root: [
                [/\t+/, "tab"],
                [/[ \f\v]+/, "white"],
                [/\bfunction\b/, "keyword.function", "@functionDeclaration"],
                // A whole string made of one LaTeX expression, doubled quotes included.
                [/"\$(?:\\\$|""|[^$"])+\$"/, "latex.string"],
                [/'\$(?:\\\$|''|[^$'])+\$'/, "latex.string"],
                [/\/\/\s*(TODO|todo|Todo)[ \t:]+.*$/, "comment.todo"],
                [/\/\/$/, "comment"],
                [/\/\//, "comment", "@lineComment"],
                [/\/\*/, "comment", "@blockComment"],
                [/((?:\b[0-9]+\.?[0-9]*|\.[0-9]+)(?:[eE][+\-]?[0-9]+)?\b)(')/, ["number", "operator"]],
                [/\b(([0-9]+\.?[0-9]*)|(\.[0-9]+))([eE][+\-]?[0-9]+)?\b/, "number"],
                [/(\.\*\.|\.\/\.|\.\\\.)/, "operator"],
                [/(\.\^|\.\/|\.\*|\.\\|\/\.|\\\.|\*\*|\.')/, "operator"],
                [/[$:]|\.{2,}/, "operator.special"],
                [/[+\-*\/\\^]/, "operator"],
                [/(==|~=|@=|<>|>=|<=|>|<|=)/, "operator"],
                [/(&&|\|\||&|\|)/, "operator"],
                [/\bend\b(?=\s*[\],)}])/, "operator.special"],
                [/([a-zA-Z_]\w*)(')/, [{
                    cases: {
                        "@functionKeywords": "keyword.function",
                        "@controlKeywords": "keyword.control",
                        "@structureKeywords": "keyword.structure",
                        "@openCloseStructureKeywords": "keyword.structure",
                        "@default": "identifier"
                    }
                }, "operator"]],
                [/[a-zA-Z_]\w*/, {
                    cases: {
                        "@functionKeywords": "keyword.function",
                        "@controlKeywords": "keyword.control",
                        "@structureKeywords": "keyword.structure",
                        "@openCloseStructureKeywords": "keyword.structure",
                        "@default": "identifier"
                    }
                }],
                [/([%!][a-zA-Z_]\w*)(')/, [{
                    cases: {
                        "@default": "identifier"
                    }
                }, "operator"]],
                [/[%!][a-zA-Z_]\w*/, {
                    cases: {
                        "@default": "identifier"
                    }
                }],
                [/"(?:[^"]|"")*$/, "string.invalid"],
                [/"/, "string", "@string"],
                [/([\]\)}])(')/, ["delimiter", "operator"]],
                [/'(?:[^']|'')*$/, "string.invalid"],
                [/'/, "string", "@singleString"],
                [/\.\.\.\s*$/, "delimiter"],
                [/[;,.()[\]{}]/, "delimiter"]
            ],
            blockComment: [
                [/\$(?:\\\$|[^$])+\$/, "latex.comment"],
                [/\*\//, "comment", "@pop"],
                [/[^$*/]+/, "comment"],
                [/[$*/]/, "comment"]
            ],
            lineComment: [
                [/\$(?:\\\$|[^$])+\$/, {
                    cases: {
                        "@eos": { token: "latex.comment", next: "@pop" },
                        "@default": "latex.comment"
                    }
                }],
                [/[^$]+/, {
                    cases: {
                        "@eos": { token: "comment", next: "@pop" },
                        "@default": "comment"
                    }
                }],
                [/\$/, {
                    cases: {
                        "@eos": { token: "comment", next: "@pop" },
                        "@default": "comment"
                    }
                }]
            ],
            functionDeclaration: [
                [/\s+/, "white"],
                [/\[/, "delimiter", "@functionOutputs"],
                [/([%!]?[a-zA-Z_]\w*)(\s*=\s*)/, ["type.identifier", "operator"]],
                [/=/, "operator"],
                [/[%!]?[a-zA-Z_]\w*/, "macro.inFile", "@functionParameters"],
                [/[;,]/, "delimiter", "@pop"],
                [/$/, "", "@pop"]
            ],
            functionOutputs: [
                [/\s+/, "white"],
                [/[%!]?[a-zA-Z_]\w*/, "type.identifier"],
                [/,/, "delimiter"],
                [/\]/, "delimiter", "@pop"],
                [/$/, "", "@popall"]
            ],
            functionParameters: [
                [/\s+/, "white"],
                [/[%!]?[a-zA-Z_]\w*/, "type.identifier"],
                [/[,(]/, "delimiter"],
                [/\)/, "delimiter", "@popall"],
                [/$/, "", "@popall"]
            ],
            string: [
                // The quote is excluded so that the expression stops at a doubled
                // quote or at the end of the string instead of swallowing them.
                [/\$[^$"]+\$/, "latex.string"],
                [/\t+/, "string.tab"],
                [/[ \f\v]+/, "string.white"],
                [/[^"$\s]+/, "string"],
                [/\$/, "string"],
                [/""/, "string.escape"],
                [/"(?!")/, "string", "@pop"]
            ],
            singleString: [
                [/\$[^$']+\$/, "latex.string"],
                [/\t+/, "string.tab"],
                [/[ \f\v]+/, "string.white"],
                [/[^'$\s]+/, "string"],
                [/\$/, "string"],
                [/''/, "string.escape"],
                [/'(?!')/, "string", "@pop"]
            ]
        }
    });
}

/*
 * The syntax block of a help hover: calling forms such as
 * "[a, b] = name(x, list(f, p))". Semantic tokens do not reach a hover, so the
 * editor colors are rebuilt here: outputs and arguments as the arguments of a
 * function declaration, the documented name as a primitive or a macro
 * (calleeToken), nested calls as primitives.
 * Each line is a form of its own. Monarch carries its state to the next line
 * and tries no rule once a line is consumed, so every rule goes back to root
 * when its match ends the line.
 */
function registerScilabPrototypeLanguage(id, calleeToken) {
    function rule(regex, token, next) {
        var cases = { "@eos": { token: token, next: "@popall" } };
        cases["@default"] = next ? { token: token, next: next } : token;
        return [regex, { cases: cases }];
    }
    var identifier = /[%!$]?[a-zA-Z_]\w*/;
    var common = [
        rule(/\/\/.*$/, "comment"),
        rule(/"(?:[^"]|"")*"?/, "string"),
        rule(/'(?:[^']|'')*'?/, "string"),
        rule(/\b(([0-9]+\.?[0-9]*)|(\.[0-9]+))([eE][+\-]?[0-9]+)?\b/, "number"),
        rule(/\.{2,}|\$|:/, "operator.special"),
        rule(/(==|~=|<>|>=|<=|&&|\|\||[=<>&|+\-*\/\\^'])/, "operator"),
        rule(/[;,()[\]{}]/, "delimiter"),
        rule(/\s+/, "white")
    ];
    var keywords = [].concat(scilabKeywords.functionKeywords, scilabKeywords.controlKeywords,
                             scilabKeywords.structureKeywords, scilabKeywords.openCloseStructureKeywords);

    monaco.languages.register({ id: id });
    monaco.languages.setMonarchTokensProvider(id, {
        tokenizer: {
            root: [
                // A line of its own: the forms left out of the hover.
                [/\.\.\.$/, "elision"],
                rule(/\[/, "delimiter", "@outputs"),
                rule(/[%!$]?[a-zA-Z_]\w*(?=\s*=(?!=))/, "type.identifier"),
                [identifier, {
                    cases: {
                        "@eos": { token: "@rematch", next: "@lastWord" },
                        "@default": { token: "@rematch", next: "@callee" }
                    }
                }]
            ].concat(common),
            // The first name of a form: a keyword, or the documented function.
            callee: [
                [identifier, {
                    cases: {
                        "$0@keywords": { token: "keyword.structure", switchTo: "@args" },
                        "@default": { token: calleeToken, switchTo: "@args" }
                    }
                }]
            ],
            lastWord: [
                [identifier, {
                    cases: {
                        "$0@keywords": { token: "keyword.structure", next: "@popall" },
                        "@default": { token: calleeToken, next: "@popall" }
                    }
                }]
            ],
            outputs: [
                rule(/\]/, "delimiter", "@pop"),
                rule(identifier, "type.identifier")
            ].concat(common),
            args: [
                rule(/[%!$]?[a-zA-Z_]\w*(?=\s*\()/, "function.defaultLibrary"),
                rule(identifier, "type.identifier")
            ].concat(common)
        },
        keywords: keywords
    });
}
