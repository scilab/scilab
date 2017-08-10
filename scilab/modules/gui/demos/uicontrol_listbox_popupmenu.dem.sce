// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

function f = demo_list_combo()
    c = get(0);c.usedeprecatedskin = "off";
    f = figure("dockable", "off" ,"infobar_visible", "off", "toolbar_visible", "off", "menubar_visible", "off", "default_axes", "off", "visible", "off", "layout", "gridbag");
    set(f, "figure_id", 100001);
    set(f, "figure_name", gettext("Listboxes and Popupmenus"));
    set(f, "axes_size", [850 500]);

    colors4 = [
    "#000000", "Color 01", "#000000", "#FFFFFF";
    "#000080", "Color 02", "#FFFFFF", "#000000";
    "#0000FF", "Color 03", "#000000", "#FFFFFF";
    "#008000", "Color 04", "#FFFFFF", "#000000";
    "#008080", "Color 05", "#000000", "#FFFFFF";
    "#0080FF", "Color 06", "#FFFFFF", "#000000";
    "#00FF00", "Color 07", "#000000", "#FFFFFF";
    "#00FF80", "Color 08", "#FFFFFF", "#000000";
    "#00FFFF", "Color 09", "#000000", "#FFFFFF";
    "#800000", "Color 10", "#FFFFFF", "#000000";
    "#800080", "Color 11", "#000000", "#FFFFFF";
    "#8000FF", "Color 12", "#FFFFFF", "#000000";
    "#808000", "Color 13", "#000000", "#FFFFFF";
    "#808080", "Color 14", "#FFFFFF", "#000000";
    "#8080FF", "Color 15", "#000000", "#FFFFFF";
    "#80FF00", "Color 16", "#FFFFFF", "#000000";
    "#80FF80", "Color 17", "#000000", "#FFFFFF";
    "#80FFFF", "Color 18", "#FFFFFF", "#000000";
    "#FF0000", "Color 19", "#000000", "#FFFFFF";
    "#FF0080", "Color 20", "#FFFFFF", "#000000";
    "#FF00FF", "Color 21", "#000000", "#FFFFFF";
    "#FF8000", "Color 22", "#FFFFFF", "#000000";
    "#FF8080", "Color 23", "#000000", "#FFFFFF";
    "#FF80FF", "Color 24", "#FFFFFF", "#000000";
    "#FFFF00", "Color 25", "#000000", "#FFFFFF";
    "#FFFF80", "Color 26", "#FFFFFF", "#000000";
    "#FFFFFF", "Color 27", "#000000", "#FFFFFF"];

    c = createConstraints("gridbag", [1, 1, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors4);

    c = createConstraints("gridbag", [1, 2, 1, 1], [0 0], "both");
    uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors4, "value", 1);

    colors4(1:4:$,1) = "go-up";
    colors4(2:4:$,1) = "go-previous";
    colors4(3:4:$,1) = "go-down";
    colors4(4:4:$,1) = "go-next";

    c = createConstraints("gridbag", [1, 3, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors4);

    c = createConstraints("gridbag", [1, 4, 1, 1], [0 0], "both");
    uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors4, "value", 1);

    colors3_1 = [
    "#000000", "Color 01", "#D0D0D0";
    "#000080", "Color 02", "#FFFFFF";
    "#0000FF", "Color 03", "#D0D0D0";
    "#008000", "Color 04", "#FFFFFF";
    "#008080", "Color 05", "#D0D0D0";
    "#0080FF", "Color 06", "#FFFFFF";
    "#00FF00", "Color 07", "#D0D0D0";
    "#00FF80", "Color 08", "#FFFFFF";
    "#00FFFF", "Color 09", "#D0D0D0";
    "#800000", "Color 10", "#FFFFFF";
    "#800080", "Color 11", "#D0D0D0";
    "#8000FF", "Color 12", "#FFFFFF";
    "#808000", "Color 13", "#D0D0D0";
    "#808080", "Color 14", "#FFFFFF";
    "#8080FF", "Color 15", "#D0D0D0";
    "#80FF00", "Color 16", "#FFFFFF";
    "#80FF80", "Color 17", "#D0D0D0";
    "#80FFFF", "Color 18", "#FFFFFF";
    "#FF0000", "Color 19", "#D0D0D0";
    "#FF0080", "Color 20", "#FFFFFF";
    "#FF00FF", "Color 21", "#D0D0D0";
    "#FF8000", "Color 22", "#FFFFFF";
    "#FF8080", "Color 23", "#D0D0D0";
    "#FF80FF", "Color 24", "#FFFFFF";
    "#FFFF00", "Color 25", "#D0D0D0";
    "#FFFF80", "Color 26", "#FFFFFF";
    "#FFFFFF", "Color 27", "#D0D0D0"];

    c = createConstraints("gridbag", [2, 1, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors3_1);

    c = createConstraints("gridbag", [2, 2, 1, 1], [0 0], "both");
    cb3_1 = uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors3_1, "value", 1);

    colors3_2 = [
    "Color 01", "#000000", "#FFFFFF";
    "Color 02", "#FFFFFF", "#000000";
    "Color 03", "#000000", "#FFFFFF";
    "Color 04", "#FFFFFF", "#000000";
    "Color 05", "#000000", "#FFFFFF";
    "Color 06", "#FFFFFF", "#000000";
    "Color 07", "#000000", "#FFFFFF";
    "Color 08", "#FFFFFF", "#000000";
    "Color 09", "#000000", "#FFFFFF";
    "Color 10", "#FFFFFF", "#000000";
    "Color 11", "#000000", "#FFFFFF";
    "Color 12", "#FFFFFF", "#000000";
    "Color 13", "#000000", "#FFFFFF";
    "Color 14", "#FFFFFF", "#000000";
    "Color 15", "#000000", "#FFFFFF";
    "Color 16", "#FFFFFF", "#000000";
    "Color 17", "#000000", "#FFFFFF";
    "Color 18", "#FFFFFF", "#000000";
    "Color 19", "#000000", "#FFFFFF";
    "Color 20", "#FFFFFF", "#000000";
    "Color 21", "#000000", "#FFFFFF";
    "Color 22", "#FFFFFF", "#000000";
    "Color 23", "#000000", "#FFFFFF";
    "Color 24", "#FFFFFF", "#000000";
    "Color 25", "#000000", "#FFFFFF";
    "Color 26", "#FFFFFF", "#000000";
    "Color 27", "#000000", "#FFFFFF"];

    c = createConstraints("gridbag", [2, 3, 1, 1], [1 1], "both");
    lst3_2 = uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors3_2);

    c = createConstraints("gridbag", [2, 4, 1, 1], [0 0], "both");
    cb3_2 = uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors3_2, "value", 1);

    colors2_1 = [
    "#000000", "Color 01";
    "#000080", "Color 02";
    "#0000FF", "Color 03";
    "#008000", "Color 04";
    "#008080", "Color 05";
    "#0080FF", "Color 06";
    "#00FF00", "Color 07";
    "#00FF80", "Color 08";
    "#00FFFF", "Color 09";
    "#800000", "Color 10";
    "#800080", "Color 11";
    "#8000FF", "Color 12";
    "#808000", "Color 13";
    "#808080", "Color 14";
    "#8080FF", "Color 15";
    "#80FF00", "Color 16";
    "#80FF80", "Color 17";
    "#80FFFF", "Color 18";
    "#FF0000", "Color 19";
    "#FF0080", "Color 20";
    "#FF00FF", "Color 21";
    "#FF8000", "Color 22";
    "#FF8080", "Color 23";
    "#FF80FF", "Color 24";
    "#FFFF00", "Color 25";
    "#FFFF80", "Color 26";
    "#FFFFFF", "Color 27"];

    c = createConstraints("gridbag", [3, 1, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors2_1);

    c = createConstraints("gridbag", [3, 2, 1, 1], [0 0], "both");
    uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors2_1, "value", 1);

    colors2_2 = [
    "Color 01", "#D0D0D0";
    "Color 02", "#FFFFFF";
    "Color 03", "#D0D0D0";
    "Color 04", "#FFFFFF";
    "Color 05", "#D0D0D0";
    "Color 06", "#FFFFFF";
    "Color 07", "#D0D0D0";
    "Color 08", "#FFFFFF";
    "Color 09", "#D0D0D0";
    "Color 10", "#FFFFFF";
    "Color 11", "#D0D0D0";
    "Color 12", "#FFFFFF";
    "Color 13", "#D0D0D0";
    "Color 14", "#FFFFFF";
    "Color 15", "#D0D0D0";
    "Color 16", "#FFFFFF";
    "Color 17", "#D0D0D0";
    "Color 18", "#FFFFFF";
    "Color 19", "#D0D0D0";
    "Color 20", "#FFFFFF";
    "Color 21", "#D0D0D0";
    "Color 22", "#FFFFFF";
    "Color 23", "#D0D0D0";
    "Color 24", "#FFFFFF";
    "Color 25", "#D0D0D0";
    "Color 26", "#FFFFFF";
    "Color 27", "#D0D0D0"];

    c = createConstraints("gridbag", [3, 3, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors2_2);

    c = createConstraints("gridbag", [3, 4, 1, 1], [0 0], "both");
    uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors2_2, "value", 1);

    c = createConstraints("gridbag", [4, 1, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors4(:,1:2));

    c = createConstraints("gridbag", [4, 2, 1, 1], [0 0], "both");
    uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors4(:,1:2), "value", 1);

    c = createConstraints("gridbag", [4, 3, 1, 1], [1 1], "both");
    uicontrol(f, "style", "listbox", "constraints", c, "margins", [5 5 5 5], "string", colors4(:,2));

    c = createConstraints("gridbag", [4, 4, 1, 1], [0 0], "both");
    uicontrol(f, "style", "popupmenu", "constraints", c, "margins", [5 5 5 5], "string", colors4(:,2), "value", 1);

    set(f, "visible", "on");
endfunction

f = demo_list_combo();
clear demo_list_combo;
