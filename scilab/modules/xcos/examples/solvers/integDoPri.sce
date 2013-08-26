// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Import the diagram and augment the ending time
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/examples/solvers/ODE_Example.zcos");
scs_m.props.tf = 30000;

solverName=["BDF/Newton", "BDF/Functional", "Adams/Newton", "Adams/Functional", "Dormand-Prince"];

for solver=1:5

    // Select the solver
    scs_m.props.tol(6) = solver;

    // Set max step size if DoPri
    if (solver == 5) then scs_m.props.tol(7) = 0.01; end

    // Start the timer, launch the simulation and display time
    tic();
    try scicos_simulate(scs_m, "nw"); catch disp(lasterror()); end;
    t = toc();
    disp(t, "Time for " + solverName(solver) + ":");

end
