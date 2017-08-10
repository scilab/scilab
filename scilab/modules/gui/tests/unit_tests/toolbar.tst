// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Unitary tests for toolbar function
f=gcf();

// Check errors: wrong size for input
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A real expected.\n"), "toolbar", 1);
assert_checkerror("toolbar([1 2])", refMsg, 999);
assert_checkerror("toolbar([1 2], ""on"")", refMsg, 999);

// Check errors: wrong type for input
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real or a Figure handle expected.\n"), "toolbar", 1);
assert_checkerror("toolbar(%f)", refMsg, 999);
assert_checkerror("toolbar(%f, ""on"")", refMsg, 999);

//  Check errors: wrong value for input
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be > %d expected.\n"), "toolbar", 1, -1);
assert_checkerror("toolbar(-12)", refMsg, 999);
assert_checkerror("toolbar(-12, ""on"")", refMsg, 999);

// Check errors: the figure does not exist
refMsg = msprintf(_("%s: Wrong value for input argument #%d: ''Graphic Window Number %d'' does not exist.\n"), "toolbar", 1, 12);
assert_checkerror("toolbar(12)", refMsg, 999);
assert_checkerror("toolbar(12, ""on"")", refMsg, 999);

// Check errors: wrong handle type for input
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real or a Figure handle expected.\n"), "toolbar", 1);
assert_checkerror("toolbar(gca())", refMsg, 999);
assert_checkerror("toolbar(gca(), ""on"")", refMsg, 999);

// Check errors: wrong size for handle as input
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), "toolbar", 1);
assert_checkerror("toolbar([f f])", refMsg, 999);
assert_checkerror("toolbar([f f], ""on"")", refMsg, 999);

// Check errors: handle does not exist
g=scf();
delete(g);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: this handle does not exist.\n"), "toolbar", 1);
assert_checkerror("toolbar(g)", refMsg, 999);
assert_checkerror("toolbar(g, ""on"")", refMsg, 999);

// Check errors: wrong type for second argument
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "toolbar", 2);
assert_checkerror("toolbar(f, 2)", refMsg, 999);

// Check errors: wrong size for second argument
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "toolbar", 2);
assert_checkerror("toolbar(f, [""on"", ""off""])", refMsg, 999);

// Check errors: wrong value for second argument
refMsg = msprintf(_("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "toolbar", 2, "on", "off");
assert_checkerror("toolbar(f, ""onf"")", refMsg, 999);

// Check that the toolbar is visible
assert_checkequal(toolbar(f), "on");
assert_checkequal(toolbar(f.figure_id), "on");
assert_checkequal(f.toolbar_visible, "on");

// Change toolbar status using toolbar function
toolbar(f, "off");
assert_checkequal(toolbar(f), "off");
assert_checkequal(toolbar(f.figure_id), "off");
assert_checkequal(f.toolbar_visible, "off");

// Change toolbar status using toolbar_visible property
f.toolbar_visible = "on";
assert_checkequal(toolbar(f), "on");
assert_checkequal(toolbar(f.figure_id), "on");
assert_checkequal(f.toolbar_visible, "on");
