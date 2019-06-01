// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

assert_checkequal(lstcat(list(1,2),list(4)), list(1,2,4));
assert_checkequal(lstcat(list(1,2,4),list()), list(1,2,4));
assert_checkequal(lstcat(list(1),list(2,4)), list(1,2,4));
assert_checkequal(lstcat(list(),list(1,2,4)), list(1,2,4));

assert_checkequal(lstcat(list(1,2), 4), list(1,2,4));
assert_checkequal(lstcat(1, list(2,4)), list(1,2,4));

assert_checkequal(lstcat(list(1), list(2), list(4)), list(1,2,4));
assert_checkequal(lstcat(list(1,2),list(),4), list(1,2,4));

L = lstcat(list([1 2],['my string';'matrix']), list('foo',%s));
assert_checkequal(L, list([1 2],['my string';'matrix'], 'foo', %s));

sp = sprand(100,100,0.01);
m = rand(100,100);
L1 = list(%z, m , sp, sp<>0, "abc", list(%f,,"defg"));
L2 = list(sin, 1:$, , list(cosd, , %pi));
     // include gdf() after fixing http://bugzilla.scilab.org/16103
Lres = list(%z, m , sp, sp<>0, "abc", list(%f,,"defg"), ..
            2:$, sind, sin, 1:$, , list(cosd, , %pi), cos);
assert_checkequal(lstcat(L1, 2:$, sind, L2, cos), Lres);
