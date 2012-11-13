// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run with exec("SCI/modules/xcos/help/en_US/solvers/integ.sce");

// Import the diagram and augment the ending time
importXcosDiagram("SCI/modules/xcos/examples/solvers/ODE_Example.xcos");
scs_m.props.tf = 3500;

// BDF / Newton
// Select the solver
scs_m.props.tol(6) = 0;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for BDF / Newton :");

// BDF / Functional
// Select the solver
scs_m.props.tol(6) = 1;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for BDF / Functional :");

// Adams / Functional
// Select the solver
scs_m.props.tol(6) = 3;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for Adams / Functional :");

// Adams / Newton
// Select the solver
scs_m.props.tol(6) = 2;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for Adams / Newton :");
