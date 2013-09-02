// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Import the diagram, modify parameters, augment the ending time and store its compilation in Info()
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/demos/ModelicaBlocks/Hydraulics.zcos");
// Redefining messagebox() to avoid popup
function messagebox(msg, title)
    disp(title);
    disp(msg);
endfunction

Info = scicos_simulate(scs_m, "nw");
tf = 50; // Final time
tolerances = scs_m.props.tol;
[%tcur, %cpr, alreadyran, needstart, needcompile, %state0] = Info(:);

solverName = ["IDA" "DDaskr - Newton" "DDaskr - GMRes"];

disp("--------------------------------");
for solver = 0:2

    disp("Time for " + solverName(solver + 1) + ":");
    tolerances(6) = solver+100;

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
