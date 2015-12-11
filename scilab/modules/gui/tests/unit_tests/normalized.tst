// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//create some button with normalized position
//resize the window and components must follow

function f = test()
    c = get(0);c.usedeprecatedskin = "off";

    function create_buttons(parent)
        uicontrol(parent, "style", "pushbutton", "units", "normalized", "position", [0 0.5 0.5 0.5], "string", "1");
        uicontrol(parent, "style", "pushbutton", "units", "normalized", "position", [0 0 0.5 0.5], "string", "2");
        uicontrol(parent, "style", "pushbutton", "units", "normalized", "position", [0.5 0.5 0.5 0.5], "string", "3");
        uicontrol(parent, "style", "pushbutton", "units", "normalized", "position", [0.5 0 0.5 0.5], "string", "4");
    endfunction

    f = figure( ...
        "dockable", "off" , ...
        "infobar_visible", "off", ...
        "toolbar_visible", "off", ...
        "menubar_visible", "off", ...
        "default_axes", "off", ...
        "figure_name", "normalized", ...
        "background", 1);

    //bottom_left : 4 buttons in figure
    uicontrol(f, "style", "pushbutton", "units", "normalized", "position", [0 0.25 0.25 0.25], "string", "1");
    uicontrol(f, "style", "pushbutton", "units", "normalized", "position", [0 0 0.25 0.25], "string", "2");
    uicontrol(f, "style", "pushbutton", "units", "normalized", "position", [0.25 0.25 0.25 0.25], "string", "3");
    uicontrol(f, "style", "pushbutton", "units", "normalized", "position", [0.25 0 0.25 0.25], "string", "4");

    //bottom_right : 4 buttons in a frame
    fr = uicontrol(f, "style", "frame", "units", "normalized", "position", [0.5 0 0.5 0.5], "backgroundcolor", [0 0 0]);
    create_buttons(fr);

    //top : 4 buttons in 4 frames in a frame
    fr = uicontrol(f, "style", "frame", "units", "normalized", "position", [0 0.5 1 0.5], "backgroundcolor", [0 0 0]);
    fr1 = uicontrol(fr, "style", "frame", "units", "normalized", "position", [0 0.5 0.5 0.5], "backgroundcolor", [0 0 0]);
    create_buttons(fr1);
    fr2 = uicontrol(fr, "style", "frame", "units", "normalized", "position", [0 0 0.5 0.5], "backgroundcolor", [0 0 0]);
    create_buttons(fr2);
    fr3 = uicontrol(fr, "style", "frame", "units", "normalized", "position", [0.5 0.5 0.5 0.5], "backgroundcolor", [0 0 0]);
    create_buttons(fr3);
    fr4 = uicontrol(fr, "style", "frame", "units", "normalized", "position", [0.5 0 0.5 0.5], "backgroundcolor", [0 0 0]);
    create_buttons(fr4);
endfunction

test();
clear test;
