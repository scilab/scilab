// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab-Enterprises - Cedric DELAMARRE
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->

plot3d();

f1 = gcf();
a1 = gca();
f2 = scf(10);

relocate_handle(a1, f2);

a2 = f2.children(1);
assert_checkequal(a2.children(1).type, "Plot3d");


