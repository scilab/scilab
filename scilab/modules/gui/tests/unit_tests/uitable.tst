// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Han DONG
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Unitary tests for Scilab Java uitable

// Exec tests common to all uicontrols
clear uicontrol_generic_test
exec("SCI/modules/gui/tests/unit_tests/uicontrol_generic_test.sci");
if uicontrol_generic_test("uitable") <> 0 then pause; end

// Regular testing of imagerenderer in uicontrol
h = uicontrol("style", "uitable", "position", [0 0 300 300]);
set(h, "colnames", "1|2|3|4|5");
set(h, "tabledata", "a b c d e|1.2 3.2 43.3");

// TODO test with wrong values

// Test set functions for uicontrol
h = uicontrol("style", "uitable");
set(h, "position", [0 0 300 300]);
set(h, "colnames", "1|2|3|4|5");
set(h, "tabledata", "a b c d e|1.2 3.2 43.3");

h = uicontrol(gcf(), "style", "uitable");
set(h, "position", [10 0 400 300]);
set(h, "colnames", "1|2|3|4|5");
set(h, "tabledata", "a x y d 1.2|1.2 3.2 43.3 0 0.3");
set(h, "rownames", "r1|r2");

h = uicontrol("parent", gcf(), "style", "uitable");
set(h, "position", [0 20 400 200]);
set(h, "colnames", "c1|c2|c3|c4|5");
set(h, "tabledata", "a x y d 1.2|1.2 3.2 43.3 0 0.3|x y z w c y|1 2 3 4 5 6");
