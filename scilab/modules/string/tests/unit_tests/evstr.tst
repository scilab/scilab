// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// =============================================================================
// unit tests evstr
// =============================================================================
a = 1;
b = 2;
Z = ["a","b"];
res = evstr(Z);
assert_checkequal(res(1), a);
assert_checkequal(res(2), b);
// =============================================================================
a = 1;
b = 2;
Z = list(["%(1)","%(1)-%(2)"], ["a+1","b+1"]);
res = evstr(Z);
assert_checkequal(res, [2 -1]);
// =============================================================================
assert_checktrue(isnan(evstr(%nan)));
assert_checktrue(isinf(evstr(%inf)));
assert_checktrue(isnan(evstr("%nan")));
assert_checktrue(isinf(evstr("%inf")));
assert_checktrue(isnan(evstr("Nan")));
assert_checktrue(isinf(evstr("Inf")));
assert_checktrue(isnan(evstr("NaN")));
assert_checktrue(isinf(evstr(-%inf)));
assert_checktrue(isinf(evstr("-%inf")));
assert_checktrue(isinf(evstr("-Inf")));
// =============================================================================
Scilab_value = 3;
assert_checkequal(Scilab_value, evstr("Scilab_value"));
// =============================================================================
clear toto;
assert_checkfalse(execstr("evstr(''toto'')", "errcatch") == 0);
assert_checkfalse(execstr("evstr(toto)", "errcatch") == 0);
// =============================================================================
s = rand(3,3,3);
assert_checkequal(execstr("evstr(s)", "errcatch"), 0);
// =============================================================================
function foo(), endfunction
assert_checkequal(execstr("evstr(''foo'')", "errcatch"), 0);
// =============================================================================
assert_checkequal(evstr(""), []);
// =============================================================================
