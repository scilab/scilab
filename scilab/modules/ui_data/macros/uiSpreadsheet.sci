// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - ESI Group - Antoine ELIAS
// Copyright (C) 2021 - Samuel Gougeon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function uiSpreadsheet(action)
    // action: private internal argument

    if isdef("action","l")
        select action
        case "cbselect"
            uispreadsheet_cbselect()

        case "preview"
            uispreadsheet_preview()

        case "variable"
            uispreadsheet_variable()
            if isdef("names","l")
                if isdef("header","l")
                    str = msprintf("[%s] = resume(data,header)\n", names)
                else
                    str = msprintf("[%s] = resume(data)\n", names)
                end
                execstr(str)
            end

        case "function"
            uispreadsheet_function()
        end
        return
    end
    // ------------------------------

    win = get("uispreadsheet");
    if ~isempty(win) then
        //set visible
        set("win", "visible", "on");
        //set on top
        set("win");
        return;
    end

    w = 800;
    h = 600;

    fig = figure(...
        "figure_name", _("Import data"), ...
        "dockable", "off", ...
        "axes_size", [w h], ...
        "infobar_visible", "off", ...
        "toolbar_visible", "off", ...
        "menubar_visible", "off", ...
        "default_axes", "off", ...
        "background", -2, ...
        "resize", "off", ...
        "layout", "border", ...
        "tag", "uispreadsheet", ...
        "visible", "off");

    //Border frame
    t = uicontrol(fig, ...
        "style", "frame", ...
        "constraints", createConstraints("border", "top", [0 90]));

    c = uicontrol(fig, ...
        "style", "frame", ...
        "layout", "border", ...
        "constraints", createConstraints("border", "center"));

    uicontrol(c, ...
        "style", "frame", ...
        "tag", "uispreadsheet_preview", ...
        "scrollable", "on", ...
        "layout", "gridbag", ...
        "constraints", createConstraints("border", "center"));

    b = uicontrol(fig, ...
        "style", "frame", ...
        "constraints", createConstraints("border", "bottom", [0 45]));

    //frame file
    fr1 = uicontrol(t, ...
        "style", "frame", ...
        "position", [0 0 800 90]);

    uicontrol(fr1, ...
        "style", "text", ...
        "position", [20 50 560 30], ...
        "horizontalalignment", "center", ...
        "relief", "solid", ...
        "fontsize", 13, ...
        "fontweight", "bold", ...
        "tag", "uispreadsheet_filename")

    uicontrol(fr1, ...
        "style", "pushbutton", ...
        "string", _("Select a file"), ...
        "callback", "uiSpreadsheet(""cbselect"")", ...
        "position", [600 50 180 30]);

    uicontrol(fr1, ...
        "style", "checkbox", ...
        "string", _("File has a header"), ...
        "tag", "uispreadsheet_header", ...
        "callback", "uiSpreadsheet(""preview"")", ...
        "position", [20 20 130 25]);

    uicontrol(fr1, ...
        "style", "checkbox", ...
        "string", _("Show all columns"), ...
        "tooltip", _("It can take a long time"), ...
        "tag", "uispreadsheet_showcol", ...
        "callback", "uiSpreadsheet(""preview"")", ...
        "position", [20 0 130 25]);

    uicontrol(fr1, ...
        "style", "text", ...
        "string", _("Separator"), ...
        "horizontalalignment", "center", ...
        "position", [180 10 60 25]);

        uicontrol(fr1, ...
        "style", "popupmenu", ...
        "string", [_("Comma"), _("Space"), _("Tab"), _("Semicolon"), _("Pipe"), _("Colon")], ...
        "userdata", [",", " ", ascii(9), ";", "|", ":"], ...
        "value", 4, ...
        "tag", "uispreadsheet_delim", ...
        "callback", "uiSpreadsheet(""preview"")", ...
        "position", [240 10 120 25]);

    uicontrol(fr1, ...
        "style", "text", ...
        "string", _("Decimal"), ...
        "horizontalalignment", "center", ...
        "position", [390 10 60 25]);

    uicontrol(fr1, ...
        "style", "popupmenu", ...
        "string", [_("Point"), _("Comma")], ...
        "userdata", [".", ","], ...
        "value", 1, ...
        "tag", "uispreadsheet_decimal", ...
        "callback", "uiSpreadsheet(""preview"")", ...
        "position", [450 10 120 25]);

    uicontrol(fr1, ...
        "style", "text", ...
        "string", _("Conversion"), ...
        "horizontalalignment", "center", ...
        "position", [600 10 60 25]);

    uicontrol(fr1, ...
        "style", "popupmenu", ...
        "string", [_("Keep as strings"), _("Convert to double")], ...
        "userdata", ["string", "double"], ...
        "value", 1, ...
        "tag", "uispreadsheet_conversion", ...
        "callback", "uiSpreadsheet(""preview"")", ...
        "position", [660 10 120 25]);


    //footer
    outer = uicontrol(b, ...
        "style", "frame", ...
        "position", [0 40 800 5], ...
        "border", createBorder("line", "#dddddd"));

    uicontrol(outer, ...
        "style", "frame", ...
        "backgroundcolor", [0 120 215]./255, ...
        "tag", "uispreadsheet_progressbar", ...
        "position", [0 0 0 5]);

    uicontrol(b, ...
        "style", "pushbutton", ...
        "string", _("Import as variable"), ...
        "callback", "uiSpreadsheet(""variable"")", ...
        "enable", "off", ...
        "tag", "uispreadsheet_btnvarialble", ...
        "position", [50 5 200 30]);

    uicontrol(b, ...
        "style", "pushbutton", ...
        "string", _("Quit"), ...
        "callback", "delete(get(""uispreadsheet""));", ...
        "position", [300 5 200 30]);

    uicontrol(b, ...
        "style", "pushbutton", ...
        "string", _("Copy function to clipboard"), ...
        "callback", "uiSpreadsheet(""function"")", ...
        "enable", "off", ...
        "tag", "uispreadsheet_btnfunction", ...
        "position", [550 5 200 30]);

    sw = get(0,"screensize_px")(3);
    sh = get(0,"screensize_px")(4);

    s = fig.figure_size;
    w = (sw - s(1)) / 2
    h = (sh - s(2)) / 2
    fig.figure_position = [w h];

    fig.visible = "on";
    uispreadsheet_cbselect();
