// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13834 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13834
//
// <-- Short Description -->
// NPE when too many strings are drawn

// Just check that there is no NPE printed in the terminal

f=scf();
f.children(1).data_bounds = [0 0; 500 500];
x=linspace(1,400,1000);
y=linspace(1,400,1000);
xstring(x, y, string(x));