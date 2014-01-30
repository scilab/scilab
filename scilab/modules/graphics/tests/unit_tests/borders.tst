// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->


l = list();
//borders
//none
//l($+1) = createBorders("none");
//empty
l($+1) = createBorders("empty");
l($+1) = createBorders("empty", 10, 20, 30, 40);
//line
l($+1) = createBorders("line", "blue");
l($+1) = createBorders("line", "red", 2);
//bevel
l($+1) = createBorders("bevel", "raised");
l($+1) = createBorders("bevel", "lowered", "blue", "red");
l($+1) = createBorders("bevel", "lowered", "blue", "red", "magenta", "orange");
// softbevel
l($+1) = createBorders("softbevel", "raised");
l($+1) = createBorders("softbevel", "lowered", "blue", "red");
l($+1) = createBorders("softbevel", "lowered", "blue", "red", "magenta", "orange");
//etched
l($+1) = createBorders("etched");
l($+1) = createBorders("etched", "raised");
l($+1) = createBorders("etched", "blue", "red");
l($+1) = createBorders("etched", "lowered", "blue", "red");
//titled
font1 = createFontBorder("Serif", 12, "bold");
font2 = createFontBorder("Serif", 12, "normal", "italic");
l($+1) = createBorders("titled", "title");
l($+1) = createBorders("titled", createBorders("empty"));
l($+1) = createBorders("titled", createBorders("line", "blue"), "title");
l($+1) = createBorders("titled", createBorders("bevel", "raised"), "title", "center", "top");
l($+1) = createBorders("titled", createBorders("matte", 1, 2, 3, 4, "pink"), "title", "center", "top", font1);
l($+1) = createBorders("titled", createBorders("etched"), "title", "left", "top", font2, "blue");
//compound
l($+1) = createBorders("compound");
l($+1) = createBorders("compound", createBorders("empty"), createBorders("etched"));
//matte
l($+1) = createBorders("matte", 4, 3, 2, 1, "yellow");

//create a window with all type of frame border.

f = figure( ...
    "dockable", "off", ...
    "menubar_visible", "off", ...
    "infobar_visible", "off", ...
    "toolbar_visible", "off", ...
    "layout", "grid", ...
    "layout_options", createLayoutOptions("grid", [4,0], [1 1]));
for i = 1:size(l)
    u = uicontrol(f, "style", "frame", "borders", l(i));
    assert_checkequal(u.borders, l(i));
end
