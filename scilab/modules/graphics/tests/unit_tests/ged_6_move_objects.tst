// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// Unitary tests for ged(6,figure)
//  = interactively selects and moves objects in a figure

// 1) Executes the following block of instructions to generate the test figure:
// ==================================
function move_test(ptitle)
    x = linspace(-2,2,150);
    y = exp(2*x).*(1.05+sin(5*x));
    y2 = exp(2*x).*(1.05+cos(5*x));
    y3 = exp(x).*(1.05+cos(10*x));
    plot2d(x,[y', y2' y3'])
    ax = gca();
    ax.log_flags = "nln";
    ax.axes_reverse(1) = "on";
    ax.grid_style = [8 8];
    ax.sub_ticks = [1 8];
    xgrid(color("grey80"))
    // Labels
    xtitle([ptitle; "row #2"],"Abscissae (inverted)", "Ordinates (log)")
    // Text
    xstring(0.8, 0.02, "Message");
    e = gce();
    e.font_size = 3;
    e.box = "on";
    xstring(0.5, 0.005, "Titled text");
    e = gce();
    e.font_size = 2;
    e.font_angle = -30;
    //xstring(-0.5, 3, "Centered");  // bugged
    //e = gce();
    //e.font_size = 2;
    //e.text_box_mode = "centered";

    // Segs - arrows
    x = linspace(0,2*%pi,20);
    xv = [sin(x); 9*sin(x)]/20 + 0.4;
    yv = [cos(x); 9*cos(x)]*2 + 30;
    xsegs(xv,yv,color("cyan"))
    e = gce();
    e.arrow_size = 1;
    // Rectangle
    xrect(-0.6, 20, 0.3, 14)
    e = gce();
    e.thickness = 2;
    e.foreground = color("red");
    // Datatip
    d = datatipCreate(ax.children($).children(1),[-1.14 0.6]);
    d.detached_position = [-1.2 1 0];
    // Arc
    xarc(0.9, 3.181, 0.6, 3.18, 120*64, 250*64)
    e = gce();
    e.foreground = color("orange");
    e.thickness = 2;
    // Legends from legends() or legend()
    legend(['cos(t)';'cos(2*t)';'cos(3*t)']); // "Legend" type
    legends(['sin(t)';'sin(2*t)';'sin(3*t)'],[-1,2 3],opt="ll") // "Axes" type
    replot tight
    // colorbar
    c = jetcolormap(50);
    gcf().color_map = [gcf().color_map ; c];
    colorbar(-5,10,[35 85]);
    //gce().parent.axes_bounds(3) = 0.06;
endfunction
clf reset
subplot(1,2,1)
move_test("Subplot")
subplot(2,2,2)
plot2d()
xtitle("plot2d() example","Abscissae X","Ordinates Y")

// Uicontrols
// ----------
f = gcf();
    // listbox
h = uicontrol(f,'style','listbox','units','normalized','position', [0.55 0.1 0.1 0.15]);
h.string = "item 1|item 2|item3";   // fill the list
h.value = 2;
    // slider
h = uicontrol(f,'style','slider', 'string', "Slider", ..
    'units','normalized','position', [0.68 0.1 0.03 0.15], ..
    'min', -3, 'max', 10, 'value', 1, 'sliderstep', [0.1 1]);
h = uicontrol(f,'style','slider', ..
    'units','normalized','position', [0.55 0.05 0.17 0.030], ..
    'min', -3, 'max', 10, 'value', 1, 'sliderstep', [0.1 1]);
    // popup menu
h = uicontrol(f,'style','popupmenu', 'position', [550 175 100 20], ..
    'string', ["Option 1" "Option 2" "$x^{-3}$"], 'value', 2);
    // Checkbox
h = uicontrol(f,'style','checkbox', ..
    'position', [400 175 130 20], ..
    'string', "Absolute position", 'value', 1);
    // Radio buttons
h = uicontrol(f,'style','radiobutton', 'position', [400 150 130 20], ..
    "min", 0, "max", 3, "value", 3, ..
    'string', "Resize window & see", "groupname", "test");
h = uicontrol(f,'style','radiobutton', 'position', [400 125 130 20], ..
    'string', "Alternative", "groupname", "test");

// Second separated figure
// -----------------------
scf();
subplot(2,1,2)
plot2d()
xtitle("plot2d() example","Abscissae X","Ordinates Y")
// ==================================

// 2) Click on the "Move objects" icon in the graphic toolbar
//   or in the graphic "Edit => Move objects" menu.
//
// 3) Check that the proper message is displayed in the console:
//    "WARNING : Mouse moving picker ON => Console LOCKED
//    On a figure: Click left to get and set. Click middle to QUIT..."
//
// 4) Test moving each type of object:
//    - the whole axes: (handle = its box)
//    - both blocks of legends (generated with legend() or with legends())
//    - the boxed "Message" plotted with xstring() (inside the box)
//    - the "Tilted text" plotted with xstring() (inside the box)
//    - one of the curves, displayed with plot2d()
//    - the detached datatip linked to the green curve (inside the box)
//    - the set of cyan segments with arrows (one of its segments)
//    - the red rectangle (border)
//    - the orange arc (arc)
//    - the colorbar (small border)
//    - each uicontrol (border): listbox, popupmenu, sliders.
//      Choose a border point to drag the control ; avoid to push it.
//    - Titles of the plot2d() example in linear scaled
//
// 5) Zoom in the semi-log axes, and make the same tests as here-above
//
// 6) Click the middle button, and check that the proper message is displayed
//    in the console: "// Moving picker OFF => Back to the console."

