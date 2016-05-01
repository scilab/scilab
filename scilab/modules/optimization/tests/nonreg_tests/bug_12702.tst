//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 12702 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12702
//
// <-- Short Description -->
// When no extra parameters are needed in the cost function, NDcost does not work:
//

function f = rosenbrock(x)
    f = 100.0 * (x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

function [f, g, ind] = rosenbrockCostFixedStep(x, ind)
    // Test based on old 'derivative' function
    if ((ind == 1) | (ind == 4)) then
        f = rosenbrock ( x );
    end
    if ((ind == 1) | (ind == 4)) then
        g = numderivative ( rosenbrock , x(:) , %eps^(1/3));
    end
endfunction

function [f, g, ind] = rosenbrockCostVariableStep(x, ind)
    if ((ind == 1) | (ind == 4)) then
        f = rosenbrock ( x );
    end
    if ((ind == 1) | (ind == 4)) then
        g = numderivative ( rosenbrock , x(:));
    end
endfunction

x0 = [-1.2 1.0];
lines(0);

for impval = 1:5
    [ fopt , xopt ] = optim ( rosenbrockCostFixedStep , x0 , "gc" , imp=impval);
end

for impval = 1:5
    [ fopt , xopt ] = optim ( rosenbrockCostVariableStep , x0 , "gc" , imp=impval);
end
