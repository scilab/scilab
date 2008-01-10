// Unitary tests for Scilab Java pushbuttons
// Vincent COUVERT - INRIA 2007

// Exec tests common to all uicontrols
clear uicontrol_generic_test
getf("SCI/modules/gui/tests/unit_tests/uicontrol_generic_test.sci");
uicontrol_generic_test("pushbutton");

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
