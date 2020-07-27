// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for getdate function
// =============================================================================

D = getdate();
// ----------
assert_checkequal(D, int(D));
assert_checktrue(D(1) >= 2020);
assert_checktrue(D(2) > 0 & D(2) < 13);
assert_checktrue(D(3) > 0 & D(3) < 54);
assert_checktrue(D(4) > 0 & D(4) < 367);
assert_checktrue(D(5) > 0 & D(5) < 8);
assert_checktrue(D(6) > 0 & D(6) < 32);
assert_checktrue(D(7) >= 0 & D(7) < 25);
assert_checktrue(D(8) >= 0 & D(8) < 60);
assert_checktrue(D(9) >= 0 & D(9) < 60);
assert_checktrue(D(10) >= 0 & D(10) < 1000);

// D = getdate(X)
// --------------
D = getdate([1e9, 2e9, 3e9] + 0.21);
assert_checkequal(size(D), [3, 10]);
assert_checkalmostequal(D(:,10), [210 210 210]', 1e-3);

assert_checkequal(D(:,1:9), int(D(:,1:9)));
assert_checktrue(and(D(:,1) >= 2000));
assert_checktrue(and(D(:,2) > 0 & D(:,2) < 13));
assert_checktrue(and(D(:,3) > 0 & D(:,3) < 54));
assert_checktrue(and(D(:,4) > 0 & D(:,4) < 367));
assert_checktrue(and(D(:,5) > 0 & D(:,5) < 8));
assert_checktrue(and(D(:,6) > 0 & D(:,6) < 32));
assert_checktrue(and(D(:,7) >= 0 & D(:,7) < 25));
assert_checktrue(and(D(:,8) >= 0 & D(:,8) < 60));
assert_checktrue(and(D(:,9) >= 0 & D(:,9) < 60));

// --------------
u = getdate("s");
f = (u - int(u))*1000;
D = getdate(u);
assert_checkequal(D(10), f);

// -------------- // Only for time zone = +1h00
dt = getdate(0);
assert_checkequal(dt, [1970 1 1 1 5 1 1 0 0 0]);

dt = getdate(1);
assert_checkequal(dt, [1970 1 1 1 5 1 1 0 1 0]);

dt = getdate(90542256);
assert_checkequal(dt, [1972 11 46 318 2 13 23 37 36 0]);

t1_ref = getdate("s");
t3_ref = getdate();
t2_ref = datenum();

t1 = t1_ref - t3_ref(10)/1000;
t2 = (t2_ref - datenum(1970,1,1,1,0,0)) *3600*24;
t3 = (datenum(t3_ref(1),t3_ref(2),t3_ref(6),t3_ref(7),t3_ref(8),t3_ref(9)) - datenum(1970,1,1,1,0,0)) * 3600 * 24;

assert_checkfalse((abs(t2-t1) > 1) & (abs(t2-t1)-3600 > 1));
assert_checkfalse((abs(t3-t1) > 1) & (abs(t3-t1)-3600 > 1));
assert_checkfalse((abs(t3-t2) > 1) & (abs(t3-t2)-3600 > 1));
