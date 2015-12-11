// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 12725 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12725
//
// <-- Short Description -->
// Datatips did not work in log_scale

scf();
x = (-5:0.1:5) + 6;
plot2d("ll", x, sin(x) + 2, 9);
e = gce();
e = e.children(1);
datatipCreate(e, 10);

// Move the datatip and create new ones to see if all is ok

clf();
t = 0:0.1:5*%pi;
param3d(sin(t) + 2, cos(t) + 2, t / 10 + 1);
a = gca();
a.log_flags="lll";
e = gce();
datatipCreate(e, 10);

// Move the datatip and create new ones to see if all is ok