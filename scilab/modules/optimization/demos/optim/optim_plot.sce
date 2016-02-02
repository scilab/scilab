// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
// Copyright (C) 2010      - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function demo_optim_plot()

    mprintf(_("Running optimization ...\n"));

    //
    // 1. Define rosenbrock for contouring
    function f = rosenbrockC ( x1 , x2 )
        x = [x1 x2];
        f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction

    x0 = [-1.2 1.0];
    xopt = [1.0 1.0];

    //
    // 2. Draw the contour of Rosenbrock's function
    xdata = linspace(-2,2,100);
    ydata = linspace(-2,2,100);
    mprintf("Draw contours...\n");
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("optim_plot.sce");
    contour ( xdata , ydata , rosenbrockC , [1 10 100 500 1000])
    plot(x0(1) , x0(2) , "b.")
    plot(xopt(1) , xopt(2) , "r*")

    //
    // 3. Define Rosenbrock for optimization
    function [ f , g , ind ] = rosenbrock ( x , ind )
        if ((ind == 1) | (ind == 4)) then
            f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
        end

        if ((ind == 1) | (ind == 4)) then
            g(1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
            g(2) = 200. * ( x(2) - x(1)**2 )
        end

        if (ind == 1) then
            plot ( x(1) , x(2) , "g." )
        end
    endfunction

    //
    // 4. Plot the optimization process, during optimization
    mprintf("Plot points during optimization...\n");
    [ fopt , xopt ] = optim ( rosenbrock , x0 , imp = -1)

endfunction

demo_optim_plot();
clear demo_optim_plot;

