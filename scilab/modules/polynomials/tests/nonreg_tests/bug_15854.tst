// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15854 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15854
//
// <-- Short Description -->
// horner(1:$, 4) yielded an error. Extractions from cblock were impacted.

assert_checkequal(horner(1:$,4), 1:1:4);
assert_checkequal(horner(1:-1:$,4), 1:-1:4);
assert_checkequal(horner($:-1:$-1,4), 4:-1:3);

T = msscanf(-1,["Alain 19";"Pierre 15";"Tom 12"],'%s %d');
T = [T T]
ref = msscanf(-1,["15 Pierre 15";"12 Tom 12"],'%d %s %d');
assert_checkequal(T(2:3,2:$), ref);
ref = msscanf(-1,["Tom 12 Tom 12"],'%s %d %s %d');
assert_checkequal(T(3,:), ref);
