// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function usedeprecatedskin()
    c = get(0);
    oldSkin = c.usedeprecatedskin;
    c.usedeprecatedskin = "off";
    f = figure("dockable", "off" ,"infobar_visible", "off", "toolbar_visible", "off", "menubar_visible", "off", "position", [200 200 240 430], "figure_name", "");
    fr1 = uicontrol("style", "frame", "position", [10 10 220 410], "border", createBorder("etched"));
    label = uicontrol(fr1, "style", "text", "string", "usedeprecatedskin=""off""", "position", [10 370 200 30]);
    editbox = uicontrol(fr1, "style", "edit", "string", "Edit", "position", [10 330 200 30]);
    button = uicontrol(fr1, "style", "pushbutton", "string", "Pushbutton", "position", [10 290 200 30]);
    checkbox = uicontrol(fr1, "style", "checkbox", "string", "Checkbox", "position", [10 250 200 30]);
    radio = uicontrol(fr1, "style", "radiobutton", "string", "Radiobutton", "position", [10 210 200 30]);
    slider = uicontrol(fr1, "style", "slider", "position", [10 130 200 50]);
    frame = uicontrol(fr1, "style", "frame", "position", [10 110 200 30]);
    listbox = uicontrol(fr1, "style", "listbox", "string", "listbox1|listbox2|listbox3|listbox4|listbox5", "position", [10 50 200 50]);
    combo = uicontrol(fr1, "style", "popupmenu", "string", "popupmenu1|popupmenu2", "position", [10 10 200 30]);
    c.usedeprecatedskin = oldSkin;

    c = get(0);
    oldSkin = c.usedeprecatedskin;
    c.usedeprecatedskin = "on";
    f = figure("dockable", "off" ,"infobar_visible", "off", "toolbar_visible", "off", "menubar_visible", "off", "position", [480 200 240 430], "figure_name", "");
    fr1 = uicontrol("style", "frame", "position", [10 10 220 410], "border", createBorder("etched"));
    label = uicontrol(fr1, "style", "text", "string", "usedeprecatedskin=""on""", "position", [10 370 200 30]);
    editbox = uicontrol(fr1, "style", "edit", "string", "Edit", "position", [10 330 200 30]);
    button = uicontrol(fr1, "style", "pushbutton", "string", "Pushbutton", "position", [10 290 200 30]);
    checkbox = uicontrol(fr1, "style", "checkbox", "string", "Checkbox", "position", [10 250 200 30]);
    radio = uicontrol(fr1, "style", "radiobutton", "string", "Radiobutton", "position", [10 210 200 30]);
    slider = uicontrol(fr1, "style", "slider", "position", [10 150 200 30]);
    frame = uicontrol(fr1, "style", "frame", "position", [10 110 200 30]);
    listbox = uicontrol(fr1, "style", "listbox", "string", "listbox1|listbox2|listbox3|listbox4|listbox5", "position", [10 50 200 50]);
    combo = uicontrol(fr1, "style", "popupmenu", "string", "popupmenu1|popupmenu2", "position", [10 10 200 30]);
    c.usedeprecatedskin = oldSkin;
endfunction

usedeprecatedskin();
clear usedeprecatedskin;
