/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025-2026 - Dassault Systèmes S.E. - Antoine ELIAS
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

const COMPLETION_PAGE_SIZE = 6;
const LINES_THRESHOLD = 10000;


let resetHisto = false;
let scilabFont = null;
let currentSearchIndex = -1;
let searchResults = [];
let searchHighlightTimeout = null;

$(document).ready(async () => {
    hideCompletionWindow();

    await updateFont("Consolas", "", 13, "normal");

    $(".prompt").text("--> ");
    document.addEventListener('keyup', function(event) {
        if (event.key == "I" && event.ctrlKey && event.shiftKey) {
            Scilab({request: "devtools"});
            event.preventDefault();
            return;
         }

         if (event.key == "R" && event.ctrlKey && event.shiftKey) {
            Scilab({request: "reload"});
            event.preventDefault();
            return;
         }
    });
    document.addEventListener('wheel', function (event) {
        if (event.ctrlKey) {
            event.preventDefault();
        }
    }, { passive: false });


    $("body").on( "contextmenu", function() {
        Scilab({request: "contextmenu"});
    });

    $(".output").on("click", (e) => {
        if (window.getSelection().toString() == "") {
            focus();
            e.preventDefault();
            e.stopPropagation();
        } else {
            e.stopPropagation();
        }
    });

    $(".output").on("mouseover", (e) => {
        const target = e.target;
        // Only trigger preview if hovering directly over a LaTeX element
        if (target.classList.contains("latex-expression")) {
            showLatexPreview(e.clientX, e.clientY, target);
        }
    });

    $(".output").on("mouseout", (e) => {
        // Only hide if mouse leaves a LaTeX element
        if (e.relatedTarget && !e.relatedTarget.classList.contains("latex-preview")) {
            hideLatexPreview();
        }
    });

    // Clean up preview when clicking anywhere
    $("body").on("click", () => {
        hideLatexPreview();
    });

    $(".output").on("mousemove", (e) => {
        if ($(".latex-preview").is(":visible")) {
            updateLatexPreviewPosition(e.clientX, e.clientY);
        }
    });

    $(".console").on("mousedown", (_e) => {
        focus();
        Scilab({request: "focus"});
    });

    $(".console").on("click", (_e) => {
        hideCompletionWindow();
        focus();
    });

    $("body").on("mouseup", function(_e) {
        if (window.getSelection) {
            var txt = window.getSelection();
        } else if (document.getSelection) {
            var txt = document.getSelection();
        } else if (document.selection) {
            var txt = document.selection.createRange().text;
        }

        Scilab({request: `select::${txt.toString()}`});
    });

    $(".console").on("keypress", function (e) {
        switch (e.key) {
            case "Enter":
                if (isCompletion()) {
                    let pattern = $(".completion").data("pattern");
                    updateInput(pattern, $(".listitem.selected").data("entry"));
                    hideCompletionWindow();
                } else if (isInputEnabled()) {
                    sendCommand($(".input").val());
                }
                //the command is already handled, avoid inserting a newline in
                //the input: it would grow it one extra line until the next resize
                e.preventDefault();
                break;
            default:
                resetHisto = true;
                break;
        }
        // waiting for Y/N char, send it immediately without waiting for enter
        if (isInputDisabled() && e.key != "Enter") {
            setUserInputValue(e.key);
        }
    });

    $(".console").on("keyup", function(e) {
        if (isCompletion()) {
            if (e.key == "ArrowUp" || e.key == "ArrowDown") {
                e.preventDefault();
                e.stopPropagation();
                return;
            }

            if (e.key == "PageUp" || e.key == "PageDown") {
                e.preventDefault();
                e.stopPropagation();
                return;
            }

            if (e.key == "Home" || e.key == "End") {
                e.preventDefault();
                e.stopPropagation();
                return;
            }

            openCompletion(false);
        }

        updateLatexML();
    })

    $(".console").on("keydown", function(e) {
        log("FINEST", "console on keydown ", (e.altKey ? "ALT+" : "") + (e.ctrlKey ? "CTRL+" : "") + (e.metaKey ? "META+" : "") + e.keyCode);

        if (isCompletion()) {
            switch (e.key) {
                case "ArrowUp":
                    updateCompletionSelection(-1);
                    e.preventDefault();
                    e.stopPropagation();
                    break;
                case "PageUp":
                    updateCompletionSelection(-COMPLETION_PAGE_SIZE);
                    e.preventDefault();
                    e.stopPropagation();
                    break;
                case "Home":
                    updateCompletionSelection(0, "absolute");
                    e.preventDefault();
                    e.stopPropagation();
                    break;
                case "ArrowDown":
                    updateCompletionSelection(+1);
                    e.preventDefault();
                    e.stopPropagation();
                    break;
                case "PageDown":
                    updateCompletionSelection(COMPLETION_PAGE_SIZE);
                    e.preventDefault();
                    e.stopPropagation();
                    break;
                case "End":
                    updateCompletionSelection(-1, "absolute");
                    e.preventDefault();
                    e.stopPropagation();
                    break;
                case "Escape":
                    hideCompletionWindow();
                    e.preventDefault();
                    e.stopPropagation();
                    break;
            }
        }
        else
        {
            let s = shortcuts.filter(s => e.keyCode == s.keyCode && s.isALT == e.altKey && (e.ctrlKey || e.metaKey) == s.isCTRL);
            if (s.length != 0) {
                if (Object.keys(actionBinding).includes(s[0].action)) {
                    log("FINER", "action", s[0].action);
                    if(actionBinding[s[0].action](e)) {
                        e.preventDefault();
                        e.stopPropagation();
                    }
                }
            }
        }


    })

    //to resize input (scrollbar, ...)
    $(".input").on("input", (_e) => {
        updateInputCols();
    });

    //paste in .input is disable to ensure all paste operations arrive here
    $("body").on("paste", function(e) {
        //except the search field, which handles its paste natively
        if (e.target === $(".search-input")[0]) {
            return;
        }
        Scilab({request: "paste"});
        e.preventDefault();
    });

    $(".output").on("cut", function(e) {
        cutCopyOrInterruptAction("cut", e);
    })

    //needed to allow drag and drop
    $(".console").on("dragover", function(e) {
        e.preventDefault();
    });

    $(".console").on("drop", async function(e) {
        e.preventDefault();
        let data = e.originalEvent.dataTransfer.items;
        for (let i = 0; i < data.length; i += 1) {
            if (data[i].kind === "string") {
                data[i].getAsString((s) => {
                    let start = $(".input")[0].selectionStart;
                    let end = $(".input")[0].selectionEnd;
                    let text = $(".input").val();
                    let res = text.substring(0, start);
                    res += s;
                    $(".input")[0].selectionStart = res.length;
                    $(".input")[0].selectionEnd = $(".input")[0].selectionStart
                    res += text.substring(end);
                    setInputVal(res);
                    focus();
                });
                return;
            }

            if (data[i].kind === "file") {
                Scilab({
                    request: `file::${data[i].name}`
                })
                return;
            }
        }
    });

    Scilab({request: `ready::${getBackground()}::${getForeground()}`});
    adjustInputSize();
    updateSize();

    // Search functionality event handlers
    $(".search-input").on("input", updateSearchResults);
    $(".search-input").on("keydown", function(e) {
        if (e.key === "Enter") {
            e.preventDefault();
            if (searchResults.length > 0) {
                navigateSearchResults(e.shiftKey ? -1 : 1);
            }
        } else if (e.key === "F3") {
            //the console shortcut table does not see keys typed in the search
            //field, handle F3 / Shift+F3 here as well
            e.preventDefault();
            if (searchResults.length > 0) {
                navigateSearchResults(e.shiftKey ? -1 : 1);
            }
        } else if (e.key === "Escape") {
            closeSearch();
        }
    });
    $(".search-close").on("click", closeSearch);
    $(".search-prev").on("click", () => navigateSearchResults(-1));
    $(".search-next").on("click", () => navigateSearchResults(1));
    $(".search-case-sensitive, .search-regex, .search-whole-word").on("change", updateSearchResults);

    // Makes input editable (caret and focus)
    focus();
});

