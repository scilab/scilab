// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Simple example

function demo_ode_simple()

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("ode_simple.dem.sce");

    // ODE definition
    function xd=odelin(t,x,A)
        xd=A*x;
    endfunction
    A=[1 1;0 2];

    // Integration and comparison with exp
    ea = ode(eye(2,2),0,1,list(odelin,A)),expm(A);

    // New integration with another a
    t  = 0:0.1:3;
    ee = ode(1,0,t,list(odelin,1));

    // Visualization
    plot2d(t',ee',style=2);
    xlabel("t");ylabel("x");
    title(["Simple ODE"
    " xdot=x"],"fontsize",3)

endfunction

demo_ode_simple();
clear demo_ode_simple;
