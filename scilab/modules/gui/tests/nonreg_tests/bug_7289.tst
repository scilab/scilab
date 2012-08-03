// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7289 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7289
//
// <-- Short Description -->
// delmenu(console_uimenu) badly works. The handle is still valid after deletion.

// Launch Scilab in STD mode.

h=uimenu("parent",0,"label","Test")
delmenu("Test")
assert_checkfalse(is_handle_valid(h));
