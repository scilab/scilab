// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport(filepath)

    fname = "uiimport";
    if nargin > 1 then
        error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 0, 1));
    end

    win = get("uiimport");
    if ~isempty(win) then
        set(win, "visible", "on");
        if isdef("filepath", "l") then
            m = messagebox(["The file import will erase previous data."; "Do you want to continue ?"], "Warning", "warning", ["Yes", "No"], "modal");
            if m == 1 then
                close(win);
            else
                return;
             end
        else
            return;
        end
    end

    if isdef("filepath", "l") then
        if type(filepath) <> 10 then
            error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1));
        end

        if ~isfile(filepath) then
            error(msprintf(_("%s: Wrong value for input argument #%d: A filename expected.\n"), fname, 1));
        end

        if and(fileext(filepath) <> [".csv" ".txt"]) then
            error(msprintf(_("%s: Wrong file type: csv or txt file expected.\n"), fname));
        end
    else
        filepath = "";
    end


    uiimport_gui(filepath);
endfunction

// -----------------------------------------------------------------------------
function uiimport_gui(data)

    w = 1100;
    h = 675;

    fig = figure(...
        "figure_name", _("Import data"), ...
        "dockable", "off", ...
        "axes_size", [w h], ...
        "infobar_visible", "off", ...
        "toolbar_visible", "off", ...
        "menubar_visible", "off", ...
        "default_axes", "off", ...
        "background", -2, ...
        "layout", "border", ...
        "tag", "uiimport", ...
        "icon", "text-csv", ...
        "visible", "off");

    // store state in figure userdata
    data = struct( ...
        "path", filepath, ...
        "opts", [], ...
        "numheaderlines", [], ...
        "varnames", [], ...
        "keepcols", [], ...
        "rowtimes", "No", ...
        "convert", [], ...
        "outputFormat", [], ...
        "formatRef", [], ...
        "nbrows", 0, ...
        "nbcols", 0, ...
        "filetext", []);

    fig.userdata = data;

    frame = uicontrol(fig, ...
        "style", "frame", ...
        "layout", "border", ...
        "constraints", createConstraints("border", "center"));

    htmlpath = fullfile(SCI, "modules", "ui_data", "etc", "uiimport.html");

    uicontrol(frame, ...
        "style", "browser", ...
        "debug", "on", ...
        "string", htmlpath, ...
        "callback", "uiimport_cb", ...
        "tag", "uiimport_browser");

    userUnits = get(0,"Units");
    set(0,"Units","pixels");
    sw = get(0, "screensize")(3);
    sh = get(0, "screensize")(4);
    set(0,"Units",userUnits);


    s = fig.figure_size;
    w = (sw - s(1)) / 2
    h = (sh - s(2)) / 2
    fig.figure_position = [w h];
    fig.visible = "on";

endfunction
