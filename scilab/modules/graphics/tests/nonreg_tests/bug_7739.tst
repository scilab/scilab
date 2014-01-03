// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7739 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7739
//
// <-- Short Description -->
// Invalid axis position in log modes

clf();
x=linspace(1,7,50);
plot2d(x,100+(x-3).^3);
a=gca();
drawaxis(dir="u",tics="r",y=170,x=[1 7 6],sub_int=5);

// axis should stay at the "same" place
a.log_flags="lnn";

// axis should stay at the "same" place
a.log_flags="nln";

// axis should stay at the "same" place
a.log_flags="lln";