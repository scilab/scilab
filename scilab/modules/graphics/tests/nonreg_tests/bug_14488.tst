// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14488 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14488
//
// <-- Short Description -->
// plot2d(..,frameflag=9) was no longer accepted
// =============================================================================


assert_checkequal(execstr("plot2d(1:10,frameflag=9)","errcatch"), 0);
assert_checkequal(execstr("plot2d(1:10,strf=""091"")","errcatch"), 0);
