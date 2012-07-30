// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11069 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11069
//
// <-- Short Description -->
// An axes could be wrongly set as parent for an uicontrol.

refMsg = msprintf(_("Wrong value for ''%s'' property: A ''%s'' or ''%s'' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
assert_checkerror("uicontrol(""parent"", gca())", refMsg);
