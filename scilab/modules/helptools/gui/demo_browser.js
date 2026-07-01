/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systemes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

// ── State ──
var demoTree = null;       // full tree received from Scilab
var navStack = [];         // stack of indices for current path in tree
var searchMode = false;
var pendingNav = null;     // category name(s) to open once the tree is loaded
var appLabels = {
    title: "Demonstrations",
    home: "Home",
    search: "Search demos...",
    run: "Run",
    result: "result",
    results: "results",
    for_label: "for",
    no_result: "No demos match your search.",
    demo: "demo",
    demos: "demos"
};

// ── Color palette for category cards ──
var COLORS = [
    "#1565C0", "#2E7D32", "#E65100", "#6A1B9A",
    "#00838F", "#AD1457", "#F9A825", "#4E342E",
    "#283593", "#00695C", "#BF360C", "#4527A0",
    "#01579B", "#33691E", "#E91E63", "#546E7A"
];

// No icon map needed: we use the first letter of each category name.

// ── DOM refs ──
var content = document.getElementById("content");
var breadcrumb = document.getElementById("breadcrumb");
var searchInput = document.getElementById("search");
var searchClear = document.getElementById("search-clear");

// ── Entry point: receive data from Scilab ──
function fromScilab(msg) {
    if (msg.type === "demolist") {
        demoTree = msg.data;
        if (msg.labels) {
            appLabels = msg.labels;
        }
        if (msg.logo) {
            document.getElementById("logo").src = msg.logo;
        }
        document.title = appLabels.title;
        document.getElementById("title").textContent = appLabels.title;
        searchInput.placeholder = appLabels.search;
        navStack = [];
        render();
        // A navigation request may have arrived before the tree was ready.
        if (pendingNav !== null) {
            var target = pendingNav;
            pendingNav = null;
            navigateByNames(target);
        }
    } else if (msg.type === "theme") {
        setTheme(msg.theme || "light");
    } else if (msg.type === "navigate") {
        // Ask the browser to open a category by name (e.g. from xcos_demos.sce).
        // msg.path is a category name, or an array of names for a nested path.
        if (demoTree) {
            navigateByNames(msg.path);
        } else {
            pendingNav = msg.path;
        }
    }
}

// Resolve a path of category names to tree indices and navigate there.
// Matching is case-insensitive and tolerates surrounding whitespace.
function navigateByNames(names) {
    if (!demoTree) return;
    if (typeof names === "string") {
        names = [names];
    }
    if (!names || names.length === 0) return;

    var indices = [];
    var nodes = demoTree;
    for (var level = 0; level < names.length; level++) {
        var wanted = String(names[level]).trim().toLowerCase();
        var found = -1;
        for (var i = 0; i < nodes.length; i++) {
            if (String(nodes[i].name).trim().toLowerCase() === wanted) {
                found = i;
                break;
            }
        }
        if (found < 0) {
            // Unknown category: navigate as far as we could resolve.
            break;
        }
        indices.push(found);
        if (!nodes[found].children) {
            break;
        }
        nodes = nodes[found].children;
    }

    if (indices.length > 0) {
        navigateTo(indices);
    }
}

function setTheme(theme) {
    document.documentElement.setAttribute("data-theme", theme);
}

// ── Search ──
searchInput.addEventListener("input", function() {
    var q = this.value.trim();
    if (q.length >= 2) {
        searchMode = true;
        renderSearch(q);
    } else {
        searchMode = false;
        render();
    }
});

searchInput.addEventListener("keydown", function(e) {
    if (e.key === "Escape") {
        this.value = "";
        searchMode = false;
        render();
        this.blur();
    }
});

searchClear.addEventListener("click", function() {
    searchInput.value = "";
    searchMode = false;
    render();
    searchInput.focus();
});

// ── Navigation ──
function navigateTo(indices) {
    navStack = indices.slice();
    searchInput.value = "";
    searchMode = false;
    render();
}

function getCurrentNode() {
    var node = demoTree;
    for (var i = 0; i < navStack.length; i++) {
        node = node[navStack[i]];
        if (node.children) {
            node = node.children;
        }
    }
    return node;
}

function getNodeAtPath(indices) {
    var node = demoTree;
    for (var i = 0; i < indices.length; i++) {
        node = node[indices[i]];
        if (i < indices.length - 1 && node.children) {
            node = node.children;
        }
    }
    return node;
}

