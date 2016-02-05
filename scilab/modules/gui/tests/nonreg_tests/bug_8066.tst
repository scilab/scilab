// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Author: Ishit Mehta (ibm)
// Copyright (C) 2015
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8066 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8066
//
// <-- Short Description -->
//  the x_choose gui should change its behavior: - we should add a "OK" button

n = x_choose(['item1';'item2';'item3'],['that is a comment';'for the dialog'])

// Click on an item and click Ok.
// Should return the 1-based index of that item