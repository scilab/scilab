/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 * Scilab Help Browser - link status (iframe side)
 *
 * Loaded by every page displayed in the content iframe (help pages generated
 * by browser.xslt, section index and search result pages built by shell.js,
 * home page).
 *
 * The embedded browser has no status bar: the target of the link under the
 * mouse (or holding the keyboard focus) is sent to the shell, which shows it
 * in a status bubble, so that the user sees where a link leads before
 * following it. The target is the one the click handlers of the pages act
 * on: the xml:id of a cross-reference, the URL of an external link.
 */
(function () {
    "use strict";

    if (window.parent === window) return; // standalone: the browser shows it

    var current = "";

    /** What following the link does, or "" for an in-page control. */
    function linkTarget(a) {
        var linkend = a.getAttribute("data-linkend");
        if (linkend) {
            // type="remote": "<location>/<url>" opens the URL (see
            // handleScilabLink, SwingScilabHelpBrowser)
            var slash = linkend.indexOf("/");
            if (a.getAttribute("data-link-type") === "remote" && slash >= 0) {
                return linkend.substring(slash + 1);
            }
            return linkend;
        }
        var secpath = a.getAttribute("data-secpath");
        if (secpath) {
            return secpath;
        }
        var href = a.getAttribute("href");
        if (!href || href.charAt(0) === "#") {
            return ""; // section menu, back to top, ...
        }
        return a.href; // resolved absolute URL, as opened on click
    }

    function send(target) {
        if (target === current) return;
        current = target;
        window.parent.postMessage({type: "linkStatus", target: target}, "*");
    }

    function linkOf(node) {
        return (node && node.closest) ? node.closest("a[href]") : null;
    }

    document.addEventListener("mouseover", function (e) {
        var a = linkOf(e.target);
        send(a ? linkTarget(a) : "");
    });

    // Leaving the page (towards the shell, or out of the window)
    document.addEventListener("mouseout", function (e) {
        if (!e.relatedTarget) send("");
    });

    // Keyboard navigation (Tab) shows the focused link as well
    document.addEventListener("focusin", function (e) {
        var a = linkOf(e.target);
        if (a) send(linkTarget(a));
    });
    document.addEventListener("focusout", function (e) {
        if (linkOf(e.target)) send("");
    });

    window.addEventListener("pagehide", function () {
        send("");
    });
})();
