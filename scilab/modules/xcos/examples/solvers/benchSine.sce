// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Import the diagram, augment the ending time and store its compilation in Info()
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/examples/solvers/ODE_Example.zcos");
Info = scicos_simulate(scs_m, "nw");
tf = 50000;
tolerances = scs_m.props.tol;
[%tcur, %cpr, alreadyran, needstart, needcompile, %state0] = Info(:);

solverName = ["LSodar", "CVode BDF/Newton", "CVode BDF/Functional", "CVode Adams/Newton", "CVode Adams/Functional", "Dormand-Prince", "Runge-Kutta", "implicit Runge-Kutta", "Crank-Nicolson"];

disp("--------------------------------");
for solver = 0:8

    disp("Time for " + solverName(solver + 1) + ":");
    tolerances(6) = solver;

    // Modify 'Max step size' if RK-based solver
    if (solver >= 5) then tolerances(7) = 0.01; end

    // Start the solver
    [state, t] = scicosim(%state0, 0.0, tf, %cpr.sim, "start", tolerances);

    // Run until the end
    tic();
    [state, t] = scicosim(state, 0.0, tf, %cpr.sim, "run", tolerances);
    t = toc();
    disp(t);

    // End the solver
    [state, t] = scicosim(state, tf, tf, %cpr.sim, "finish", tolerances);

end
disp("--------------------------------");