endfunction

// -----------------------------------------------------------------------------

function head = uispreadsheet_gethead(f, n)
    head = mgetl(f, n);
endfunction

// -----------------------------------------------------------------------------

function uispreadsheet_progress(val)
    pos = get("uispreadsheet_progressbar", "position");
    pos(3) = 800 * val;
    set("uispreadsheet_progressbar", "position", pos);
endfunction

// -----------------------------------------------------------------------------

function uispreadsheet_variable()
    hasHeader = get("uispreadsheet_header", "value");
    delim = get("uispreadsheet_delim", "userdata")(get("uispreadsheet_delim", "value"));
    decimal = get("uispreadsheet_decimal", "userdata")(get("uispreadsheet_decimal", "value"));
    conversion = get("uispreadsheet_conversion", "userdata")(get("uispreadsheet_conversion", "value"));

    path = get("uispreadsheet_filename", "string");
    if isempty(path) || ~isfile(path) then
        return;
    end

    importHeader = %f;

    if hasHeader then
        x = x_mdialog("Import Data", ["Variable name";"Import header";"Header name"], ["data";"%F";"header"]);
        if isempty(x) then
            return;
        end

        importHeader = evstr(x(2));
        if importHeader then
            header = mgetl(path, 1);
            header = csvTextScan(header, delim, decimal, "string");
            names = strcat([x(1) x(3)],",")
            str = "[data, header, names] = resume(data, header, names)";
        else
            names = x(1)
            str = "[data, names] = resume(data, names)";
        end

        dataName = x(1);
        data = csvRead(path, delim, decimal, conversion, [], [], [], 1);
    else
        x = x_mdialog("Import Data", ["Variable name"], ["data"]);
        if isempty(x) then
            return;
        end

        names = x(1)
        str = "[data, names] = resume(data, names)";
        dataName = x(1);
        data = csvRead(path, delim, decimal, conversion);
    end

    execstr(str);
endfunction

// -----------------------------------------------------------------------------

function uispreadsheet_cbselect()

    path = pwd();
    f = get("uispreadsheet_filename", "string");
    if ~isempty(f) then
        path = fileparts(f);
    end


    path = uigetfile(["*.txt" "Text files";"*.csv" "CSV files"], path, "Choose a file", %f);
    if ~isempty(path) then
        set("uispreadsheet_filename", "string", path);
        delim = uispreadsheet_analyze(path, 10);
        if delim then
            set("uispreadsheet_delim", "value", delim);
            if delim == 1 then
                set("uispreadsheet_decimal", "value", 1);
            end
        end

        uispreadsheet_preview();
    end
endfunction

// -----------------------------------------------------------------------------

function delim = uispreadsheet_analyze(f, n)

    if ~exists("n", "local") then
        n = 10;
    end

    head = mgetl(f, n);
    n = size(head, "r");

    delimiters = get("uispreadsheet_delim", "userdata");
    count = zeros(delimiters);

    for i = 1:n
        l = head(i);
        for j = 1:size(delimiters, "*")
            r = strindex(l, delimiters(j));
            if size(r, "*") > 1 then
                count(j) = count(j) + 1;
            end
        end
    end

    [m, k] = max(count);

    if m == n then
        delim = k;
    else
        //unable to find good delimiter.
        delim = 0;
        return;
    end
endfunction

// -----------------------------------------------------------------------------

