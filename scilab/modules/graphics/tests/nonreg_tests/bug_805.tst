// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
// <-- Non-regression test for bug 805 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=805
//
// <-- Short Description -->
// creating a very large canvas used to fail

f = gcf();
f.auto_resize = "off";
plot(1:10);
f.axes_size = [10000, 10000];

// it should instead provide an error message


