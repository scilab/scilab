// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Import the diagram and augment the ending time
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/examples/solvers/ODE_Example.zcos");
scs_m.props.tf = 30000;

solverName=["BDF/Newton", "BDF/Functional", "Adams/Newton", "Adams/Functional", "implicit Runge-Kutta"];

for solver=1:5

    // Select the solver (implicit Runge-Kutta is solver number 7)
    scs_m.props.tol(6) = solver;
    if (solver == 5) then scs_m.props.tol(6) = 7; end

    // Set max step size and reltol if implicit Runge-Kutta
    if (solver == 5) then scs_m.props.tol(7) = 0.01; scs_m.props.tol(2) = 1d-10; end

    // Start the timer, launch the simulation and display time
    tic();
    try scicos_simulate(scs_m, "nw"); catch disp(lasterror()); end
    t = toc();
    disp(t, "Time for " + solverName(solver) + ":");

end
