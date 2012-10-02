// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING,
// which you should have received as part of this distribution.
// The terms are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run with exec("SCI/modules/xcos/examples/solvers/integLSodar.sce");

// Import the diagram and augment the ending time
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/examples/solvers/ODE_Example.xcos");
scs_m.props.tf = 30000;

solverName = ["LSodar", "BDF/Newton", "BDF/Functional", "Adams/Newton", "Adams/Functional"];

for solver = 0:4

 // Select the solver
 scs_m.props.tol(6) = solver;

 // Start the timer, launch the simulation and display time
 tic();
 try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end;
 t = toc();
 disp(t, "Time for " + solverName(solver+1) + " :");

end
