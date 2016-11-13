//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// figure without extra bars
f = figure("menubar_visible", "off", "toolbar_visible", "off", "infobar_visible", "off", "dockable", "off", "default_axes", "off", "visible", "off", "layout", "border");

// a tab is a frame container
tab = uicontrol(f, "style", "tab");

// each frame below is a tab entry and as a distinct layout and specific options

// a 2x3 grid layout
layout_opt = createLayoutOptions("grid", [2,3]);
frame3 = uicontrol(tab, "style", "frame", "string", "Grid layout", "layout", "grid", "layout_options", layout_opt);

for i=1:prod(layout_opt.grid)
    uicontrol(frame3, "style", "pushbutton", "string", "grid "+string(i));
end

// a gridbag layout with custom grid relative width and height
frame2 = uicontrol(tab, "style", "frame", "string", "Gridbag layout", "layout", "gridbag");

c = createConstraints("gridbag", [1, 1, 1, 3], [1,1], "both");
uicontrol(frame2, "style", "pushbutton", "string", "Expandable", "constraints", c);
c = createConstraints("gridbag", [1, 4, 1, 1], [2,1]);
uicontrol(frame2, "style", "pushbutton", "string", "Much more stable on resize", "constraints", c);
c = createConstraints("gridbag", [1, 5, 1, 1], [0.1,1], "horizontal");
uicontrol(frame2, "style", "pushbutton", "string", "Horizontal fill in", "constraints", c);


// a border layout
frame1 = uicontrol(tab, "style", "frame", "string", "Border layout", "layout", "border");

for n=["top" "left" "center" "right" "bottom"];
    c = createConstraints("border", n);
    uicontrol(frame1, "style", "pushbutton", "string", n, "constraints", c);
end

// display
f.visible = "on";