function sendCommand(cmd) {
    Scilab({request: `input::${cmd}`});

    adjustInputSize();
    resetHisto = true;
}

function setUserInputValue(inputChar) {
    Scilab({request: `input_char::${inputChar}`});

    adjustInputSize();
    resetHisto = true;
}

function isCompletion() {
    return $(".window").is(":visible");
}

function openCompletion(autovalid) {
    let cmd = $(".input").val();
    let pos = $(".input")[0].selectionStart

    if (cmd != "") {
        Scilab({
            request: `completion::${cmd}::${pos}`,
            onSuccess: (res) => {
                res = JSON.parse(res);
                if (res.entries.length == 0) {
                    clearCompletionContent();
                    return;
                }

                if (autovalid && res.entries.length == 1) {
                    updateInput(res.pattern, res.entries[0])
                } else {
                    clearCompletionContent();
                    $(".completion").data("pattern", res.pattern);
                    res.entries.forEach(r => addCompletionEntry(r.entry, r.type));
                    showCompletionWindow();
                }
            }
        });
    }
}

function updateInput(pattern, entry) {
    if (entry == undefined) {
        hideCompletionWindow();
        return;
    }

    let input = $(".input").val();
    let start = input.substring(0, $(".input")[0].selectionStart);
    let middle = entry.entry;
    let end = input.substring($(".input")[0].selectionEnd);

    Scilab({
        request: `completeline::${start}::${middle}::${entry.type}::${end}`,
        onSuccess: (res) => {
            let caretPos = $(".input")[0].selectionStart + (res.length - input.length);
            setInputVal(res);
            if (0 < caretPos && caretPos <= res.length) {
                moveInputCaret(caretPos);
            }

            if (entry.type == "Directory") {
                openCompletion(true);
            }
        }
    })
}

function getNextHistory() {
    //get data before caret
    let c = $(".input")[0];
    let str = c.value.substring(0, c.selectionStart);
    Scilab({request: `next::${str}::${resetHisto}`, onSuccess: res => setInputVal(res)});
    resetHisto = false;
}

function getNextPage(e) {
    // handle page up only in completion list, regular behavior is handled by the browser
    if (isCompletion()) {
        updateCompletionSelection(COMPLETION_PAGE_SIZE);
        e.preventDefault();
        e.stopPropagation();
    }
}

function getPreviousHistory() {
    //get data before caret
    let c = $(".input")[0];
    let str = c.value.substring(0, c.selectionStart);
    Scilab({request: `previous::${str}::${resetHisto}`, onSuccess: res => setInputVal(res)});
    resetHisto = false;
}

function getPreviousPage(e) {
    // handle page up only in completion list, regular behavior is handled by the browser
    if (isCompletion()) {
        updateCompletionSelection(-COMPLETION_PAGE_SIZE);
        e.preventDefault();
        e.stopPropagation();
    }
}

function adjustInputSize() {
    let c = $(".input")[0]
    let con = $(".console")[0];
    //shrinking the textarea to measure scrollHeight clamps the container scroll,
    //so save the scroll state and restore it afterwards to avoid jitter
    let atBottom = con.scrollTop + con.clientHeight >= con.scrollHeight - 1;
    let prev = con.scrollTop;
    c.style.height = "5px";
    c.style.height = `${c.scrollHeight}px`;
    con.scrollTop = atBottom ? con.scrollHeight : prev;
}

