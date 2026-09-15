/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 * Scilab Help Browser - document zoom (iframe side)
 *
 * Loaded by every page displayed in the content iframe (help pages generated
 * by browser.xslt, section index and search result pages built by shell.js).
 *
 * Ctrl+wheel and Ctrl+'+'/'-'/'0' normally trigger Chromium's page zoom, which
 * scales the whole browser (sidebar included). They are intercepted here and
 * forwarded to the shell, which keeps the zoom factor and applies it to the
 * document only (CSS zoom on the root element, see the "setZoom" message).
 */
(function () {
    "use strict";

    if (window.parent === window) return; // standalone: keep browser zoom

    function send(msg) {
        window.parent.postMessage(msg, "*");
    }

    // Ctrl+wheel: one zoom step per notch. The listener must be non-passive
    // so that preventDefault() cancels the browser-level zoom.
    window.addEventListener("wheel", function (e) {
        if (!e.ctrlKey || e.deltaY === 0) return;
        e.preventDefault();
        send({type: "zoomDoc", delta: e.deltaY < 0 ? 1 : -1});
    }, {passive: false});

    // Ctrl+'+' / Ctrl+'-' / Ctrl+'0'
    window.addEventListener("keydown", function (e) {
        if (!e.ctrlKey || e.altKey) return;
        var delta = 0;
        if (e.key === "+" || e.key === "=" || e.key === "Add") delta = 1;
        else if (e.key === "-" || e.key === "_" || e.key === "Subtract") delta = -1;
        else if (e.key === "0") delta = "reset";
        else return;
        e.preventDefault();
        send({type: "zoomDoc", delta: delta});
    });

    // Shell -> page: apply the current zoom factor
    window.addEventListener("message", function (e) {
        if (e.data && e.data.type === "setZoom" && typeof e.data.zoom === "number") {
            document.documentElement.style.zoom = e.data.zoom;
        }
    });

    // Ask the shell for the current factor (a freshly loaded page starts at 1)
    send({type: "getZoom"});
})();