// ── Rendering ──
function render() {
    if (!demoTree) return;
    renderBreadcrumb();
    var items = getCurrentNode();

    // Check if this is a list (array) = multiple items at this level
    if (!Array.isArray(items)) {
        // Single node with children
        if (items.children) {
            renderMixedList(items.children);
        } else {
            renderMixedList([items]);
        }
        return;
    }

    // Determine if items are all leaf (runnable demos), all categories, or mixed
    var hasCategories = false;
    var hasLeafs = false;
    for (var i = 0; i < items.length; i++) {
        if (items[i].children) hasCategories = true;
        else hasLeafs = true;
    }

    renderMixedList(items);
}


function renderMixedList(items) {
    var cats = [];
    var demos = [];
    for (var i = 0; i < items.length; i++) {
        if (items[i].children) cats.push({item: items[i], idx: i});
        else demos.push({item: items[i], idx: i});
    }

    var html = '<div class="demo-list">';

    // Categories first
    for (var c = 0; c < cats.length; c++) {
        var cat = cats[c].item;
        var catIdx = cats[c].idx;
        var color = COLORS[(navStack.length * 3 + c) % COLORS.length];
        var iconHtml = renderIcon(cat, "small");
        var count = countLeafs(cat);
        html += '<div class="subcat-item" style="--i:' + c + '" onclick="navigateTo(' + JSON.stringify(navStack.concat(catIdx)) + ')">';
        html += '<div class="subcat-item-left">';
        html += '<div class="subcat-icon" style="background:' + color + '">' + iconHtml + '</div>';
        html += '<div>';
        html += '<div class="subcat-name">' + escapeHtml(cat.name) + '</div>';
        html += '<div class="subcat-count"><span class="subcat-count-badge">' + count + ' ' + (count > 1 ? appLabels.demos : appLabels.demo) + '</span></div>';
        html += '</div>';
        html += '</div>';
        html += '<div class="subcat-arrow">&#x203A;</div>';
        html += '</div>';
    }

    // Separate demos section when mixed with categories
    if (cats.length > 0 && demos.length > 0) {
        html += '<div class="demo-section-title">' + escapeHtml(appLabels.demos) + '</div>';
    }

    for (var d = 0; d < demos.length; d++) {
        var demo = demos[d].item;
        var colorDemo = COLORS[(navStack.length * 3 + cats.length + d) % COLORS.length];
        html += '<div class="demo-item" style="--i:' + (cats.length + d) + '">';
        html += '<div class="demo-item-left">';
        html += '<div class="demo-dot" style="background:' + colorDemo + '"></div>';
        html += '<div class="demo-item-name">' + escapeHtml(demo.name) + '</div>';
        html += '</div>';
        html += '<button class="btn-run" data-path="' + escapeAttr(demo.path || "") + '" onclick="runDemo(this.dataset.path)">&#x25B6; ' + escapeHtml(appLabels.run) + '</button>';
        html += '</div>';
    }

    html += '</div>';
    content.innerHTML = html;
    content.scrollTop = 0;
}

function renderBreadcrumb() {
    var html = '<a class="crumb" onclick="navigateTo([])">' + escapeHtml(appLabels.home) + '</a>';
    for (var i = 0; i < navStack.length; i++) {
        var path = navStack.slice(0, i + 1);
        var node = getNodeAtPath(path);
        html += '<span class="separator">&#x203A;</span>';
        if (i === navStack.length - 1) {
            html += '<span class="crumb current">' + escapeHtml(node.name) + '</span>';
        } else {
            html += '<a class="crumb" onclick="navigateTo(' + JSON.stringify(path) + ')">' + escapeHtml(node.name) + '</a>';
        }
    }
    breadcrumb.innerHTML = html;
}

