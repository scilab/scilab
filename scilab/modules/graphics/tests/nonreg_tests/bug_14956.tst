// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14956 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14956
//
// <-- Short Description -->
// clf('reset') forgets resetting immediate_drawing, resize, resizefcn,
//     closerequestfcn, toolbar_visible, menubar_visible, infobar_visible,
//     default_axes, icon.

cf = figure("resizefcn","a=1","resize","off", ..
     "toolbar_visible","off","menubar_visible","off","infobar_visible","off",..
     "closerequestfcn", "b=2", ..
     "icon", SCI+"\modules\gui\images\icons\16x16\mimetypes\package-x-generic.png");
plot2d()
cf.default_axes = "off";
cf.immediate_drawing = "off";

clf('reset')
assert_checkequal(cf.resize, "on");
assert_checkequal(cf.resizefcn, "");
assert_checkequal(cf.toolbar_visible, "on");
assert_checkequal(cf.menubar_visible, "on");
assert_checkequal(cf.infobar_visible, "on");
//assert_checkequal(cf.layout, "none");
//assert_checkequal(cf.layout_options, "OptNoLayout");
assert_checkequal(cf.default_axes, "on");
assert_checkequal(cf.immediate_drawing, "on");
assert_checkequal(cf.icon, "");
