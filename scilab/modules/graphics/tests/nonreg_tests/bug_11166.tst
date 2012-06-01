// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11166 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11166
//
// <-- Short Description -->
// datatipRemove(t1) returned a error message


x=linspace(0,1,100)';
y=x.^3;
k=x.^3+0.5;
clf();

plot(x,y);
e=gce();
p=e.children(1);
t1=datatipCreate(p,80);

assert_checkequal(execstr("datatipRemove(t1)", "errcatch"), 0);

