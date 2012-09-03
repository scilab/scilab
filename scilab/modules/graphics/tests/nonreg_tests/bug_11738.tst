// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11738 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11738
//
// <-- Short Description -->
// xsave + xload : bad and very slow rendering

scf(0);
clf;
plot();
f=gcf();
nbrChild = size(f.children());
xsave("test.scg",0);
xdel(0);
xload("test.scg");

f=gcf();
assert_checkequal(size(f.children), nbrChild);
