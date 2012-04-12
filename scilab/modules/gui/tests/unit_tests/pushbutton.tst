// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unitary tests for Scilab Java pushbuttons

// Exec tests common to all uicontrols
clear uicontrol_generic_test
exec("SCI/modules/gui/tests/unit_tests/uicontrol_generic_test.sci");
if uicontrol_generic_test("pushbutton") <> 0 then pause; end

h = uicontrol("parent", scf(), "style", "pushbutton");

// Default backgroundcolor value
assert_checkequal(get(h, "backgroundcolor"), [0.6 0.6 0.6]); // Default value

// Default relief value
assert_checkequal(get(h, "relief"), "raised");

// --- Value tests ---
assert_checkequal(get(h, "value"), []); // Default value

set(h, "value", [1]);
assert_checkequal(get(h, "value"), 1);

set(h, "value", [1 2 3]);
assert_checkequal(get(h, "value"), [1 2 3]);

set(h, "value", []);
assert_checkequal(get(h, "value"), []);

// TODO test with wrong values

// Test uicontrol without style property to create PushButtons
h = uicontrol();
set(h, "string", "Black button");
set(h, "backgroundcolor", [0 0 0]);
set(h, "foregroundcolor", [1 0 0]);
set(h, "position", [50 100 200 30]);
set(h, "fontweight", "normal");

h = uicontrol(gcf());
set(h, "string", "White button");
set(h, "backgroundcolor", [1 1 1]);
set(h, "foregroundcolor", [0 1 0]);
set(h, "position", [50 130 200 30]);
set(h, "fontweight", "light");

h = uicontrol("parent", gcf());
set(h, "string", "Light grey button");
set(h, "backgroundcolor", [0.7 0.7 0.7]);
set(h, "foregroundcolor", [0 0 1]);
set(h, "position", [50 160 200 30]);
set(h, "fontweight", "demi");
