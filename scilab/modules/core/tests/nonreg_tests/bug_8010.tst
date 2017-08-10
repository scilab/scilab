// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8010 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8010
//
// <-- Short Description -->
// Permanent variables might be silently redefined, as with %i(1,1)=1

assert_checkfalse(execstr("%i=1","errcatch")==0);
assert_checkequal(%i, complex(0,1));
assert_checkfalse(execstr("%i(1,1)=1","errcatch")==0);
assert_checkequal(%i, complex(0,1));

assert_checkfalse(execstr("%e=1","errcatch")==0);
assert_checkequal(%e, 2.7182818284590451);
assert_checkfalse(execstr("%e(1,1)=1","errcatch")==0);
assert_checkequal(%e, 2.7182818284590451);

assert_checkfalse(execstr("%s=1","errcatch")==0);
assert_checkequal(%s, poly(0,"s"));
assert_checkfalse(execstr("%s(1,1)=1","errcatch")==0);
assert_checkequal(%s, poly(0,"s"));

assert_checkfalse(execstr("%t=""true""","errcatch")==0);
assert_checkequal(%t, 1==1);
assert_checkfalse(execstr("%t(1,1)=""true""","errcatch")==0);
assert_checkequal(%t, 1==1);
