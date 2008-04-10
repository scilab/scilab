// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2796 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2796
//
// <-- Short Description -->
// Rescaling a subplot alos modify other subplots scales.

T=1:100;

// create 4 subplots and save their data bounds,
// margins and axes_bounds
subplot(3,3,1);
plot2d(T,cos(T/10));
axes1 = gca();
dataBounds1 = axes1.data_bounds;
margins1 = axes1.margins;
axesBounds1 = axes1.axes_bounds;

subplot(3,3,2);
plot2d(T,sin(T/10));
axes2 = gca();
dataBounds2 = axes2.data_bounds;
margins2 = axes2.margins;
axesBounds2 = axes2.axes_bounds;

subplot(3,3,3);
plot2d(T,T);
axes3 = gca();
dataBounds3 = axes3.data_bounds;
margins3 = axes3.margins;
axesBounds3 = axes3.axes_bounds;

subplot(3,3,4);
plot2d(T,T/2);
axes4 = gca();
dataBounds4 = axes4.data_bounds;
margins4 = axes4.margins;
axesBounds4 = axes4.axes_bounds;

// modify 4th plot
subplot(3,3,4);
plot2d(T,T);

// check that data bounds did not change for other plots
if (axes1.data_bounds <> dataBounds1) then pause; end;
if (axes2.data_bounds <> dataBounds2) then pause; end;
if (axes3.data_bounds <> dataBounds3) then pause; end;

// margins should not have change for any of plots
if (axes1.margins <> margins1) then pause; end;
if (axes2.margins <> margins2) then pause; end;
if (axes3.margins <> margins3) then pause; end;
if (axes4.margins <> margins4) then pause; end;

// axes_bounds also
if (axes1.axes_bounds <> axesBounds1) then pause; end;
if (axes2.axes_bounds <> axesBounds2) then pause; end;
if (axes3.axes_bounds <> axesBounds3) then pause; end;
if (axes4.axes_bounds <> axesBounds4) then pause; end;


