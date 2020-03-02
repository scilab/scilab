// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14191 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14191
//
// <-- Short Description -->
// plot(logflag,..) new option

clf
plot([1 10],[1,10]);
assert_checkequal(gca().log_flags, "nnn");

clf
plot("nn",[1 10],[1,10]);
assert_checkequal(gca().log_flags, "nnn");

clf
plot("ln",[1 10],[1,10]);
assert_checkequal(gca().log_flags, "lnn");

clf
plot("ll",[1 10],[1,10]);
assert_checkequal(gca().log_flags, "lln");

clf
plot("nl",[1 10],[1,10]);
assert_checkequal(gca().log_flags, "nln");