function isInputDisabled() {
    return $(".input-group").hasClass("hide-input");
}
function isInputEnabled() {
    return $(".input-group").hasClass("hide-input") === false;
}

function disableInput() {
    $(".input-group").addClass("hide-input");
    $(".console").focus();
    $(".input").val("");
    adjustInputSize();
    $("body").addClass(".progress");
}

function enableInput() {
    $(".input-group").removeClass("hide-input");
    focus();
    $("body").removeClass(".progress");
}

function clearInput() {
    setInputVal("");
    focus();
}

/**
 * Clear output sections interpreted as lines of text
 * @param {int} nbLines number of lines to remove
 *                       * 0 to clear all
 *                       * 1 to clear one line (y/n input asking)
 *                       * N to clear N lines
 */
function clearConsole(nbLines) {
    let loopTI = setInterval(() => {
        if (refreshTO == null) {
            let linesToRemove = nbLines;
            if (linesToRemove <= 0)
            {
                // corner case, 0 is a full clear
                $(".output").empty();
            }
            else
            {
                let last = $(".output").children(":last-child");
                let txt = last.text();
                let idx = txt.lastIndexOf("\n");
                while (idx != -1 && linesToRemove > 0) {
                    while (idx != -1 && linesToRemove > 0) {
                        linesToRemove--;
                        txt = txt.substring(0, idx + 1);
                        idx = txt.lastIndexOf("\n");
                    }
                    if (linesToRemove > 0) {
                        // remove the block entirely and continue with the previous one
                        last.remove();
                        last = $(".output").children(":last-child");
                        txt = last.text();
                        idx = txt.lastIndexOf("\n");
                    }
                }
                if (linesToRemove > 0)
                {
                    last.remove();
                    last = $(".output").children(":last-child");
                }
                else
                {
                    last.text(txt);
                }

                focus();
            }

            clearInterval(loopTI);
            Scilab({request: "cleared"});
        }
    }, 20);
}

var output = [];
var refreshTO = null;
async function fromJava(msg) {
    switch(msg.type)
    {
        case "lock-input":
            disableInput();
            break;
        case "unlock-input":
            enableInput();
            break;
        case "clear":
        {
            let keys = Object.keys(msg);
            let key1 = keys.includes("data1");
            if (key1) {
                clearConsole(msg.data1);
            } else {
                clearConsole(0);
            }
            break;
        }
        case "output":
            if (output.length > 0 && output[output.length-1].style == msg.data2) {
                output[output.length-1].text += msg.data1
            } else {
                output.push({style: msg.data2, text: msg.data1});
            }

            if (refreshTO == null) {
                refreshTO = setTimeout(() => {
                    let last = $(".output").children(":last-child");

                    output.forEach(o => {
                        if (o.style == "monospace" && last.hasClass("monospace")) {
                            appendMonospaceText(last, processTextForLatex(processTextCRtoNL(o.text)));
                        } else if (o.style == "monospace") {
                            last = $(`<pre class="monospace"></pre>`);
                            appendMonospaceText(last, processTextForLatex(processTextCRtoNL(o.text)));
                            $(".output").append(last);
                        }
                        // no else on purpose, implement style and mime-type for further support
                    });

                    // limit output to LINES_THRESHOLD lines, remove oldest lines if exceeded
                    let c = $(".console")[0];
                    let lines = c.scrollHeight / 15;
                    if (lines > LINES_THRESHOLD * 1.5) {
                        // TODO: process mixed bag of img/div and monospace text
                        let arr = $(".output").children(":last-child")[0].innerHTML.split("\n");
                        arr = arr.slice(-LINES_THRESHOLD);
                        $(".output").children(":last-child")[0].innerHTML = arr.join("\n");
                    }

                    updateScrollView();
                    output = [];
                    refreshTO = null;
                }, 10);
            }
            break;
        case "bgcolor":
            $("body").css("background-color", msg.data1)
            break;
        case "fgcolor":
            $("body").css("color", msg.data1)
            break;
        case "caret":
            $("body").css("caret-color", msg.data1)
            break;
        case "cut":
            cutCopyOrInterruptAction("cut");
            break;
        case "copy":
            cutCopyOrInterruptAction("copy");
            break;
        case "paste":
            pasteAction(msg.data1);
            break;
        case "prompt":
            $(".prompt").text(msg.data1);
            break;
        case "debug":
        {
            let keys = Object.keys(msg);
            let key1 = keys.includes("data1");
            let key2 = keys.includes("data2");
            let key3 = keys.includes("data3");
            let key4 = keys.includes("data4");

            if (key4 && key3 && key2 && key1) {
                log("debug", [msg.data1, msg.data2, msg.data3, msg.data4]);
            } else if (key3 && key2 && key1) {
                log("debug", [msg.data1, msg.data2, msg.data3]);
            } else if (key2 && key1) {
                log("debug", [msg.data1, msg.data2]);
            } else if (key1) {
                log("debug", [msg.data1]);
            }
            break;
        }
        case "focus":
            focus();
            break;
        case "selectall":
            selectAllAction();
            break;
        case "font":
            setTimeout(async () => {
                await updateFont(msg.data1, msg.data2, msg.data3, msg.data4 ? "bold" : "normal");
            }, 1);
            break;
        case "clear-shortcut":
            shortcuts = [];
            break;
        case "sort-shortcut":
            shortcuts.sort((a, b) => a.keyCode - b.keyCode || a.isCTRL - b.isCTRL || a.isALT - b.isALT || a.action.localeCompare(b.action));
            break;
        case "shortcut":
            shortcuts.push({
                keyCode: msg.data2,
                action: msg.data1,
                isCTRL: msg.data3,
                isALT: msg.data4
             });

            break;
        default:
            log("WARNING", "unmanaged msg", msg.type);
            break;
        }
}