function uispreadsheet_function()

    hasHeader = get("uispreadsheet_header", "value");
    delim = get("uispreadsheet_delim", "userdata")(get("uispreadsheet_delim", "value"));
    decimal = get("uispreadsheet_decimal", "userdata")(get("uispreadsheet_decimal", "value"));
    conversion = get("uispreadsheet_conversion", "userdata")(get("uispreadsheet_conversion", "value"));

    path = get("uispreadsheet_filename", "string");

    importHeader = %f;

    if hasHeader then
        x = x_mdialog("Create function", ["Function name";"Import header"], ["importdata";"%F"]);
        if isempty(x) then
            return;
        end

        importHeader = evstr(x(2));
    else
        x = x_mdialog("Create function", ["Function name"], ["importdata"]);
        if isempty(x) then
            return;
        end
    end


    str = [];
    str($+1) = sprintf("clear %s;", x(1));
    str($+1) = "";
    if importHeader then
        str($+1) = sprintf("function [header, data] = %s(filename)", x(1));
        str($+1) = "    header = mgetl(filename, 1);";
        str($+1) = sprintf("    header = csvTextScan(header, ""%s"", ""%s"", ""string"");", delim, decimal);
    else
        str($+1) = sprintf("function [data] = %s(filename)", x(1));
    end

    if hasHeader then
        str($+1) = sprintf("    data = csvRead(filename, ""%s"", ""%s"", ""%s"", [], [], [], 1);", delim, decimal, conversion);
    else
        str($+1) = sprintf("    data = csvRead(filename, ""%s"", ""%s"", ""%s"");", delim, decimal, conversion);
    end

    str($+1) = "endfunction";
    str($+1) = "";

    if importHeader then
        str($+1) = sprintf("[header, data] = importdata(""%s"");", path);
    else
        str($+1) = sprintf("[data] = importdata(""%s"");", path);
    end

    clipboard("copy", str);
endfunction

// -----------------------------------------------------------------------------

function uispreadsheet_preview()
    global %uispreadsheet_cancel;
    //get window information
    delim = get("uispreadsheet_delim", "value")
    if delim == 1 then
        set("uispreadsheet_decimal", "value", 1);
    end

    set("uispreadsheet_btnvarialble", "enable", "off");
    set("uispreadsheet_btnfunction", "enable", "off");

    hasHeader = get("uispreadsheet_header", "value");
    delim = get("uispreadsheet_delim", "userdata")(delim);
    decimal = get("uispreadsheet_decimal", "userdata")(get("uispreadsheet_decimal", "value"));
    conversion = get("uispreadsheet_conversion", "userdata")(get("uispreadsheet_conversion", "value"));

    path = get("uispreadsheet_filename", "string");

    if isempty(path) || ~isfile(path) then
        return;
    end

    c = get("uispreadsheet_preview");
    c.visible = "off";
    delete(c.children);

    parent = c.parent;
    fr = uicontrol(parent, ...
        "style", "frame", ...
        "layout", "gridbag", ...
        "tag", "uispreadsheet_cancel", ...
        "constraints", createConstraints("border", "center"));

    uicontrol(fr, ...
        "style", "pushbutton", ...
        "string", _("Cancel preview"), ...
        "callback_type", 10, ...
        "callback", "global %uispreadsheet_cancel;%uispreadsheet_cancel=%t;", ...
        "constraints", createConstraints("gridbag", [1 1 1 1], [1, 1]));

    txt = mgetl(path, 20 + hasHeader);
    if hasHeader then
        header = txt(1);
        txt(1) = [];

        //process header
        try
            header = csvTextScan(header, delim, decimal, "string");
        catch
            delete(fr);
            c.visible = "on";
            return;
        end
    end

    try
        x = csvTextScan(txt, delim, decimal, conversion);
    catch
        delete(fr);
        c.visible = "on";
        return;
    end

    if conversion == "double" then
        //must convert double to string to display data
        x = string(x);
    end

    //frame data
    limit = min(10, size(x, "c"));
    show = get("uispreadsheet_showcol", "value");
    if show then
        limit = size(x, "c");
    end

    for i = 1:limit
        if %uispreadsheet_cancel then
            break;
        end

        progress = i / limit;
        constraints = createConstraints("gridbag", [i 1 1 1], [1, 0], "horizontal", "upper", [5, 5]);

        if hasHeader then
            h = header(i);
        else
            h = sprintf("col%d", i);
        end

        uicontrol(c, ...
            "style", "text", ...
            "string", h, ...
            "horizontalalignment", "center", ...
            "relief", "solid", ...
            "constraints", constraints)

        for j = 1:size(x, "r")
            constraints = createConstraints("gridbag", [i j+1 1 1], [1, 0], "horizontal", "upper", [7 7]);

            uicontrol(c, ...
                "style", "text", ...
                "horizontalalignment", "center", ...
                "string", x(j,i), ...
                "constraints", constraints);
        end

        uispreadsheet_progress(progress);
    end

    //to push lines to top
    constraints = createConstraints("gridbag", [1 size(x, "r")+1 size(x, "c") 1], [1, 1], "both", "upper");
    uicontrol(c, ...
        "style", "frame", ...
        "constraints", constraints);

    //delete cancel frame
    delete(fr);

    if %uispreadsheet_cancel then
        //delete created children to keep interface clean
        delete(c.children)
    else
        set("uispreadsheet_btnvarialble", "enable", "on");
        set("uispreadsheet_btnfunction", "enable", "on");
    end

    clearglobal %uispreadsheet_cancel;
    c.visible = "on";
    uispreadsheet_progress(0);
endfunction
