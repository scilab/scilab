// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11096 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11096
//
// <-- Short Description -->
// uimenu fails with a weird error message when adding a uimenu 
// to the console in NW mode.

refMsg = msprintf(_("%s: can not add a menu into the console in this mode.\n"), "SetMenuParent");
assert_checkerror("uimenu(""parent"", 0, ""label"", ""Test"")", refMsg);
