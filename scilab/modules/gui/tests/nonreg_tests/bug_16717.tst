// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHICS -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16717 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16717
//
// <-- Short Description -->
// Error in findobj when no object exists

assert_checktrue(isempty(findobj()))
assert_checktrue(isempty(findobj("type","Figure")))

