// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4176 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4176
//
// <-- Short Description -->
// Plotting huge amount of data nothing is displayed and no errors are displayed.
//

Maximum=3e6
dt=1/Maximum;
t=0:dt:1;
y=sin(2*%pi*50*t);
plot(t,y);

// check that the plot or an error message is displayed.
