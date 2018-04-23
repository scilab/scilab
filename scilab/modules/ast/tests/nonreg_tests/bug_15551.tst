// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15551 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15551
//
// <-- Short Description -->
// arithmetic operators give wrong results with booleans

assert_checkequal(%f+%f,0);
assert_checkequal(%f+%t,1);
assert_checkequal(%t+%t,2);
assert_checkequal([%f %f]+[%f %f],[0 0]);
assert_checkequal([%t %f]+[%f %t],[1 1]);
assert_checkequal([%t %t]+[%t %t],[2 2]);
assert_checkequal(%f-%f,0);
assert_checkequal(%f-%t,-1);
assert_checkequal(%t-%t,0);
assert_checkequal([%f %f]-[%f %f],[0 0]);
assert_checkequal([%t %f]-[%f %t],[1 -1]);
assert_checkequal([%t %t]-[%t %t],[0 0]);
