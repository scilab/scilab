// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9789 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9789
//
// <-- Short Description -->
// optim prints its termination status even when "imp" is not given,
// so the user is informed of the final state of the optimization.
//

function [ f , g , ind ] = woodFG ( x , ind )
    if ind == 2 | ind == 3 | ind == 4 then
        A = x(2)-x(1)^2
        B = x(4)-x(3)^2
    end
    if ind == 2 | ind == 4 then
        f = 100*A^2+(1-x(1))^2+90*B^2+(1-x(3))^2+...
        10.1*((x(2)-1)^2+(x(4)-1)^2)+19.8*(x(2)-1)*(x(4)-1)
    end
    if ind == 3 | ind == 4 then
        g(1) = -2*(200*x(1)*A+1.-x(1))
        g(2) = 2*(100*A+10.1*(x(2)-1)+9.9*(x(4)-1))
        g(3) = -2*(180*x(3)*B+1.-x(3))
        g(4) = 2*(90*B+10.1*(x(4)-1)+9.9*(x(2)-1))
    end
endfunction
x0 = [-3 -1 -3 -1];

[ fopt , xopt ] = optim ( woodFG , x0 );