/**  Convert carriage returns and new lines */
function processTextCRtoNL(txt) {
    return txt.replace(/\r\n/g, "\n").replace(/\r/g, "\n");
}

function appendMonospaceText(node, {text, isHTML}) {
    let prev = null;
    if (isHTML || node.children().length > 0) {
        prev = node.html();
    } else {
        prev = node.text();
    }

    if (isHTML) {
        node.html(prev + text);
    } else if (node.children().length > 0) {
        node.html(prev + escapeHtml(text));
    } else {
        node.text(prev + text);
    }
}

/** Process text to wrap LaTeX expressions in HTML elements */
function processTextForLatex(txt) {
    let idx = 0;
    let processed = "";
    let isHTML = false;
    for (const match of txt.matchAll(/"\$(.*?)\$"/gm))
    {
        // text before
        processed += escapeHtml(txt.substring(idx, match.index));
        // LaTeX
        processed += `<span class="monospace latex-expression" data-latex="${escapeHtml(match[1])}">${escapeHtml(match[0])}</span>`;
        // next
        idx = match.index + match[0].length;

        isHTML = true;
    }
    // last text
    if (isHTML) {
        processed += escapeHtml(txt.substring(idx));
    } else {
        processed += txt.substring(idx);
    }

    return {text: processed, isHTML: isHTML};
}

function getSelectedText() {
    let text = "";

    if (window.getSelection) {
        text = window.getSelection().toString();
    } else if (document.selection && document.selection.type != "Control") {
        text = document.selection.createRange().text;
    }

    return text;
}

function deleteSelection() {
    let start = $(".input")[0].selectionStart;
    let end = $(".input")[0].selectionEnd;
    if (start != end) {
        let text = $(".input").val();
        let res = text.substring(0, start);
        res += text.substring(end);
        setInputVal(res);
    }
}

function pasteAction(clipText) {

    if (clipText.slice(-1) == "\n") {
        let lines = clipText.split("\n");
        lines.forEach(l => sendCommand(l));
        return;
    }

    let c = $(".input")[0];
    let start = c.selectionStart;
    let end = c.selectionEnd;
    var before = c.value.slice(0, start);
    var after = c.value.slice(end);

    c.value = before + clipText + after;
    c.setSelectionRange(start + clipText.length, start + clipText.length);
    focus();
    adjustInputSize();

    updateScrollView();

    resetHisto = true;
}

function updateScrollView() {
    let c  = $(".console")[0];
    c.scrollTo(0, c.scrollHeight);
}

function updateSize() {
    let c = $("body");
    let h = c[0].scrollHeight;
    let w = c[0].scrollWidth;
    Scilab({request: `size::${w}::${h}::${getConsoleSize().join("::")}`});
}

function getConsoleSize() {
    let rect = getStringRect("0123456789");
    return [Math.floor((($("body").width() - $(".prompt").width()) / rect.width * 10)),
        Math.floor((($("body").height() - $(".input-group").height()) / rect.height))];
}

function getStringRect(s) {
    $(".ghost").text(s);
    return $(".ghost")[0].getBoundingClientRect();
}

/* completion */
function showCompletionWindow() {
    let width = getStringRect($(".input").val().substring(0, $(".input")[0].selectionStart)).width;
    let left = $(".input")[0].getBoundingClientRect().x + width;
    $(".window").css("left", left);

    $(".listitem:first-child").addClass("selected");
    $(".completion").show();
    updateCompletionSelection(0, "absolute");
    updateScrollView();
}

function hideCompletionWindow() {
    $(".completion").hide();
    focus();
}

function clearCompletionContent() {
    $(".listbox").children().remove();
}

function addCompletionEntry(entry, type) {
    let li = $("<li>").addClass("listitem").data("entry", {entry, type});
    li.append($("<div>").addClass("listentry").text(entry));
    li.append($("<div>").addClass("listpad"));
    li.append($("<div>").addClass("listtype").text(type));
    li.click((e) => {
        let pattern = $(".completion").data("pattern");
        updateInput(pattern, $(e.target).parent().data("entry"));
        hideCompletionWindow();
    });

    $(".listbox").append(li);
}

function updateCompletionSelection(increment, mode) {
    let idx = 0;
    let count = $(".listbox").children().length;

    if (mode == "absolute") {
        idx = increment == 0 ? 1 : count;
    } else {
        let count = $(".listbox").children().length;
        idx = $(".listitem.selected").index();
        idx += increment + 1;
        if (idx > count) {
            idx = 1;
        }

        if (idx <= 0) {
            idx = count;
        }
    }

    selectCompletion(idx);
}

function selectCompletion(idx) {
    let selected = $(`.listbox li:nth-child(${idx})`);
    $(".listitem").removeClass("selected");
    selected.addClass("selected");
    selected[0].scrollIntoView({
        block: "nearest",
        inline: "start",
        behavior: "auto"
    });
}

function rgb2hex(c) {
    function hex(x) {
        return ("0" + parseInt(x).toString(16)).slice(-2);
    }

    c = c.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
    return hex(c[1]) + hex(c[2]) + hex(c[3]);
}

function getForeground() {
    return rgb2hex($("body").css("color"));
}

function getBackground() {
    return rgb2hex($("body").css("background-color"));
}

