// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab-Enterprises - Cedric DELAMARRE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11223 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11223
//
// <-- Short Description -->
// Deletion of datatips.
// Only the first datatip can be deleted.


x=linspace(0,1,100)';
y=x.^3;
k=x.^3+0.5;
clf();

plot(x,y);
e=gce();
p=e.children(1);
t1=datatipCreate(p,80);
t2=datatipCreate(p,40);

datatipRemove(t2)
datatipRemove(t1)

assert_checkfalse(is_handle_valid(t1));
assert_checkfalse(is_handle_valid(t2));

assert_checktrue(p.datatips == []);

