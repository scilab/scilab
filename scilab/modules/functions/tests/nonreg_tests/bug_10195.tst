// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10195 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10195
//
// <-- Short Description -->
// execstr() interpreted ascii(0:31) as the power ^ operator

A = [1:9 11 12 14:25 27:31];  // ascii([0 10 13 26]) = EOL: OK
for a = A
    assert_checkequal(execstr("b=2"+ascii(a)+"3", "errcatch"),999);
end
