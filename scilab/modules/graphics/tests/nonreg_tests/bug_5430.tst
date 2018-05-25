// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5430 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/5430
//
// <-- Short Description -->
// clf() could erase the ATOMS GUI or the m2sci ones

// ============ ATOMS GUI ===========
xdel(winsid());
atomsGui();
clf
// CHECK:
// * the ATOMS GUI must not be erased
// * a new empty figure must have been created

xdel(winsid());
plot()
atomsGui();
clf
// CHECK:
// * despite it is the latest one, the ATOMS GUI must not be erased
// * the plot() figure must have been cleared

xdel(winsid());
plot()
scf(); plot2d()
atomsGui();
clf
// CHECK:
// * despite it is the latest one, the ATOMS GUI must not be erased
// * the plot2d() figure #1 must have been cleared (not the plot() #0 one)

// ============ m2sci GUI ===========

xdel(winsid());
m2sci_gui();
clf
// CHECK:
// * the m2sci GUI must not be erased
// * a new empty figure must have been created

xdel(winsid());
plot()
m2sci_gui();
clf
// CHECK:
// * despite it is the latest one, the m2sci GUI must not be erased
// * the plot() figure must have been cleared

xdel(winsid());
plot()
scf(); plot2d()
m2sci_gui();
clf
// CHECK:
// * despite it is the latest one, the m2sci GUI must not be erased
// * the plot2d() figure #1 must have been cleared (not the plot() #0 one)

