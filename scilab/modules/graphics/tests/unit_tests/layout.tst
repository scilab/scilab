// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->


function fig = createFigure(layout)
    fig = figure("dockable", "off" ,"infobar_visible", "off", "toolbar_visible", "off", "menubar_visible", "off", "default_axes", "off")
    fig.layout = layout;
endfunction

//constraints

//no border
createFigure("none");

c = createConstraints("nolayout");
u = uicontrol("constraints", c, "string", "button", "position", [10 10 100 30]);
assert_checkequal(u.constraints, c);

delete(gcf());

//border layout
createFigure("border");

c = createConstraints("border", "center");
u = uicontrol("constraints", c, "string", "center [-1 -1]");
assert_checkequal(u.constraints, c);

c = createConstraints("border", "left", [150 150]);
u = uicontrol("constraints", c, "string", "left [150 150]");
assert_checkequal(u.constraints, c);

c = createConstraints("border", "right", [250 250]);
u = uicontrol("constraints", c, "string", "right [250 250]");
assert_checkequal(u.constraints, c);

c = createConstraints("border", "top", [50 50]);
u = uicontrol("constraints", c, "string", "top [50 50]");
assert_checkequal(u.constraints, c);

c = createConstraints("border", "bottom", [100, 100]);
u = uicontrol("constraints", c, "string", "bottom [100 100]");
assert_checkequal(u.constraints, c);

delete(gcf());

//grid layout
f = createFigure("grid");
f.layout_options = createLayoutOptions("grid", [3 4], [10 10]);
uicontrol("constraints", c, "string", "11");
uicontrol("constraints", c, "string", "12");
uicontrol("constraints", c, "string", "13");
uicontrol("constraints", c, "string", "14");
uicontrol("constraints", c, "string", "21");
uicontrol("constraints", c, "string", "22");
uicontrol("constraints", c, "string", "23");
uicontrol("constraints", c, "string", "24");
uicontrol("constraints", c, "string", "31");
uicontrol("constraints", c, "string", "32");
uicontrol("constraints", c, "string", "33");
uicontrol("constraints", c, "string", "34");

delete(gcf());

//gridbag layout
f = createFigure("gridbag");

c = createConstraints("gridbag", [1 1 1 1]);
u = uicontrol("constraints", c, "string", "text1");
assert_checkequal(u.constraints, c);

c = createConstraints("gridbag", [2 1 1 1], [1.0 1.0]);
u = uicontrol("constraints", c, "string", "text2");
assert_checkequal(u.constraints, c);

c = createConstraints("gridbag", [1 2 2 1], [1.0 1.0], "vertical");
u = uicontrol("constraints", c, "string", "text3");
assert_checkequal(u.constraints, c);

c = createConstraints("gridbag", [3 1 1 2], [1.0 1.0], "both", "lower_left", [0 0]);
u = uicontrol("constraints", c, "string", "text4");
assert_checkequal(u.constraints, c);

c = createConstraints("gridbag", [1 4 3 2], [1.0 1.0], "both", "center", [0 0], [-1 -1]);
u = uicontrol("constraints", c, "string", "text5");
assert_checkequal(u.constraints, c);

c = createConstraints("gridbag", [1 6 3 2], [1.0 1.0], "both", "center", [0 0], [200 200]);
u = uicontrol("constraints", c, "string", "text6");
assert_checkequal(u.constraints, c);

delete(gcf());


// options
createFigure("none");
f = gcf();
assert_checkequal(f.layout_options, createLayoutOptions("nolayout"));
f.layout_options = createLayoutOptions("none");
assert_checkequal(f.layout_options, createLayoutOptions("none"));
delete(gcf());

createFigure("gridbag");
f = gcf();
assert_checkequal(f.layout_options, createLayoutOptions("gridbag"));
f.layout_options = createLayoutOptions("gridbag");
assert_checkequal(f.layout_options, createLayoutOptions("gridbag"));
delete(gcf());

createFigure("border");
f = gcf();
assert_checkequal(f.layout_options, createLayoutOptions("border"));

f.layout_options = createLayoutOptions("border", [10,20]);
assert_checkequal(f.layout_options, createLayoutOptions("border", [10,20]));
delete(gcf());

createFigure("grid");
f = gcf();
assert_checkequal(f.layout_options, createLayoutOptions("grid"));

f.layout_options = createLayoutOptions("grid", [5,3]);
assert_checkequal(f.layout_options, createLayoutOptions("grid", [5,3]));

f.layout_options = createLayoutOptions("grid", [5,3], [30,40]);
assert_checkequal(f.layout_options, createLayoutOptions("grid", [5,3], [30,40]));
delete(gcf());

