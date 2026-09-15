/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 * Scilab Help Browser - Shell navigation
 *
 * Loads the TOC from toc.json, builds the tree, handles navigation
 * and search filtering.
 */
(function () {
    "use strict";

    var tocEl = document.getElementById("toc");
    var frame = document.getElementById("content-frame");
    var searchInput = document.getElementById("search");
    var resizer = document.getElementById("resizer");
    var sidebar = document.getElementById("sidebar");
    var breadcrumbEl = document.getElementById("breadcrumb");
    var activePage = null;
    var idIndex = {}; // xml:id -> page path
    var aliasOf = {}; // secondary xml:id of a page -> page id
    var pageInfo = {}; // xml:id -> {title, chapter, section}
    var history = [];
    var historyPos = -1;
    var navigatingHistory = false;
    var btnBack = document.getElementById("btn-back");
    var btnForward = document.getElementById("btn-forward");
    var btnHome = document.getElementById("btn-home");

    // ================================================================
    // TOC building
    // ================================================================

    var tocData = null; // store for index page generation
    // Directory of this shell page (modules/helptools/src/web), used to
    // resolve style.css for generated index/search pages.
    var XSLT_DIR = window.location.href.replace(/\/[^/]*$/, "").replace("file:///", "");

    function buildToc(data) {
        tocData = data;
        completionIds = null;
        var ul = document.createElement("ul");

        data.forEach(function (chapter) {
            var li = document.createElement("li");
            li.className = "toc-chapter collapsed";
            li.dataset.id = chapter.id;

            var label = document.createElement("div");
            label.className = "toc-label";
            label.innerHTML = '<span class="toc-toggle">&#9660;</span>' + escapeHtml(chapter.title);
            var chapterPath = [{title: chapter.title, node: chapter, parent: null}];
            label.addEventListener("click", (function (sp) {
                return function () {
                    li.classList.toggle("collapsed");
                    showIndexPageFromPath(sp);
                };
            })(chapterPath));
            li.appendChild(label);

            var childUl = buildSectionContents(chapter, [{title: chapter.title, node: chapter, parent: null}]);
            li.appendChild(childUl);
            ul.appendChild(li);
        });

        tocEl.appendChild(ul);
    }

    function buildSectionContents(node, breadcrumbPath) {
        var ul = document.createElement("ul");

        // Subsections first
        (node.sections || []).forEach(function (section) {
            var secLi = document.createElement("li");
            secLi.className = "toc-section collapsed";
            secLi.dataset.id = section.id;

            var secLabel = document.createElement("div");
            secLabel.className = "toc-label";
            secLabel.innerHTML = '<span class="toc-toggle">&#9660;</span>' + escapeHtml(section.title);
            var subPath = breadcrumbPath.concat([{title: section.title, node: section, parent: node}]);
            secLabel.addEventListener("click", (function (sp) {
                return function (e) {
                    e.stopPropagation();
                    secLi.classList.toggle("collapsed");
                    showIndexPageFromPath(sp);
                };
            })(subPath));
            secLi.appendChild(secLabel);

            // Recursive: build children (sub-sections + pages)
            var childUl = buildSectionContents(section, subPath);
            secLi.appendChild(childUl);
            ul.appendChild(secLi);
        });

        // Pages after sections
        (node.pages || []).forEach(function (page) {
            ul.appendChild(createPageItem(page));
            idIndex[page.id] = page.path;
            delete aliasOf[page.id];
            pageInfo[page.id] = {title: page.title, path: breadcrumbPath};
            // Secondary xml:ids (calmonths, calyears in caldays.xml): link
            // targets resolved to this page; a real page id always wins
            (page.aliases || []).forEach(function (alias) {
                if (!(alias in idIndex)) {
                    idIndex[alias] = page.path;
                    aliasOf[alias] = page.id;
                }
            });
        });

        return ul;
    }

    function createPageItem(page) {
        var li = document.createElement("li");
        li.className = "toc-page";
        li.textContent = page.title;
        li.dataset.path = page.path;
        li.dataset.id = page.id;
        li.dataset.search = (page.title + " " + (page.purpose || "")).toLowerCase();
        li.addEventListener("click", function () {
            navigateTo(page.path, li);
        });
        return li;
    }

    // ================================================================
    // Navigation
    // ================================================================

    function navigateTo(xmlPath, tocItem) {
        var tocId = tocItem ? tocItem.dataset.id : null;
        setView(function () {
            var item = tocId ? tocEl.querySelector('[data-id="' + tocId + '"].toc-page') : null;
            navigatingHistory = true;
            navigateTo(xmlPath, item);
            navigatingHistory = false;
        });
        if (typeof helpBrowserQuery === "function") {
            // Show loading while Java transforms + generates images
            frame.srcdoc = '<html><body style="display:flex;align-items:center;justify-content:center;height:100vh;margin:0;font-family:system-ui;color:#94a3b8"><p>Loading...</p></body></html>';
            // JCEF mode: ask Java to transform XML -> HTML (includes image generation)
            helpBrowserQuery({
                request: JSON.stringify({type: "transformXml", path: xmlPath}),
                onSuccess: function (htmlUrl) {
                    frame.removeAttribute("srcdoc");
                    frame.src = htmlUrl;
                },
                onFailure: function (code, msg) {
                    console.error("Transform failed:", msg);
                }
            });
        } else {
            // Standalone: load pre-generated HTML
            var htmlPath = xmlPath.replace(/\.xml$/, ".html");
            frame.src = htmlPath;
        }

        // Update active state
        if (activePage) activePage.classList.remove("active");
        if (tocItem) {
            tocItem.classList.add("active");
            activePage = tocItem;
            expandParents(tocItem);
            updateBreadcrumb(tocItem.dataset.id);
            // Scroll TOC to show active page
            tocItem.scrollIntoView({block: "nearest", behavior: "smooth"});
        }

        // Update history
        if (!navigatingHistory && tocItem) {
            // Truncate forward history
            history = history.slice(0, historyPos + 1);
            history.push({path: xmlPath, id: tocItem.dataset.id});
            historyPos = history.length - 1;
            updateNavButtons();
        }
    }

    function updateNavButtons() {
        btnBack.disabled = historyPos <= 0;
        btnForward.disabled = historyPos >= history.length - 1;
    }

    btnBack.addEventListener("click", function () {
        if (historyPos > 0) {
            historyPos--;
            navigatingHistory = true;
            var entry = history[historyPos];
            var tocItem = tocEl.querySelector('[data-id="' + entry.id + '"].toc-page');
            navigateTo(entry.path, tocItem);
            navigatingHistory = false;
            updateNavButtons();
        }
    });

    btnForward.addEventListener("click", function () {
        if (historyPos < history.length - 1) {
            historyPos++;
            navigatingHistory = true;
            var entry = history[historyPos];
            var tocItem = tocEl.querySelector('[data-id="' + entry.id + '"].toc-page');
            navigateTo(entry.path, tocItem);
            navigatingHistory = false;
            updateNavButtons();
        }
    });

    btnHome.addEventListener("click", function () {
        showHomePage();
    });

    // Navigate to a section index page from a "chapterId/secId/..." path
    // (used by the links of the generated index pages)
    function showSectionByPath(pathStr) {
        if (!tocData) return;
        var ids = pathStr.split("/");
        var path = [];
        var parent = null;
        for (var i = 0; i < ids.length; i++) {
            var list = (i === 0) ? tocData : (parent.sections || []);
            var found = null;
            for (var j = 0; j < list.length; j++) {
                if (list[j].id === ids[i]) { found = list[j]; break; }
            }
            if (!found) return;
            path.push({title: found.title, node: found, parent: parent});
            parent = found;
        }
        showIndexPageFromPath(path);
    }

    function showIndexPage(parentNode, section) {
        var path = [{title: parentNode.title, node: parentNode, parent: null}];
        if (section) {
            path.push({title: section.title, node: section, parent: parentNode});
        }
        showIndexPageFromPath(path);
    }

    function showIndexPageFromPath(path) {
        setView(function () { showIndexPageFromPath(path); });
        var lastEntry = path[path.length - 1];
        var node = lastEntry.node;
        var title = node.title;

        // Render the node hierarchy: direct pages, then each subsection as
        // a (clickable) heading with its own contents, recursively — the
        // index page mirrors the help tree like the old chapter pages did.
        function renderIndexNode(n, depth, idsPath) {
            var html = '';
            if (n.pages && n.pages.length) {
                html += '<ul style="list-style:disc;padding-left:1.5rem;line-height:1.8;margin:0 0 0.75rem">';
                n.pages.forEach(function (p) {
                    html += '<li><a href="#" data-linkend="' + escapeHtml(p.id) + '" style="color:#2563eb;text-decoration:none">'
                        + escapeHtml(p.title) + '</a>';
                    if (p.purpose) {
                        html += ' &#8212; ' + escapeHtml(p.purpose);
                    }
                    html += '</li>';
                });
                html += '</ul>';
            }
            (n.sections || []).forEach(function (s) {
                var level = Math.min(2 + depth, 4);
                var subIds = idsPath.concat([s.id]);
                html += '<h' + level + '><a href="#" data-secpath="' + escapeHtml(subIds.join('/'))
                    + '" style="color:inherit;text-decoration:none">' + escapeHtml(s.title) + '</a></h' + level + '>';
                html += renderIndexNode(s, depth + 1, subIds);
            });
            return html;
        }
        var idsPath = path.map(function (e) { return e.node.id; });

        var html = '<!DOCTYPE html><html><head><meta charset="UTF-8">'
            + '<link rel="stylesheet" href="file:///' + XSLT_DIR + '/style.css">'
            + '<script src="file:///' + XSLT_DIR + '/doczoom.js"><\/script>'
            + '<script src="file:///' + XSLT_DIR + '/docsearch.js"><\/script>'
            + '</head><body>'
            + '<article class="refentry" style="max-width:52rem;margin:0 auto;padding:1.5rem 2rem">'
            + '<h1 style="font-size:1.5rem;margin:0 0 1rem">' + escapeHtml(title) + '</h1>'
            + renderIndexNode(node, 0, idsPath)
            + '</article>'
            + '<script>document.addEventListener("click", function(e) {'
            + '  var a = e.target.closest("a[data-linkend]");'
            + '  if (a && window.parent && window.parent.scilabHelp) {'
            + '    e.preventDefault();'
            + '    window.parent.scilabHelp.navigateById(a.dataset.linkend);'
            + '    return;'
            + '  }'
            + '  var s = e.target.closest("a[data-secpath]");'
            + '  if (s && window.parent && window.parent.scilabHelp) {'
            + '    e.preventDefault();'
            + '    window.parent.scilabHelp.showSectionByPath(s.dataset.secpath);'
            + '  }'
            + '});<\/script>'
            + '</body></html>';

        frame.srcdoc = html;

        // Update breadcrumb in shell
        // Remove breadcrumb items but keep nav buttons
        Array.from(breadcrumbEl.children).forEach(function (c) {
            if (!c.classList.contains("nav-btn")) breadcrumbEl.removeChild(c);
        });
        path.forEach(function (entry, i) {
            if (i > 0) {
                var sep = document.createElement("span");
                sep.className = "sep";
                sep.innerHTML = "&#9656;";
                breadcrumbEl.appendChild(sep);
            }
            var span = document.createElement("span");
            span.className = (i === path.length - 1) ? "current" : "";
            span.textContent = entry.title;
            if (i < path.length - 1) {
                span.addEventListener("click", (function (subPath) {
                    return function () { showIndexPageFromPath(subPath); };
                })(path.slice(0, i + 1)));
            }
            breadcrumbEl.appendChild(span);
        });
        breadcrumbEl.classList.add("visible");
    }

    function updateBreadcrumb(id) {
        var info = pageInfo[id];
        if (!info) {
            breadcrumbEl.classList.remove("visible");
            return;
        }

        // Remove breadcrumb items but keep nav buttons
        Array.from(breadcrumbEl.children).forEach(function (c) {
            if (!c.classList.contains("nav-btn")) breadcrumbEl.removeChild(c);
        });

        // Each entry in info.path is {title, node, parent}
        info.path.forEach(function (entry, i) {
            if (i > 0) {
                var sep = document.createElement("span");
                sep.className = "sep";
                sep.innerHTML = "&#9656;";
                breadcrumbEl.appendChild(sep);
            }

            var span = document.createElement("span");
            span.textContent = entry.title;
            span.addEventListener("click", (function (subPath) {
                return function () { showIndexPageFromPath(subPath); };
            })(info.path.slice(0, i + 1)));
            breadcrumbEl.appendChild(span);
        });

        // Separator + current page
        var sep = document.createElement("span");
        sep.className = "sep";
        sep.innerHTML = "&#9656;";
        breadcrumbEl.appendChild(sep);

        var curSpan = document.createElement("span");
        curSpan.className = "current";
        curSpan.textContent = info.title;
        breadcrumbEl.appendChild(curSpan);

        breadcrumbEl.classList.add("visible");
    }

    function expandParents(el) {
        var parent = el.parentElement;
        while (parent && parent !== tocEl) {
            if (parent.classList.contains("toc-chapter") || parent.classList.contains("toc-section")) {
                parent.classList.remove("collapsed");
            }
            parent = parent.parentElement;
        }
    }

    /**
     * Navigate to a page by its xml:id (used for cross-references).
     */
    function navigateById(id) {
        var path = idIndex[id];
        if (!path) return;

        var tocItem = tocEl.querySelector('[data-id="' + (aliasOf[id] || id) + '"].toc-page');
        navigateTo(path, tocItem);
    }

    /**
     * Resolve a DocBook <link> target. Handles:
     *  - bare xml:id ("sum")                      -> navigate to that page
     *  - "scilab.help/sum" / "<toolbox>.help/page" -> navigate by page id
     *  - "scilab.xcos/...", ".scinotes/", ".demos/", ".exec/" and type="remote"
     *    -> resolved on the Java side (needs SCI / toolbox paths).
     */
    function resolveScilabLink(linkend, linkType) {
        if (!linkend) return;

        var slash = linkend.indexOf("/");
        if (slash < 0) {
            navigateById(linkend); // bare xml:id (native convention)
            return;
        }

        var location = linkend.substring(0, slash);
        var path = linkend.substring(slash + 1);
        var dot = location.indexOf(".");
        var subLoc = dot >= 0 ? location.substring(dot + 1) : location;

        if (subLoc === "help") {
            // Cross-reference to a help page; the page id is the last segment.
            var id = path.indexOf("/") >= 0 ? path.substring(path.lastIndexOf("/") + 1) : path;
            navigateById(id);
            return;
        }

        // Resource / action links: let Java resolve SCI / toolbox paths and act.
        if (typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "scilabLink", linkend: linkend, linkType: linkType || ""}),
                onSuccess: function () {},
                onFailure: function () {}
            });
        }
    }

    // ================================================================
    // Document zoom
    // ================================================================
    // Ctrl+wheel / Ctrl+'+'/'-'/'0' zoom the displayed page only, not the
    // shell (Chromium's own page zoom would scale the sidebar as well). The
    // factor lives here so that it survives page navigation; the page applies
    // it through doczoom.js (CSS zoom on its root element).

    var ZOOM_MIN = 0.5, ZOOM_MAX = 3, ZOOM_STEP = 1.1;
    var ZOOM_KEY = "scilabHelp.docZoom";
    var docZoom = 1;
    try {
        var saved = parseFloat(localStorage.getItem(ZOOM_KEY));
        if (saved >= ZOOM_MIN && saved <= ZOOM_MAX) docZoom = saved;
    } catch (e) { /* storage unavailable: start at 1 */ }

    function applyDocZoom() {
        if (frame.contentWindow) {
            frame.contentWindow.postMessage({type: "setZoom", zoom: docZoom}, "*");
        }
    }

    function setDocZoom(z) {
        docZoom = Math.min(ZOOM_MAX, Math.max(ZOOM_MIN, Math.round(z * 100) / 100));
        try { localStorage.setItem(ZOOM_KEY, String(docZoom)); } catch (e) { /* ignore */ }
        applyDocZoom();
    }

    // delta: +1 / -1 (one step), or "reset"
    function zoomDoc(delta) {
        if (delta === "reset") setDocZoom(1);
        else if (delta > 0) setDocZoom(docZoom * ZOOM_STEP);
        else if (delta < 0) setDocZoom(docZoom / ZOOM_STEP);
    }

    // Re-apply after each navigation (a new document starts at zoom 1)
    frame.addEventListener("load", applyDocZoom);

    // ================================================================
    // Find in page
    // ================================================================
    // Same widget as the console search. The bar lives in the shell; the
    // search itself runs in the page (docsearch.js) since the content iframe
    // is not same-origin: requests go through postMessage and the page
    // answers with a "findResult" message.

    var findBar = document.getElementById("find-bar");
    var findInput = document.getElementById("find-input");
    var findCase = document.getElementById("find-case");
    var findWord = document.getElementById("find-word");
    var findRegex = document.getElementById("find-regex");
    var findCount = document.getElementById("find-count");

    function postToPage(msg) {
        if (frame.contentWindow) frame.contentWindow.postMessage(msg, "*");
    }

    function isFindVisible() {
        return !findBar.hidden;
    }

    function runFind() {
        var term = findInput.value;
        if (term === "") {
            findCount.textContent = "";
            findBar.classList.remove("no-results");
            postToPage({type: "findClear"});
            return;
        }
        postToPage({
            type: "findInPage",
            term: term,
            caseSensitive: findCase.checked,
            wholeWord: findWord.checked,
            regex: findRegex.checked
        });
    }

    function onFindResult(count, current) {
        if (!isFindVisible() || findInput.value === "") return;
        if (count === 0) {
            findCount.textContent = "No results";
            findBar.classList.add("no-results");
        } else {
            findBar.classList.remove("no-results");
            findCount.textContent = (current >= 0 ? (current + 1) + " of " : "") + count;
        }
    }

    // seed: text to search (the page selection); undefined = ask the page.
    // Without selection the field keeps the previous search term.
    function showFind(seed) {
        findBar.hidden = false;
        if (typeof seed === "string") {
            if (seed !== "" && seed.indexOf("\n") < 0) findInput.value = seed;
        } else {
            postToPage({type: "findQuerySelection"});
        }
        findInput.focus();
        findInput.select();
        runFind();
    }

    function closeFind() {
        findBar.hidden = true;
        findCount.textContent = "";
        findBar.classList.remove("no-results");
        postToPage({type: "findClear"});
        frame.focus();
        if (frame.contentWindow) frame.contentWindow.focus();
    }

    function toggleFind(seed) {
        if (isFindVisible()) closeFind();
        else showFind(seed);
    }

    // F3 / Shift+F3: next or previous occurrence, opening the bar first
    function findStep(direction) {
        if (!isFindVisible()) {
            showFind();
            return;
        }
        postToPage({type: "findStep", direction: direction});
    }

    findInput.addEventListener("input", runFind);
    findInput.addEventListener("keydown", function (e) {
        if (e.key === "Enter" || e.key === "F3") {
            e.preventDefault();
            postToPage({type: "findStep", direction: e.shiftKey ? -1 : 1});
        } else if (e.key === "Escape") {
            e.preventDefault();
            closeFind();
        } else if ((e.ctrlKey || e.metaKey) && !e.altKey && (e.key === "f" || e.key === "F")) {
            // Ctrl+F in the field: keep the bar open, just reselect the term
            // (the console shortcut table does not see keys typed in its
            // search field either)
            e.preventDefault();
            e.stopPropagation();
            findInput.select();
        }
    });
    [findCase, findWord, findRegex].forEach(function (cb) {
        cb.addEventListener("change", runFind);
    });
    document.getElementById("find-close").addEventListener("click", closeFind);
    document.getElementById("find-prev").addEventListener("click", function () { findStep(-1); });
    document.getElementById("find-next").addEventListener("click", function () { findStep(1); });

    // Keys pressed in the shell itself (sidebar, breadcrumb)
    window.addEventListener("keydown", function (e) {
        if (e.key === "F3") {
            e.preventDefault();
            findStep(e.shiftKey ? -1 : 1);
        } else if ((e.ctrlKey || e.metaKey) && !e.altKey && (e.key === "f" || e.key === "F")) {
            e.preventDefault();
            toggleFind();
        }
    });

    // A newly loaded page has no highlight: search again if the bar is open
    frame.addEventListener("load", function () {
        if (isFindVisible() && findInput.value !== "") runFind();
    });

    // Same shortcuts when the sidebar / breadcrumb has the focus or the mouse
    window.addEventListener("wheel", function (e) {
        if (!e.ctrlKey || e.deltaY === 0) return;
        e.preventDefault();
        zoomDoc(e.deltaY < 0 ? 1 : -1);
    }, {passive: false});

    window.addEventListener("keydown", function (e) {
        if (!e.ctrlKey || e.altKey) return;
        var delta = 0;
        if (e.key === "+" || e.key === "=" || e.key === "Add") delta = 1;
        else if (e.key === "-" || e.key === "_" || e.key === "Subtract") delta = -1;
        else if (e.key === "0") delta = "reset";
        else return;
        e.preventDefault();
        zoomDoc(delta);
    });

    // Listen for messages from iframe via postMessage
    window.addEventListener("message", function (e) {
        if (!e.data || !e.data.type) return;

        if (e.data.type === "zoomDoc") {
            zoomDoc(e.data.delta);
        } else if (e.data.type === "getZoom") {
            applyDocZoom();
        } else if (e.data.type === "findResult") {
            onFindResult(e.data.count | 0, typeof e.data.current === "number" ? e.data.current : -1);
        } else if (e.data.type === "findToggle") {
            toggleFind(typeof e.data.seed === "string" ? e.data.seed : undefined);
        } else if (e.data.type === "findStep") {
            findStep(e.data.direction < 0 ? -1 : 1);
        } else if (e.data.type === "findSeed") {
            if (isFindVisible() && typeof e.data.seed === "string"
                    && e.data.seed !== "" && e.data.seed.indexOf("\n") < 0) {
                findInput.value = e.data.seed;
                findInput.select();
                runFind();
            }
        } else if (e.data.type === "openExternal" && e.data.url) {
            // Open in system browser via Java bridge
            if (typeof helpBrowserQuery === "function") {
                helpBrowserQuery({
                    request: JSON.stringify({type: "openExternal", url: e.data.url}),
                    onSuccess: function () {},
                    onFailure: function () {}
                });
            }
        } else if (e.data.type === "navigateById" && e.data.id) {
            resolveScilabLink(e.data.id, e.data.linkType || "");
        } else if (e.data.type === "execCode" && e.data.code && typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "execCode", code: e.data.code}),
                onSuccess: function () {},
                onFailure: function () {}
            });
        } else if (e.data.type === "editCode" && e.data.code && typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "editCode", code: e.data.code}),
                onSuccess: function () {},
                onFailure: function () {}
            });
        } else if (e.data.type === "openXcos" && e.data.file && typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "openXcos", file: e.data.file}),
                onSuccess: function () {},
                onFailure: function () {}
            });
        }
    });

    // ================================================================
    // Search
    // ================================================================

    var searchTimer = null;

    // The view shown in the frame, redisplayed when the full-text search
    // results are dismissed (the breadcrumb still describes it)
    var currentView = null;
    var showingSearchResults = false;

    function setView(redisplay) {
        currentView = redisplay;
        showingSearchResults = false;
    }

    function dismissSearchResults() {
        if (!showingSearchResults) return;
        if (currentView) {
            currentView();
        } else {
            showHomePage();
        }
    }

    searchInput.addEventListener("input", function () {
        clearTimeout(searchTimer);
        updateCompletion();
        if (!searchInput.value) {
            // Cleared (cross button): back to the page behind the results
            filterToc("");
            dismissSearchResults();
            return;
        }
        searchTimer = setTimeout(function () {
            filterToc(searchInput.value.trim().toLowerCase());
        }, 150);
    });

    searchInput.addEventListener("blur", hideCompletion);

    searchInput.addEventListener("keydown", function (e) {
        if (isCompletionVisible()) {
            if (e.key === "ArrowDown" || e.key === "ArrowUp") {
                e.preventDefault();
                moveCompletion(e.key === "ArrowDown" ? 1 : -1);
                return;
            } else if (e.key === "PageDown" || e.key === "PageUp") {
                e.preventDefault();
                moveCompletion(e.key === "PageDown" ? COMPLETION_PAGE : -COMPLETION_PAGE);
                return;
            } else if (e.key === "Enter" && completionSel >= 0) {
                e.preventDefault();
                acceptCompletion(completionSel);
                return;
            } else if (e.key === "Escape") {
                // First Escape closes the popup, the next one clears the field
                e.preventDefault();
                hideCompletion();
                return;
            }
        } else if ((e.key === "ArrowDown" && searchInput.value.trim())
                || (e.key === " " && e.ctrlKey)) {
            // Reopen the popup (after Escape), as Ctrl+Space in an editor
            e.preventDefault();
            updateCompletion();
            if (e.key === "ArrowDown") moveCompletion(1);
            return;
        }

        if (e.key === "Enter") {
            hideCompletion();
            var query = searchInput.value.trim();
            // Apply a pending filter now: the TOC keeps the matches
            clearTimeout(searchTimer);
            filterToc(query.toLowerCase());
            var pageId = findPageId(query);
            if (pageId) {
                // The query is a page id ("plot"): open it directly
                navigateById(pageId);
            } else if (query.length >= 2 && typeof helpBrowserQuery === "function") {
                // Full-text search via Java bridge
                helpBrowserQuery({
                    request: JSON.stringify({type: "fullTextSearch", query: query}),
                    onSuccess: function (response) {
                        try {
                            showSearchResults(query, JSON.parse(response));
                        } catch (ex) {}
                    },
                    onFailure: function () {
                        // Fallback: navigate to first TOC match
                        var first = tocEl.querySelector(".toc-page:not(.toc-hidden)");
                        if (first) first.click();
                    }
                });
            } else {
                var first = tocEl.querySelector(".toc-page:not(.toc-hidden)");
                if (first) first.click();
            }
            searchInput.blur();
        } else if (e.key === "Escape") {
            searchInput.value = "";
            filterToc("");
            dismissSearchResults();
            searchInput.blur();
        }
    });

    // ================================================================
    // Id completion
    // ================================================================
    // Popup under the search field listing the page ids (and secondary
    // ids) containing the typed text, those starting with it first, as in
    // an editor: Up/Down/PageUp/PageDown select, Enter or a click opens the
    // page, Escape closes the popup. Nothing is selected when the popup
    // opens, so that Enter on a plain word keeps doing the full-text search.

    var completionEl = document.getElementById("search-completion");
    var COMPLETION_MAX = 50;
    var COMPLETION_PAGE = 8;
    var completionIds = null; // all ids sorted case-insensitively, rebuilt with the TOC
    var completionItems = []; // ids listed in the popup
    var completionSel = -1;

    function sortedIds() {
        if (!completionIds) {
            completionIds = Object.keys(idIndex).map(function (id) {
                return {id: id, lower: id.toLowerCase()};
            }).sort(function (a, b) {
                return a.lower < b.lower ? -1 : a.lower > b.lower ? 1 : (a.id < b.id ? -1 : 1);
            });
        }
        return completionIds;
    }

    function isCompletionVisible() {
        return !completionEl.hidden;
    }

    function hideCompletion() {
        completionEl.hidden = true;
        completionItems = [];
        completionSel = -1;
        searchInput.setAttribute("aria-expanded", "false");
        searchInput.removeAttribute("aria-activedescendant");
    }

    function updateCompletion() {
        var q = searchInput.value.trim().toLowerCase();
        var prefix = [], inner = [];
        if (q) {
            sortedIds().forEach(function (e) {
                var i = e.lower.indexOf(q);
                if (i === 0) prefix.push(e.id);
                else if (i > 0) inner.push(e.id);
            });
        }
        var matches = prefix.concat(inner);
        if (matches.length === 0) {
            hideCompletion();
            return;
        }
        completionItems = matches.slice(0, COMPLETION_MAX);
        completionSel = -1;

        var html = "";
        completionItems.forEach(function (id, i) {
            var info = pageInfo[aliasOf[id] || id];
            var title = info ? info.title : "";
            var where = info && info.path.length ? info.path[info.path.length - 1].title : "";
            var tip = info ? info.path.map(function (p) { return p.title; }).concat([title]).join(" ▸ ") : id;
            html += '<li class="completion-item" role="option" id="completion-' + i + '" data-index="' + i + '"'
                + ' title="' + escapeHtml(tip) + '">'
                + '<span class="completion-id">' + markMatches(id, q) + '</span>';
            // Title shown when it says more than the id (secondary ids)
            if (title && title.toLowerCase() !== id.toLowerCase()) {
                html += '<span class="completion-title">' + escapeHtml(title) + '</span>';
            }
            html += '<span class="completion-where">' + escapeHtml(where) + '</span></li>';
        });
        if (matches.length > COMPLETION_MAX) {
            html += '<li class="completion-more" role="presentation">+ '
                + (matches.length - COMPLETION_MAX) + ' more&hellip;</li>';
        }
        completionEl.innerHTML = html;
        completionEl.scrollTop = 0;
        completionEl.hidden = false;
        searchInput.setAttribute("aria-expanded", "true");
        searchInput.removeAttribute("aria-activedescendant");
    }

    function selectCompletion(index, scroll) {
        var prev = completionEl.querySelector(".completion-item.selected");
        if (prev) prev.classList.remove("selected");
        completionSel = index;
        var li = document.getElementById("completion-" + index);
        if (li) {
            li.classList.add("selected");
            searchInput.setAttribute("aria-activedescendant", li.id);
            if (scroll) li.scrollIntoView({block: "nearest"});
        }
    }

    // Single steps wrap around the list, page steps stop at its ends
    function moveCompletion(delta) {
        var n = completionItems.length;
        if (n === 0) return;
        var i;
        if (completionSel < 0) {
            i = delta > 0 ? Math.min(delta, n) - 1 : Math.max(n + delta, 0);
        } else if (Math.abs(delta) === 1) {
            i = (completionSel + delta + n) % n;
        } else {
            i = Math.min(n - 1, Math.max(0, completionSel + delta));
        }
        selectCompletion(i, true);
    }

    function acceptCompletion(index) {
        var id = completionItems[index];
        hideCompletion();
        if (!id) return;
        // Same state as Enter on a typed id: the TOC filtered on it
        searchInput.value = id;
        clearTimeout(searchTimer);
        filterToc(id.toLowerCase());
        navigateById(id);
        searchInput.blur();
    }

    // Keep the focus in the search field while using the popup
    completionEl.addEventListener("mousedown", function (e) {
        e.preventDefault();
    });

    completionEl.addEventListener("mousemove", function (e) {
        var li = e.target.closest(".completion-item");
        if (li && +li.dataset.index !== completionSel) selectCompletion(+li.dataset.index, false);
    });

    completionEl.addEventListener("click", function (e) {
        var li = e.target.closest(".completion-item");
        if (li) acceptCompletion(+li.dataset.index);
    });

    // Page id (or secondary id) matching query, exact case first, then
    // case-insensitive ("Plot" -> "plot"); null if none.
    function findPageId(query) {
        if (!query) return null;
        if (Object.prototype.hasOwnProperty.call(idIndex, query)) return query;
        var q = query.toLowerCase();
        for (var id in idIndex) {
            if (Object.prototype.hasOwnProperty.call(idIndex, id) && id.toLowerCase() === q) {
                return id;
            }
        }
        return null;
    }

    // data: {total, terms, results: [{id, title, purpose, snippet, path}]},
    // the best pages first (ranked on the Java side, HelpSearchIndex)
    function showSearchResults(query, data) {
        var results = data.results || [];
        var terms = data.terms && data.terms.length ? data.terms : [query];
        var html = '<!DOCTYPE html><html><head><meta charset="UTF-8">'
            + '<link rel="stylesheet" href="file:///' + XSLT_DIR + '/style.css">'
            + '<script src="file:///' + XSLT_DIR + '/doczoom.js"><\/script>'
            + '<script src="file:///' + XSLT_DIR + '/docsearch.js"><\/script>'
            + '</head><body>'
            + '<article class="refentry" style="max-width:52rem;margin:0 auto;padding:1.5rem 2rem">'
            + '<h1 style="font-size:1.3rem;margin:0 0 1rem">Search results for &ldquo;' + escapeHtml(query) + '&rdquo;</h1>';

        if (results.length === 0) {
            html += '<p>No results found.</p>';
        } else {
            var total = data.total || results.length;
            html += '<p style="font-size:0.85rem;color:#64748b;margin:0 0 1rem">'
                + (total === 1 ? '1 page' : total + ' pages')
                + (total > results.length ? ', the ' + results.length + ' most relevant shown' : '') + '</p>';
            html += '<ul style="list-style:none;padding:0">';
            results.forEach(function (r) {
                html += '<li style="margin-bottom:0.8rem;padding-bottom:0.8rem;border-bottom:1px solid #e2e8f0">'
                    + '<a href="#" data-linkend="' + escapeHtml(r.id) + '" style="color:#2563eb;text-decoration:none;font-weight:600;font-size:0.95rem">'
                    + markMatches(r.title, terms) + '</a>';
                if (r.purpose) {
                    html += ' &mdash; ' + markMatches(r.purpose, terms);
                }
                var info = pageInfo[r.id];
                if (info && info.path.length) {
                    html += '<div style="font-size:0.8rem;color:#94a3b8">'
                        + escapeHtml(info.path.map(function (p) { return p.title; }).join(" ▸ ")) + '</div>';
                }
                if (r.snippet) {
                    html += '<div style="font-size:0.85rem;color:#64748b;margin-top:0.2rem">' + markMatches(r.snippet, terms) + '</div>';
                }
                html += '</li>';
            });
            html += '</ul>';
        }

        html += '</article>'
            + '<script>document.addEventListener("click", function(e) {'
            + '  var a = e.target.closest("a[data-linkend]");'
            + '  if (a && window.parent !== window) {'
            + '    e.preventDefault();'
            + '    window.parent.postMessage({type: "navigateById", id: a.dataset.linkend}, "*");'
            + '  }'
            + '});<\/script>'
            + '</body></html>';

        frame.srcdoc = html;
        breadcrumbEl.classList.remove("visible");
        showingSearchResults = true;
    }

    function filterToc(query) {
        var pages = tocEl.querySelectorAll(".toc-page");
        var sections = tocEl.querySelectorAll(".toc-section");
        var chapters = tocEl.querySelectorAll(".toc-chapter");

        if (!query) {
            // Show all, re-collapse
            pages.forEach(function (p) { p.classList.remove("toc-hidden"); });
            sections.forEach(function (s) {
                s.classList.remove("toc-hidden");
                s.classList.add("collapsed");
            });
            chapters.forEach(function (c) {
                c.classList.remove("toc-hidden");
                c.classList.add("collapsed");
            });
            // Keep active page's parents expanded
            if (activePage) expandParents(activePage);
            return;
        }

        // Hide/show pages based on match
        pages.forEach(function (p) {
            // Match title + purpose
            var searchText = p.dataset.search || p.textContent.toLowerCase();
            p.classList.toggle("toc-hidden", searchText.indexOf(query) === -1);
        });

        // Show sections that have visible pages
        sections.forEach(function (s) {
            var hasVisible = s.querySelector(".toc-page:not(.toc-hidden)");
            s.classList.toggle("toc-hidden", !hasVisible);
            if (hasVisible) s.classList.remove("collapsed");
        });

        // Show chapters that have visible content
        chapters.forEach(function (c) {
            var hasVisible = c.querySelector(".toc-page:not(.toc-hidden)");
            c.classList.toggle("toc-hidden", !hasVisible);
            if (hasVisible) c.classList.remove("collapsed");
        });
    }

    // ================================================================
    // Sidebar resize
    // ================================================================

    var isResizing = false;

    resizer.addEventListener("mousedown", function (e) {
        isResizing = true;
        resizer.classList.add("active");
        document.body.style.cursor = "col-resize";
        document.body.style.userSelect = "none";
        // Block iframe from stealing mouse events during resize
        frame.style.pointerEvents = "none";
        e.preventDefault();
    });

    document.addEventListener("mousemove", function (e) {
        if (!isResizing) return;
        var w = Math.min(500, Math.max(200, e.clientX));
        sidebar.style.width = w + "px";
    });

    document.addEventListener("mouseup", function () {
        if (isResizing) {
            isResizing = false;
            resizer.classList.remove("active");
            document.body.style.cursor = "";
            document.body.style.userSelect = "";
            frame.style.pointerEvents = "";
        }
    });

    // ================================================================
    // Init
    // ================================================================

    function escapeHtml(text) {
        var div = document.createElement("div");
        div.textContent = text;
        return div.innerHTML;
    }

    // Escape text and wrap the occurrences of the terms (a string or an
    // array of them, case-insensitive) in <mark class="search-hit">. An
    // occurrence inside a longer word ("table" in "unstable") is also
    // "partial", shown lighter so that such results can be dismissed at a
    // glance.
    var WORD_CHAR = /[\p{L}\p{N}_%]/u;
    function markMatches(text, terms) {
        terms = [].concat(terms || []).filter(Boolean).map(function (t) { return t.toLowerCase(); });
        if (!text || !terms.length) return escapeHtml(text || "");
        var lower = text.toLowerCase();
        var html = "", pos = 0;
        for (;;) {
            // Next occurrence of a term, the longest one at the same place
            var i = -1, len = 0;
            terms.forEach(function (t) {
                var k = lower.indexOf(t, pos);
                if (k >= 0 && (i < 0 || k < i || (k === i && t.length > len))) {
                    i = k;
                    len = t.length;
                }
            });
            if (i < 0) break;
            var j = i + len;
            var partial = (i > 0 && WORD_CHAR.test(text[i - 1]))
                || (j < text.length && WORD_CHAR.test(text[j]));
            html += escapeHtml(text.substring(pos, i))
                + '<mark class="search-hit' + (partial ? ' partial' : '') + '">'
                + escapeHtml(text.substring(i, j)) + '</mark>';
            pos = j;
        }
        return html + escapeHtml(text.substring(pos));
    }

    // Load TOC
    function loadToc() {
        // Try JCEF bridge first
        if (typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "getToc"}),
                onSuccess: function (response) {
                    try {
                        buildToc(JSON.parse(response));
                        onTocReady();
                    } catch (e) {
                        tocEl.textContent = "Failed to parse TOC: " + e.message;
                    }
                },
                onFailure: function (code, msg) {
                    loadTocViaFetch();
                }
            });
        } else {
            loadTocViaFetch();
        }
    }

    function loadTocViaFetch() {
        fetch("toc.json")
            .then(function (r) { return r.json(); })
            .then(function (data) { buildToc(data); onTocReady(); })
            .catch(function (err) {
                tocEl.textContent = "Failed to load TOC: " + err.message;
            });
    }

    loadToc();

    // Pending navigation (if called before TOC is ready)
    var pendingNavigateId = null;
    var tocReady = false;

    function navigateByIdSafe(id) {
        if (id === "Whatsnew" || id === "homepage") {
            if (tocReady) {
                showHomePage();
            } else {
                pendingNavigateId = "__home__";
            }
            return;
        }
        if (tocReady) {
            navigateById(id);
        } else {
            pendingNavigateId = id;
        }
    }

    function onTocReady() {
        tocReady = true;
        if (pendingNavigateId === "__home__" || !pendingNavigateId) {
            pendingNavigateId = null;
            showHomePage();
        } else if (pendingNavigateId) {
            var id = pendingNavigateId;
            pendingNavigateId = null;
            navigateById(id);
        }
    }

    function showHomePage() {
        setView(showHomePage);
        if (typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "getHomePage"}),
                onSuccess: function (url) {
                    frame.removeAttribute("srcdoc");
                    frame.src = url;
                },
                onFailure: function () {}
            });
        }
        breadcrumbEl.classList.remove("visible");
    }

    // Rebuild the TOC tree from a freshly generated toc.json (e.g. after a
    // toolbox registered a new help chapter). The currently displayed page is
    // left untouched.
    function reloadToc() {
        tocEl.innerHTML = "";
        idIndex = {};
        aliasOf = {};
        pageInfo = {};
        completionIds = null;
        activePage = null;
        if (typeof helpBrowserQuery === "function") {
            helpBrowserQuery({
                request: JSON.stringify({type: "getToc"}),
                onSuccess: function (response) {
                    try {
                        buildToc(JSON.parse(response));
                    } catch (e) { /* keep old (empty) tree on parse error */ }
                },
                onFailure: function () {}
            });
        } else {
            loadTocViaFetch();
        }
    }

    // Expose for JCEF bridge
    window.scilabHelp = {
        navigateById: navigateByIdSafe,
        navigateTo: navigateTo,
        search: function(query) {
            searchInput.value = query;
            filterToc(query.toLowerCase());
        },
        showHomePage: showHomePage,
        showSectionByPath: showSectionByPath,
        reloadToc: reloadToc,
        zoomIn: function () { zoomDoc(1); },
        zoomOut: function () { zoomDoc(-1); },
        resetZoom: function () { zoomDoc("reset"); },
        showFind: function () { showFind(); },
        closeFind: closeFind,
        // Preferences > Colors changed: the page re-fetches them (scilab-init.js)
        syntaxColorsChanged: function () { postToPage({type: "syntaxColorsChanged"}); },
    };

})();