function setInputVal(content) {
    $(".input").val(content);

    updateInputCols();
    adjustInputSize();
    //to focus on caret position
    $(".input").blur()
    focus()
}

function updateInputCols() {
    //update textarea cols
    let lines = $(".input").val().split(/\n/);
    let min = getConsoleSize()[0] - $(".prompt").text().length;
    let max = Math.max(...lines.map(l => l.length));
    $(".input").attr("cols", Math.max(min, max));
}

function focus() {
    let inputEl = $(".input")[0];

    // Only focus if not already focused
    if (document.activeElement != inputEl) {
        inputEl.focus();
    }

    if (inputEl.selectionStart === inputEl.selectionEnd) {
        // If caret position is invalid or at the very end, ensure it's visible
        let currentPos = inputEl.selectionStart;
        if (currentPos === undefined || currentPos === null || currentPos < 0 || currentPos > inputEl.value.length) {
            // Invalid position - set to appropriate end
            inputEl.selectionStart = inputEl.selectionEnd = inputEl.value.length;
        }
    }
    // An active selection (start != end) is left untouched: collapsing it here
    // would kill mouse selections, since releasing the mouse over the input
    // bubbles a click that ends up calling focus()
}

/** Log Scilab version */
function getVersion() {
    Scilab({
        request: "version",
        onSuccess: (data) => {
            log("INFO", data);
        }
    })
}

/**
 * Log data to Scilab Java Loggers
 * @param {string} [level] - Log level (INFO, WARNING, ERROR, DEBUG)
 * @param {any} args - Data to log, can be of any type. Objects will be stringified.
 * @returns {void}
 */
function log(...args) {
    Scilab({
        request: `log::${args.map(d => typeof d == "object" ? JSON.stringify(d) : d).join("::")}`
    })
}

/*shortcut ; referenced as action in modules/console/etc/XConfiguration-general.xml */
let shortcuts = [];
let actionBinding = {
    "console-box": (_e) => { openConsoleBox(); return true; },
    "console-clear": (_e) => { clearAction(); return true; },
    "console-completion": (_e) => { openCompletion(true); return true; },
    "console-copy-or-interrupt": (e) => { cutCopyOrInterruptAction("copy", e); return false; },
    "console-cut-or-interrupt": (e) => { cutCopyOrInterruptAction("cut", e); return false; },
    "console-del-end-of-line": (_e) => { deleteEndOfLine(); return true; },
    "console-del-last-word": (_e) => { deleteLastWord(); return true; },
    "console-del-next-char": (_e) => { deleteNextChar(); return true; },
    "console-del-previous-char": (_e) => { deletePreviousChar(); return true; },
    "console-del-whole-line": (_e) => { clearInput(); return true; },
    "console-enter": (_e) => { sendCommand($(".input").val()); return true; },
    "console-help-browser": (_e) => { Scilab({ request: `help::${getSelectedText()}` }); return true; },
    "console-line-back-one-word": (_e) => { moveBackOneWord(); return true; },
    "console-line-begin": (_e) => { moveInputCaret(0); return true; },
    "console-line-end": (_e) => { moveInputCaret($(".input").val().length); return true; },
    "console-line-forward-one-word": (_e) => { moveForwardOneWord(); return true; },
    "console-next-history": (_e) => { getNextHistory(); return true; },
    "console-next-page": (e) => { getNextPage(e); return false; },
    "console-prev-history": (_e) => { getPreviousHistory(); return true; },
    "console-prev-page": (e) => { getPreviousPage(e); return false; },
    "console-search-field": (_e) => { toggleSearch(); return true; },
    //Shift is not part of the shortcut table, the handler reads it from the event
    "console-search-next": (e) => { searchStep(e && e.shiftKey ? -1 : 1); return true; },
    "console-select-all": (_e) => { selectAllAction(true); return true; },
}

function openConsoleBox() {
    Scilab({request: `consolebox::on`});
}

function clearAction() {
    Scilab({request: `clear`});
}

function cutCopyOrInterruptAction(action, e) {
    if (isInputDisabled()) {
        Scilab({
            request: "interrupt"
        })
        if (e) {
            e.preventDefault();
        }
    } else if ($(".output")[0].selectionStart == $(".output")[0].selectionEnd) {
        let text = getSelectedText();
        Scilab({
            request: `copy::${text}`
        });
        if (action == "cut") {
            deleteSelection();
        }
        if (e) {
            e.preventDefault();
        }
    } else if (document.activeElement == $(".input")[0]) {
        if ($(".prompt").text() !== "--> " || //in multiline command
            $(".input")[0].selectionStart == $(".input")[0].selectionEnd) { //selection in input
            Scilab({
                request: `interruptcommand::${$(".input").val()}`
            })

            if (e) {
                e.preventDefault();
            }
        }
    }
}

function selectAllAction() {
    var range = document.createRange();
    range.selectNodeContents($(".output")[0]);
    window.getSelection().removeAllRanges();
    window.getSelection().addRange(range);
}

function moveInputCaret(pos) {
    let el = $(".input")[0];
    if (typeof el.selectionStart == "number") {
        el.selectionStart = el.selectionEnd = pos;
    } else if (typeof el.createTextRange != "undefined") {
        el.focus();
        let range = el.createTextRange();
        range.collapse(false);
        range.select();
    }
}

function deletePreviousChar() {
    let c = $(".input")[0];
    let start = c.selectionStart;
    let end = c.selectionEnd;
    if (start != end) {
        let text = $(".input").val();
        let res = text.substring(0, start);
        res += text.substring(end);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start;
    } else if (start > 0) {
        let text = $(".input").val();
        let res = text.substring(0, start - 1);
        res += text.substring(end);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start - 1;
    }
}

