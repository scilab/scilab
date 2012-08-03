// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Test if scf set also the current axes.

plot;
f=gcf();
sca(f.children(2));
a=gca();
assert_checkequal(f.children(2), a);

scf;
plot2d;
f=gcf();
assert_checkequal(f.figure_id, 1);

scf(0);
f=gcf();
a=gca();
assert_checkequal(a.parent.figure_id, f.figure_id);
assert_checkequal(f.children(2), a);
