// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4327 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4327
//
// <-- Short Description -->
// overloading did not support type names longer than 8 characters

t = tlist(['myLongTypename','x','y'],2.0,3.0);
deff("r = %myLongTypename_sin(t)","r = sin(t.x * t.y)");
deff("r = %myLongTypename_o_myLongTypename(t1,t2)","r = (t1.x == t2.y)");
assert_checktrue(execstr("sin(t)", "errcatch")==0);
assert_checktrue(execstr("t<>t", "errcatch")==0);

t = mlist(['myLongTypename','x','y'],2.0,3.0);
assert_checktrue(execstr("sin(t)", "errcatch")==0);
assert_checktrue(execstr("t==t", "errcatch")==0);