function deleteNextChar() {
    let c = $(".input")[0];
    let start = c.selectionStart;
    let end = c.selectionEnd;
    let text = $(".input").val();
    if (start != end) {
        let res = text.substring(0, start);
        res += text.substring(end);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start;
    } else if (end < text.length) {
        let res = text.substring(0, start);
        res += text.substring(end + 1);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start;
    }
}

function deleteLastWord() {
    let c = $(".input")[0];
    let start = c.selectionStart;
    let end = c.selectionEnd;
    let text = $(".input").val();
    if (start != end) {
        let res = text.substring(0, start);
        res += text.substring(end);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start;
    } else if (start > 0) {
        let res = text.substring(0, start - 1).replace(/\S+$/, "");
        res += text.substring(end);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = res.length - text.substring(end).length;
    }
}

function deleteEndOfLine() {
    let c = $(".input")[0];
    let start = c.selectionStart;
    let end = c.selectionEnd;
    let text = $(".input").val();
    if (start != end) {
        let res = text.substring(0, start);
        res += text.substring(end);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start;
    } else if (end < text.length) {
        let res = text.substring(0, start);
        res += text.substring(end + 1);
        setInputVal(res);
        c.selectionStart = c.selectionEnd = start;
    }
}

function moveBackOneWord() {
    let c = $(".input")[0];
    let start = c.selectionStart;
    let text = $(".input").val();
    if (start > 0) {
        let idx = text.substring(0, start - 1).search(/\S+\s*$/);
        if (idx != -1) {
            c.selectionStart = c.selectionEnd = idx;
        } else {
            c.selectionStart = c.selectionEnd = 0;
        }
    }
}

function moveForwardOneWord() {
    let c = $(".input")[0];
    let start = c.selectionStart;
    let text = $(".input").val();
    if (start < text.length) {
        let idx = text.substring(start).search(/^\S+/);
        if (idx != -1) {
            c.selectionStart = c.selectionEnd = start + idx;
        } else {
            c.selectionStart = c.selectionEnd = text.length;
        }
    }
}

