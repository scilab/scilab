// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING,
// which you should have received as part of this distribution.
// The terms are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run with exec("SCI/modules/xcos/examples/solvers/RootfindingSimple.sce");

// Import the "with-ZC" diagram and augment the ending time
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/examples/solvers/with_ZeroCrossingSimple.xcos");
scs_m.props.tf = 30;

// Set tolerances and select LSodar
scs_m.props.tol(1) = 1.0e-7;
scs_m.props.tol(2) = 1.0e-7;
scs_m.props.tol(6) = 0;

// Start the timer, launch the simulation and display time
tic();
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end
t = toc();
disp(t, "Time with rootfinding :");

// Import the "without-ZC" diagram and augment the ending time
importXcosDiagram("SCI/modules/xcos/examples/solvers/without_ZeroCrossingSimple.xcos");
scs_m.props.tf = 30;

// Set tolerances and select LSodar
scs_m.props.tol(1) = 1.0e-7;
scs_m.props.tol(2) = 1.0e-7;
scs_m.props.tol(6) = 0;

// Start the timer, launch the simulation and display time
tic();
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end
t = toc();
disp(t, "Time without rootfinding :");
