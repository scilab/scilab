/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 * Scilab Help Browser - find in page (iframe side)
 *
 * Loaded by every page displayed in the content iframe (help pages generated
 * by browser.xslt, section index / search result pages built by shell.js,
 * homepage). The find bar itself lives in the shell (index.html / shell.js):
 * the pages are not same-origin with the shell, so the shell drives the
 * search through postMessage and this script does the work in the document.
 *
 * Matches are shown with the CSS Custom Highlight API (::highlight()), which
 * needs no change to the page markup: links, highlighted code and MathJax
 * output stay untouched, and a match may span several inline elements.
 *
 * Shell -> page messages:
 *   findInPage {term, caseSensitive, wholeWord, regex}  run a search
 *   findStep {direction}                                 next (1) / previous (-1)
 *   findClear                                            remove highlights
 *   findQuerySelection                                   ask for the selected text
 * Page -> shell messages:
 *   findResult {count, current}   after findInPage / findStep
 *   findSeed {seed}               answer to findQuerySelection
 *   findToggle {seed}             Ctrl+F pressed in the page
 *   findStep {direction}          F3 / Shift+F3 pressed in the page
 */
(function () {
    "use strict";

    if (window.parent === window) return;

    var HL_ALL = "scilab-find";
    var HL_CURRENT = "scilab-find-current";
    var supported = typeof CSS !== "undefined" && CSS.highlights && typeof Highlight === "function";

    var ranges = [];
    var current = -1;

    // Same colors as the console search widget
    var style = document.createElement("style");
    style.textContent = "::highlight(" + HL_ALL + ") { background-color: rgba(255, 196, 0, 0.35); }"
        + " ::highlight(" + HL_CURRENT + ") { background-color: #FF9632; color: #000000; }";
    (document.head || document.documentElement).appendChild(style);

    function send(msg) {
        window.parent.postMessage(msg, "*");
    }

    function reply() {
        send({type: "findResult", count: ranges.length, current: current});
    }

    // Text nodes worth searching: visible, not script/style, not the hidden
    // MathML copy MathJax keeps for assistive technologies.
    function isSearchable(textNode) {
        var p = textNode.parentElement;
        if (!p) return false;
        var tag = p.tagName;
        if (tag === "SCRIPT" || tag === "STYLE" || tag === "NOSCRIPT" || tag === "TEXTAREA") return false;
        if (p.closest("mjx-assistive-mml")) return false;
        return p.getClientRects().length > 0;
    }

    // Concatenate the searchable text nodes, keeping where each one starts,
    // so that a match may cross element boundaries (e.g. "a<b>b</b>c").
    function collect() {
        var parts = [];
        var text = "";
        var walker = document.createTreeWalker(document.body, NodeFilter.SHOW_TEXT, null);
        var n;
        while ((n = walker.nextNode())) {
            if (!n.data || !isSearchable(n)) continue;
            parts.push({node: n, start: text.length});
            text += n.data;
        }
        return {text: text, parts: parts};
    }

    // Global offset -> (text node, local offset). An end offset that falls on
    // a node boundary is resolved in the previous node (strict comparison).
    function locate(parts, offset, isEnd) {
        var lo = 0, hi = parts.length - 1;
        while (lo < hi) {
            var mid = (lo + hi + 1) >> 1;
            var before = isEnd ? parts[mid].start < offset : parts[mid].start <= offset;
            if (before) lo = mid; else hi = mid - 1;
        }
        return {node: parts[lo].node, offset: offset - parts[lo].start};
    }

    function buildRegex(term, caseSensitive, wholeWord, useRegex) {
        var src = useRegex ? term : term.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
        if (wholeWord) src = "\\b" + src + "\\b";
        return new RegExp(src, caseSensitive ? "g" : "gi");
    }

    function clear() {
        ranges = [];
        current = -1;
        if (supported) {
            CSS.highlights.delete(HL_ALL);
            CSS.highlights.delete(HL_CURRENT);
        }
    }

    function setCurrent() {
        if (current < 0 || current >= ranges.length) {
            CSS.highlights.delete(HL_CURRENT);
            return;
        }
        var h = new Highlight();
        h.add(ranges[current]);
        CSS.highlights.set(HL_CURRENT, h);
        // scrollIntoView on an element (not on the range) stays correct under
        // the CSS zoom applied by doczoom.js; the nearest inline ancestor is
        // close enough to the match.
        var el = ranges[current].startContainer.parentElement;
        if (el) el.scrollIntoView({block: "center", inline: "nearest", behavior: "smooth"});
    }

    function search(term, caseSensitive, wholeWord, useRegex) {
        clear();
        if (!supported || !term) {
            reply();
            return;
        }
        var regex;
        try {
            regex = buildRegex(term, caseSensitive, wholeWord, useRegex);
        } catch (e) {
            reply(); // invalid regular expression: no result
            return;
        }
        var data = collect();
        var m;
        while ((m = regex.exec(data.text)) !== null) {
            if (m[0].length === 0) {
                // empty match (e.g. "a*"): move on or exec loops forever
                regex.lastIndex++;
                continue;
            }
            var s = locate(data.parts, m.index, false);
            var e = locate(data.parts, m.index + m[0].length, true);
            var r = document.createRange();
            r.setStart(s.node, s.offset);
            r.setEnd(e.node, e.offset);
            ranges.push(r);
        }
        if (ranges.length) {
            var all = new Highlight();
            ranges.forEach(function (r) { all.add(r); });
            CSS.highlights.set(HL_ALL, all);
            current = 0;
            setCurrent();
        }
        reply();
    }

    function step(direction) {
        if (ranges.length) {
            current = (current + direction + ranges.length) % ranges.length;
            setCurrent();
        }
        reply();
    }

    window.addEventListener("message", function (e) {
        var d = e.data;
        if (!d || !d.type) return;
        switch (d.type) {
            case "findInPage":
                search(String(d.term || ""), !!d.caseSensitive, !!d.wholeWord, !!d.regex);
                break;
            case "findStep":
                step(d.direction < 0 ? -1 : 1);
                break;
            case "findClear":
                clear();
                break;
            case "findQuerySelection":
                send({type: "findSeed", seed: String(window.getSelection())});
                break;
        }
    });

    // Keys pressed while the page has the focus never reach the shell
    window.addEventListener("keydown", function (e) {
        if (e.key === "F3") {
            e.preventDefault();
            send({type: "findStep", direction: e.shiftKey ? -1 : 1});
        } else if ((e.ctrlKey || e.metaKey) && !e.altKey && (e.key === "f" || e.key === "F")) {
            e.preventDefault();
            send({type: "findToggle", seed: String(window.getSelection())});
        }
    });
})();
