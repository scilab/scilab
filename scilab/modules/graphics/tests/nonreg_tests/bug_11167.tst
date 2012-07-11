// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11167 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11167
//
// <-- Short Description -->
// datatipRemoveAll example does not work

x=linspace(0,1,100)';
y=x.^3;
clf();
plot(x,x.^3,'b',x,x.^2,'r');
e=gce();e=e.children;
datatipCreate(e(1),50);
datatipCreate(e(2),20);
datatipRemoveAll(e);

plot(x,x.^3,'b',x,x.^2,'r');
e=gce();e=e.children;
datatipCreate(e(1),50);
datatipCreate(e(2),20);

assert_checkequal(execstr("datatipRemoveAll()", "errcatch"), 0);



