//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12221 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12221
//
// <-- Short Description -->
// fspec is not documented and there was an error message "undefined variable: Sl".

a=-1000;
b=5;
c=5;
d=5;
sl=syslin('c',a,b,c,d);
res=fspec(sl);
ref=[-1000 5 2.2305056 2.236068 0];
for i=1:5
assert_checkalmostequal(ref(i),res(i+1));
end
