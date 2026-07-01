//
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systemes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.
//

function demo_gui_callback(msg, cb)
    // Callback for the demo browser uicontrol.
    // Receives messages from JavaScript and responds accordingly.

    // The browser uicontrol may have been destroyed between the moment the
    // event was queued and the moment Scilab executes this callback. The
    // generic Java-side guard skips most stale events, but the figure can
    // still be closed while we are running here.
    if isempty(get("scilab_demo_fig")) then
        return;
    end

    if type(msg) == 10 then
        if msg == "loaded" then
            demo_gui_send_tree();
        end
        return;
    end

    select msg.type
    case "loaded"
        demo_gui_send_tree();
    case "run"
        // Run a demo script
        path = demo_gui_sanitize_incoming_path(msg.path);
        path = pathconvert(path, %f, %t);
        if ~isfile(path) then
            path2 = pathconvert(path, %f, %t);
            if isfile(path2) then
                path = path2;
            end
        end
        if isfile(path) then
            demo_gui_close_demo_figures();
            vars1 = who("scope");
            exec(path, -1);
            vars2 = who("scope");
            d = setdiff(vars2, vars1);
            d(d == "vars1") = [];
            if ~isempty(d) then
                l = strcat(d, ", ");
                execstr("[" + l + "] = resume(" + l + ");");
            end
        end
    end
endfunction

function demo_gui_send_tree()
    demolist = get("demo_browser", "userdata");

    // find introduction category so that it's always at the beginning
    idx = find(demolist(:,1) == _("Introduction: Getting started with Scilab"));
    // introduction
    tree = demo_gui_build_tree(demolist(idx, :));
    tree = demo_gui_sort_nodes(tree);
    demolist(idx, :) = [];
    // the others
    tree2 = demo_gui_build_tree(demolist);
    tree2 = demo_gui_sort_nodes(tree2);

    tree = lstcat(tree, tree2);

    labels = struct( ...
        "title", _("Demonstrations"), ...
        "home", "Home", ...
        "search", _("Search demos..."), ...
        "run", _("Run"), ...
        "result", _("result"), ...
        "results", _("results"), ...
        "for_label", _("for"), ...
        "no_result", _("No demos match your search."), ...
        "demo", _("demo"), ...
        "demos", _("demos"));

    data = struct( ...
        "type", "demolist", ...
        "data", tree, ...
        "labels", labels);
    set("demo_browser", "data", data);

    // A demo launcher (e.g. xcos_demos.sce) may have asked to open a given
    // category. The browser only has the tree now, so honor the request here.
    global demo_gui_nav_request;
    if ~isempty(demo_gui_nav_request) then
        req = demo_gui_nav_request;
        clearglobal demo_gui_nav_request;
        demo_gui_navigate(req);
    end
endfunction

function nodes = demo_gui_sort_nodes(nodes)
    n = length(nodes);
    if n <= 1 then
        return;
    end

    // Sort recursively first.
    for i = 1:n
        if isfield(nodes(i), "children") then
            nodes(i).children = demo_gui_sort_nodes(nodes(i).children);
        end
    end

    names = emptystr(n, 1);
    for i = 1:n
        names(i) = convstr(string(nodes(i).name), "l");
    end

    [dummy, k] = gsort(names, "g", "i");
    sortedNodes = list();
    for i = 1:n
        sortedNodes(i) = nodes(k(i));
    end
    nodes = sortedNodes;
endfunction

function demo_gui_close_demo_figures()
    // Close figures opened by demos, while keeping the browser window.
    all_figs = winsid();
    all_figs = all_figs(all_figs >= 100001);
    for fig_id = all_figs
        fig_to_del = findobj("figure_id", fig_id);
        if ~isempty(fig_to_del) then
            delete(fig_to_del);
        end
    end
endfunction

function tree = demo_gui_build_tree(demolist)
    // Build a nested tree structure from the flat demolist.
    // demolist is [name, path] (legacy 2-col) or [name, path, description, icon] (4-col).
    // If path ends with "dem.gateway.sce", it has children (sub-demos).
    // We recursively expand gateways to build the full tree.

    global subdemolist;
    tree = list();
    nc = size(demolist, 2);

    for i = 1:size(demolist, 1)
        name = demolist(i, 1);
        path = demolist(i, 2);
        path = demo_gui_resolve_path(path, "");

        node = struct("name", name);

        // Add description and icon when available (4-col demolist)
        if nc >= 3 && demolist(i, 3) <> "" then
            node.description = demolist(i, 3);
        end
        if nc >= 4 && demolist(i, 4) <> "" then
            node.icon = demo_gui_browser_path(demolist(i, 4));
        end

        if grep(path, "dem.gateway.sce") <> [] then
            children = demo_gui_exec_gateway(path);
            if children <> [] then
                node.children = demo_gui_build_children(children, path);
            end
        else
            node.path = demo_gui_browser_path(path);
        end

        tree($ + 1) = node;
    end

    clearglobal subdemolist;
endfunction

function childTree = demo_gui_build_children(sublist, parent_gateway_path)
    // Build children list from a subdemolist matrix [name, path; ...]
    global subdemolist;
    childTree = list();

    for j = 1:size(sublist, 1)
        cname = sublist(j, 1);
        cpath = sublist(j, 2);
        cpath = demo_gui_resolve_path(cpath, parent_gateway_path);
        child = struct("name", cname);

        if grep(cpath, "dem.gateway.sce") <> [] then
            subchildren = demo_gui_exec_gateway(cpath);
            if subchildren <> [] then
                child.children = demo_gui_build_children(subchildren, cpath);
            end
        else
            child.path = demo_gui_browser_path(cpath);
        end

        childTree($ + 1) = child;
    end
endfunction

function path = demo_gui_resolve_path(path, parent_gateway_path)
    // Keep path unchanged when it already points to an existing file.
    if isfile(path) then
        return;
    end

    // Try normalized separators first.
    path2 = pathconvert(path, %f, %t);
    if isfile(path2) then
        path = path2;
        return;
    end

    // Resolve relative paths against the parent gateway directory.
    if parent_gateway_path <> "" then
        base = get_absolute_file_path(parent_gateway_path);
        path3 = base + path;
        if isfile(path3) then
            path = path3;
            return;
        end

        path4 = pathconvert(path3, %f, %t);
        if isfile(path4) then
            path = path4;
            return;
        end
    end
endfunction

function path = demo_gui_browser_path(path)
    // Browser bridge interprets backslash escapes (\f, \t, ...).
    // Send only "/" separators to JavaScript.
    path = strsubst(path, "\", "/");

    // Normalize duplicated separators in local paths: E://foo//bar -> E:/foo/bar
    isUNC = part(path, 1:min(2, length(path))) == "//";
    while grep(path, "//") <> [] then
        path = strsubst(path, "//", "/");
    end

    // Preserve UNC form when applicable.
    if isUNC then
        path = "/" + path;
    end
endfunction

function path = demo_gui_sanitize_incoming_path(path)
    // Replace control chars that may come from escaped backslash sequences.
    bad = [9 10 11 12 13];
    for k = 1:size(bad, "*")
        path = strsubst(path, ascii(bad(k)), "/");
    end
endfunction

function result = demo_gui_exec_gateway(path)
    // Execute a dem.gateway.sce and return its subdemolist
    global subdemolist;
    result = [];

    if ~isfile(path) then
        return;
    end

    subdemolist = [];
    ierr = execstr("exec(path, -1);", "errcatch");
    if ierr == 0 && ~isempty(subdemolist) then
        result = gettext(subdemolist);
    end
endfunction