// ── Search rendering ──
function renderSearch(query) {
    var results = [];
    searchTree(demoTree, [], [], query.toLowerCase(), results);

    var resultLabel = results.length === 1 ? appLabels.result : appLabels.results;
    var html = '<div class="search-results-title">' + results.length + ' ' + escapeHtml(resultLabel) + ' ' + escapeHtml(appLabels.for_label) + ' "' + escapeHtml(query) + '"</div>';
    html += '<div class="demo-list">';

    if (results.length === 0) {
        html += '<div class="empty-state"><p>' + escapeHtml(appLabels.no_result) + '</p></div>';
    }

    for (var i = 0; i < results.length; i++) {
        var r = results[i];
        var color = COLORS[i % COLORS.length];
        var highlighted = highlightMatch(r.name, query);

        if (r.isCategory) {
            html += '<div class="subcat-item" style="--i:' + i + '" onclick="navigateTo(' + JSON.stringify(r.navPath) + ')">';
            html += '<div class="subcat-item-left">';
            html += '<div class="subcat-icon" style="background:' + color + '">' + renderIcon(r, "small") + '</div>';
            html += '<div>';
            html += '<div class="subcat-name">' + highlighted + '</div>';
            html += '<div class="search-path-info">' + escapeHtml(r.breadcrumb) + '</div>';
            html += '</div>';
            html += '</div>';
            html += '<div class="subcat-arrow">&#x203A;</div>';
            html += '</div>';
        } else {
            html += '<div class="demo-item" style="--i:' + i + '">';
            html += '<div class="demo-item-left">';
            html += '<div class="demo-dot" style="background:' + color + '"></div>';
            html += '<div>';
            html += '<div class="demo-item-name">' + highlighted + '</div>';
            html += '<div class="search-path-info">' + escapeHtml(r.breadcrumb) + '</div>';
            html += '</div>';
            html += '</div>';
            html += '<button class="btn-run" data-path="' + escapeAttr(r.path || "") + '" onclick="runDemo(this.dataset.path)">&#x25B6; ' + escapeHtml(appLabels.run) + '</button>';
            html += '</div>';
        }
    }
    html += '</div>';
    content.innerHTML = html;
}

function searchTree(nodes, parentNames, parentIndices, query, results) {
    if (!Array.isArray(nodes)) return;
    for (var i = 0; i < nodes.length; i++) {
        var node = nodes[i];
        var match = node.name.toLowerCase().indexOf(query) >= 0;
        var crumb = parentNames.length > 0 ? parentNames.join(" > ") : "Home";
        var currentPath = parentIndices.concat([i]);

        if (match) {
            results.push({
                name: node.name,
                path: node.path || "",
                icon: node.icon || "",
                isCategory: !!node.children,
                breadcrumb: crumb,
                navPath: currentPath
            });
        }

        if (node.children) {
            searchTree(node.children, parentNames.concat(node.name), currentPath, query, results);
        }
    }
}

// ── Run demo ──
function runDemo(path) {
    toScilab({type: "run", path: path});
}

// ── Helpers ──
function countLeafs(node) {
    if (!node.children) return 1;
    var count = 0;
    for (var i = 0; i < node.children.length; i++) {
        count += countLeafs(node.children[i]);
    }
    return count;
}

function getInitial(name) {
    var trimmed = name.replace(/^\s+/, "");
    return trimmed.length > 0 ? trimmed.charAt(0).toUpperCase() : "?";
}

function renderIcon(node, size) {
    // If node has a custom icon path, render an <img>; otherwise render the initial letter.
    if (node.icon) {
        var cls = size === "large" ? "card-icon-img" : "subcat-icon-img";
        return '<img class="' + cls + '" src="' + escapeAttr(node.icon) + '">';
    }
    return getInitial(node.name);
}

function escapeHtml(str) {
    var div = document.createElement("div");
    div.appendChild(document.createTextNode(str));
    return div.innerHTML;
}

function escapeAttr(str) {
    return escapeHtml(str).replace(/"/g, "&quot;");
}

function highlightMatch(text, query) {
    var lower = text.toLowerCase();
    var idx = lower.indexOf(query.toLowerCase());
    if (idx < 0) return escapeHtml(text);
    var before = text.substring(0, idx);
    var match = text.substring(idx, idx + query.length);
    var after = text.substring(idx + query.length);
    return escapeHtml(before) + '<span class="search-highlight">' + escapeHtml(match) + '</span>' + escapeHtml(after);
}

// ── Keyboard shortcut ──
document.addEventListener("keydown", function(e) {
    // Ctrl+F or / to focus search
    if ((e.ctrlKey && e.key === "f") || (e.key === "/" && document.activeElement !== searchInput)) {
        e.preventDefault();
        searchInput.focus();
        searchInput.select();
    }
    // Backspace when not in search to go back
    if (e.key === "Backspace" && document.activeElement !== searchInput && navStack.length > 0) {
        navigateTo(navStack.slice(0, -1));
    }
});

// Demo tree is requested by Scilab when the browser uicontrol
// reports "loaded" via its Java-side onLoadEnd callback.