function updateLatexML() {
    let c = $(".input")[0];

    let caret = c.selectionStart;

    // Find the LaTeX opening ("$ or '$) closest before the caret, i.e. the
    // expression the caret is currently in. Scanning every opening (instead of
    // only the first one) lets the preview follow the caret across several
    // expressions, e.g. ["$\LaTeX$" "$\alpha$"].
    let start = -1;
    for (const m of c.value.matchAll(/["']\$/g)) {
        if (m.index + 2 <= caret) {
            start = m.index + 2;
        } else {
            break;
        }
    }

    if (start != -1) { //latex
        let end = c.value.indexOf(`$`, start);
        if (end == -1) {
            end = c.value.length;
        }

        if (caret >= start && caret <= end) {
            let latex = c.value.substring(start, end);
            // Get or create the latex-preview container
            let previewContainer = $(".latex-preview");
            if (previewContainer.length === 0) {
                previewContainer = $('<div>').addClass('latex-preview');
                $('body').append(previewContainer);
            }

            // Clear previous content
            previewContainer.empty();

            // Add loading indicator
            previewContainer.html('<div style="text-align:center;padding:8px;">Rendering LaTeX...</div>');

            // Calculate position based on caret
            let width = getStringRect($(".input").val().substring(0, start)).width;
            let inputRect = $(".input")[0].getBoundingClientRect();
            let left = inputRect.x + width;
            let top = inputRect.y + inputRect.height;

            previewContainer.css({
                'left': left + 'px',
                'top': top + 'px',
                'display': 'block'
            });

            // Render the LaTeX
            try {
                MathJax.texReset();
                const options = MathJax.getMetricsFor(previewContainer[0]);
                options.display = false;

                MathJax.tex2svgPromise(latex, options).then(function (node) {
                    previewContainer.empty();
                    previewContainer[0].appendChild(node);
                    MathJax.startup.document.clear();
                    MathJax.startup.document.updateDocument();
                }).catch(function (err) {
                    previewContainer.html('<div style="text-align:center;padding:8px;color:red;">LaTeX error</div>');
                    log("ERROR", "LaTeX preview error:", err.message);
                });
            } catch (err) {
                previewContainer.html('<div style="text-align:center;padding:8px;color:red;">LaTeX error</div>');
                log("ERROR", "LaTeX preview error:", err.message);
            }
            return;
        }
    } else {
        let res = c.value.match(/"<(.*?)>.*<\/(.*)>/);
        if (res) {
            if (res.length == 3 && res[1] == res[2]) {
                let mathml = res[0].substring(1);
                // Get or create the latex-preview container
                let previewContainer = $(".latex-preview");
                if (previewContainer.length === 0) {
                    previewContainer = $('<div>').addClass('latex-preview');
                    $('body').append(previewContainer);
                }

                // Clear previous content
                previewContainer.empty();

                // Add loading indicator
                previewContainer.html('<div style="text-align:center;padding:8px;">Rendering MathML...</div>');

                // Position MathML preview below the input (similar to LaTeX but without caret tracking)
                let inputRect = $(".input")[0].getBoundingClientRect();
                previewContainer.css({
                    'left': inputRect.x + 'px',
                    'top': inputRect.y + inputRect.height + 'px',
                    'display': 'block'
                });

                try {
                    MathJax.mathmlReset();
                    const options = MathJax.getMetricsFor(previewContainer[0]);
                    options.display = false;

                    log("INFO", "mathml", mathml);
                    MathJax.mathml2svgPromise(`<math>${mathml}</math>`, options).then(function (node) {
                        previewContainer.empty();
                        previewContainer[0].appendChild(node);
                        MathJax.startup.document.clear();
                        MathJax.startup.document.updateDocument();
                    }).catch(function (err) {
                        log("ERROR", err);
                        previewContainer.html('<div style="text-align:center;padding:8px;color:red;">MathML error</div>');
                    });
                } catch (err) {
                    previewContainer.html('<div style="text-align:center;padding:8px;color:red;">MathML error</div>');
                    log("ERROR", "MathML error:", err.message);
                }
                return;
            }
        }
    }

    // Hide the preview if no valid LaTeX/MathML is being edited
    hideLatexPreview();
}

async function updateFont(name, family, size, weight) {
    //remove current scilab-console font
    log("INFO", "updateFont" ,family, size, weight);
    if (scilabFont) {
        document.fonts.delete(scilabFont);
        scilabFont = null;
    }

    if (family == "Monospaced") {
        $("body").css("font-family", "monospace");
        $("body").css("font-size", size);
        $("body").css("font-weight", weight);
        return;
    }

    scilabFont = new FontFace("scilab-font", `local("${name}")`);

    try {
        await scilabFont.load();
        document.fonts.add(scilabFont);
        $("body").css("font-family", "scilab-font");
        $("body").css("font-size", size);
        $("body").css("font-weight", weight);

    } catch(e) {
        log("WARNING", `Font ${family} failed to load`, e);
    }
}

/* Search functionality */
//F3 / Shift+F3: jump to next or previous occurrence, opening the search first
//if it is not visible (the seeding with the current selection then applies)
function searchStep(direction) {
    if (!$(".search-container").is(":visible")) {
        toggleSearch();
        return;
    }
    navigateSearchResults(direction);
}

function toggleSearch() {
    const searchContainer = $(".search-container");
    if (searchContainer.is(":visible")) {
        closeSearch();
    } else {
        searchContainer.show();

        //seed the field with the current selection when it fits on one line,
        //wherever it lives: output (DOM selection) or input (textarea selection).
        //Without selection the field keeps the previous search term
        let sel = window.getSelection().toString();
        if (sel === "") {
            let input = $(".input")[0];
            sel = input.value.substring(input.selectionStart, input.selectionEnd);
        }
        if (sel !== "" && !sel.includes("\n")) {
            $(".search-input").val(sel);
        }

        $(".search-input").focus();
        $(".search-input")[0].select();
        updateSearchResults();
    }
}

function closeSearch() {
    $(".search-container").hide();
    clearSearchHighlights();
    focus();
}

function updateSearchResults() {
    const searchTerm = $(".search-input").val();
    //clear before searching: leftover highlight spans would be scanned again
    //by performSearch and produce duplicated results
    clearSearchHighlights();
    if (searchTerm === "") {
        $(".search-result-count").text("");
        $(".search-container").removeClass("no-results");
        searchResults = [];
        currentSearchIndex = -1;
        return;
    }

    const caseSensitive = $(".search-case-sensitive").is(":checked");
    const useRegex = $(".search-regex").is(":checked");
    const wholeWord = $(".search-whole-word").is(":checked");

    searchResults = performSearch(searchTerm, caseSensitive, useRegex, wholeWord);

    if (searchResults.length > 0) {
        currentSearchIndex = 0;
        highlightAllSearchResults();
    } else {
        currentSearchIndex = -1;
    }

    updateSearchResultDisplay();
}

function updateSearchResultDisplay() {
    if (searchResults.length === 0) {
        $(".search-result-count").text("No results");
        $(".search-container").addClass("no-results");
    } else {
        $(".search-container").removeClass("no-results");
        if (currentSearchIndex >= 0 && currentSearchIndex < searchResults.length) {
            $(".search-result-count").text(`${currentSearchIndex + 1} of ${searchResults.length}`);
        } else {
            $(".search-result-count").text(`${searchResults.length}`);
        }
    }
}

function performSearch(searchTerm, caseSensitive, useRegex, wholeWord) {
    const results = [];
    const outputElements = $(".output").find("*");

    //only for the plain-text path: lowercasing a regex pattern would corrupt
    //escapes like \W, \S or \D into their opposite class, regex uses the i flag
    if (!caseSensitive && !useRegex) {
        searchTerm = searchTerm.toLowerCase();
    }

    outputElements.each((index, element) => {
        const $element = $(element);
        const text = $element.text();
        const originalText = text;
        let searchText = caseSensitive ? text : text.toLowerCase();

        try {
            if (useRegex) {
                //m flag: the output is one big text block, ^ and $ must match
                //on every line, not only at the block boundaries
                const flags = caseSensitive ? "gm" : "gim";
                const regex = wholeWord ? new RegExp(`\\b${searchTerm}\\b`, flags) : new RegExp(searchTerm, flags);
                let match;
                while ((match = regex.exec(text)) !== null) {
                    if (match[0].length === 0) {
                        //empty match (e.g. "a*"): move on or exec loops forever
                        regex.lastIndex++;
                        continue;
                    }
                    results.push({
                        element: $element,
                        start: match.index,
                        end: match.index + match[0].length,
                        text: match[0]
                    });
                }
            } else {
                let startIndex = 0;
                while (true) {
                    let foundIndex = searchText.indexOf(searchTerm, startIndex);
                    if (foundIndex === -1) break;

                    if (wholeWord) {
                        // Check if it's a whole word match
                        const before = foundIndex === 0 || !searchText[foundIndex - 1].match(/\w/);
                        const after = foundIndex + searchTerm.length === searchText.length ||
                                      !searchText[foundIndex + searchTerm.length].match(/\w/);
                        if (!(before && after)) {
                            startIndex = foundIndex + 1;
                            continue;
                        }
                    }

                    results.push({
                        element: $element,
                        start: foundIndex,
                        end: foundIndex + searchTerm.length,
                        text: originalText.substring(foundIndex, foundIndex + searchTerm.length)
                    });

                    startIndex = foundIndex + searchTerm.length;
                }
            }
        } catch (e) {
            log("ERROR", "Search error:", e.message);
        }
    });

    return results;
}

//wrap every match in a highlight span (VS Code style), then mark the current one.
//Content is rebuilt with text nodes, never with html(): output text may contain
//markup characters that must stay literal
function highlightAllSearchResults() {
    clearSearchHighlights();

    //group results by element, they are already in document order
    const byElement = new Map();
    searchResults.forEach((result, index) => {
        const el = result.element[0];
        if (!byElement.has(el)) {
            byElement.set(el, []);
        }
        byElement.get(el).push({result, index});
    });

    byElement.forEach((matches, el) => {
        const text = $(el).text();
        const frag = document.createDocumentFragment();
        let pos = 0;
        matches.forEach(({result, index}) => {
            if (result.start > pos) {
                frag.appendChild(document.createTextNode(text.substring(pos, result.start)));
            }
            const span = document.createElement("span");
            span.className = "search-highlight";
            span.dataset.searchIndex = index;
            span.textContent = text.substring(result.start, result.end);
            frag.appendChild(span);
            pos = result.end;
        });
        if (pos < text.length) {
            frag.appendChild(document.createTextNode(text.substring(pos)));
        }
        el.textContent = "";
        el.appendChild(frag);
    });

    setCurrentSearchResult();
}

//move the "current" style to the highlight span of currentSearchIndex and show it
function setCurrentSearchResult() {
    $(".search-highlight").removeClass("search-highlight-current");
    const $span = $(`.search-highlight[data-search-index="${currentSearchIndex}"]`);
    if ($span.length) {
        $span.addClass("search-highlight-current");
        scrollToSearchResult($span);
    }
}

function scrollToSearchResult($highlightSpan) {
    const $scrollContainer = $(".console")

    // Get the position of the highlighted span relative to the scroll container
    const spanPosition = $highlightSpan.position().top;
    const containerHeight = $scrollContainer.height();
    const currentScroll = $scrollContainer.scrollTop();

    // Calculate target scroll position to center the span
    const targetScroll = currentScroll + spanPosition - (containerHeight / 2) + ($highlightSpan.outerHeight() / 2);

    // Animate scroll to center the span
    $scrollContainer.animate({
        scrollTop: targetScroll
    }, 200, function() {
        // Ensure the span is visible after scrolling
        $highlightSpan[0].scrollIntoView({block: "nearest", inline: "nearest"});
    });
}

function clearSearchHighlights() {
    if (searchHighlightTimeout) {
        clearTimeout(searchHighlightTimeout);
    }

    const parents = new Set();
    $(".search-highlight").each(function() {
        const $span = $(this);
        parents.add(this.parentNode);
        $span.replaceWith($span.text());
    });
    //merge back the text nodes left by replaceWith
    parents.forEach(p => p && p.normalize());
}

function navigateSearchResults(direction) {
    if (searchResults.length === 0) return;

    currentSearchIndex += direction;

    if (currentSearchIndex >= searchResults.length) {
        currentSearchIndex = 0;
    } else if (currentSearchIndex < 0) {
        currentSearchIndex = searchResults.length - 1;
    }

    setCurrentSearchResult();
    updateSearchResultDisplay();
}

/* LaTeX Preview Functions */
function showLatexPreview(x, y, element) {

    // Get the LaTeX expression directly from the element's data attribute
    const latex = element.getAttribute('data-latex');
    if (!latex) {
        hideLatexPreview();
        return;
    }

    // Show preview immediately since we know we're hovering over a LaTeX element
    renderLatexPreview(latex, x, y);
}

function renderLatexPreview(latex, x, y) {
    // Create preview container if it doesn't exist
    let previewContainer = $(".latex-preview");
    if (previewContainer.length === 0) {
        previewContainer = $('<div>').addClass('latex-preview');
        $('body').append(previewContainer);
    }

    // Clear previous content
    previewContainer.empty();

    // Add loading indicator
    previewContainer.html('<div style="text-align:center;padding:8px;">Rendering LaTeX...</div>');
    previewContainer.css({
        'left': (x + 15) + 'px',
        'top': (y + 15) + 'px',
        'display': 'block'
    });

    // Render the LaTeX
    try {
        MathJax.texReset();
        const options = MathJax.getMetricsFor(previewContainer[0]);
        options.display = false;

        MathJax.tex2svgPromise(latex, options).then(function (node) {
            previewContainer.empty();
            previewContainer[0].appendChild(node);
            MathJax.startup.document.clear();
            MathJax.startup.document.updateDocument();
        }).catch(function (err) {
            previewContainer.html('<div style="text-align:center;padding:8px;color:red;">LaTeX error</div>');
            log("ERROR", "LaTeX preview error:", err.message);
        });
    } catch (err) {
        previewContainer.html('<div style="text-align:center;padding:8px;color:red;">LaTeX error</div>');
        log("ERROR", "LaTeX preview error:", err.message);
    }
}

function updateLatexPreviewPosition(x, y) {
    const previewContainer = $(".latex-preview");
    if (previewContainer.length > 0) {
        previewContainer.css({
            'left': (x + 15) + 'px',
            'top': (y + 15) + 'px'
        });
    }
}

function hideLatexPreview() {
    // Hide the preview container
    $(".latex-preview").hide();
}

/**
 * Escape HTML special characters for safe attribute values
 */
function escapeHtml(text) {
    return text
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}
