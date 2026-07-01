//
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
// Copyright (C) 2026 - Dassault Systemes S.E. - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function demo_gui(category)
    // category (optional): name of the category to open directly, or a
    // string vector for a nested path (e.g. ["Xcos" "Standard demos"]).

    [lhs, rhs] = argn(0);
    if rhs < 1 then
        category = [];
    end

    global demolist;
    global demolistlock;
    if isempty(demolist) then
        if isempty(demolistlock) then
            demolistlock = %t;
            modules = getmodules();
            for i = 1:size(modules, "*")
                if isfile("SCI/modules/" + modules(i) + "/demos/" + modules(i) + ".dem.gateway.sce") then
                    exec("SCI/modules/" + modules(i) + "/demos/" + modules(i) + ".dem.gateway.sce", -1);
                end
            end
            clear demolistlock;
            clearglobal demolistlock;
        end
    end

    // localize demo names
    tmp = demolist;
    clear demolist;
    demolist = gettext(tmp);

    if get("scilab_demo_fig") <> [] then
        set("scilab_demo_fig", "visible", "on");
        // GUI already open: its tree is loaded, navigate right away.
        if ~isempty(category) then
            demo_gui_navigate(category);
        end
        return;
    end

    // Fresh GUI: the tree is sent asynchronously once the browser page
    // reports "loaded". Stash the request so demo_gui_send_tree() can
    // honor it right after sending the tree.
    if ~isempty(category) then
        global demo_gui_nav_request;
        demo_gui_nav_request = category;
    end

    // Figure
    demo_fig = figure( ...
        "figure_name", _("Demonstrations"), ...
        "figure_id", 100000, ...
        "infobar_visible", "off", ...
        "toolbar_visible", "off", ...
        "dockable", "off", ...
        "menubar", "none", ...
        "menubar_visible", "off", ...
        "default_axes", "off", ...
        "position", [50 50 900 620], ...
        "layout", "border", ...
        "icon", "x-office-presentation", ...
        "tag", "scilab_demo_fig", ...
        "visible", "off");

    fr = uicontrol(demo_fig, ...
        "style", "frame", ...
        "backgroundcolor", [1 1 1], ...
        "layout", "border");

    uicontrol(fr, ...
        "style", "browser", ...
        "string", SCI + "/modules/helptools/gui/demo_browser.html", ...
        "callback", "demo_gui_callback", ...
        "tag", "demo_browser");

    // Store the demolist on the browser for later use
    set("demo_browser", "userdata", demolist);

    demo_fig.closerequestfcn = "delete(get(""scilab_demo_fig""));";
    demo_fig.visible = "on";

endfunction
