// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

copyfile(fullfile(SCI, "modules/dynamic_link/tests/unit_tests", "call_tests.c"), fullfile(TMPDIR, "call_tests.c"));
old = pwd();
cd(TMPDIR);

intef = ["call_0_0" "call_i1_0" "call_d1_0" "call_0_i1" "call_0_d1" "call_i1_i1" "call_d1_d1" "call_i1_i1_bis" "call_d1_d1_bis" "call_i1_d1"];

ilib_for_link(intef, "call_tests.c", [], "c");
exec loader.sce;

//no args
call("call_0_0");

//1 out int
v = call("call_i1_0", "out", [1 1], 1, "i");
assert_checkequal(v, 42);

//1 out double
v = call("call_d1_0", "out", [1 1], 1, "d");
assert_checkequal(v, %pi);

//1 in int
call("call_0_i1", 42, 1, "i");

//1 in double
call("call_0_d1", %pi, 1, "d");

//1 in int, 1 out int
v = call("call_i1_i1", 21, 1, "i", "out", [1 1], 1, "i");
assert_checkequal(v, 42);
v = call("call_i1_i1", 21, 1, "i", "out", 1);
assert_checkequal(v, 42);

//1 in double, 1 out double
v = call("call_d1_d1", %pi, 1, "d", "out", [1 1], 1, "d");
assert_checkequal(v, 2*%pi);
v = call("call_d1_d1", %pi, 1, "d", "out", 1);
assert_checkequal(v, 2*%pi);

//1 in int, 1 out int
v = call("call_i1_i1_bis", 21, 1, "i", "out", [1 1], 2, "i");
assert_checkequal(v, 42);

//1 in double, 1 out double
v = call("call_d1_d1_bis", %pi, 1, "d", "out", [1 1], 2, "d");
assert_checkequal(v, 2*%pi);

//1 in int, 1 out double
v = call("call_i1_d1", 21, 1, "i", "out", [1 1], 2, "d");
assert_checkequal(v, 21*%pi);

ulink();
cd(old);
