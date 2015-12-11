// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 9688 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9688
//
// <-- Short Description -->
// optim could crash when "imp" option was < 0
//
function [ f , g , ind ] = woodFG ( x , ind )
    f = []
    g = []
    if ind == 2 | ind == 3 | ind == 4 then
        A = x(2)-x(1)^2
        B = x(4)-x(3)^2
    end
    if ind == 2 | ind == 4 then
        f = 100*A^2+(1-x(1))^2+90*B^2+(1-x(3))^2+...
        10.1*((x(2)-1)^2+(x(4)-1)^2)+19.8*(x(2)-1)*(x(4)-1)
    end
    if ind == 3 | ind == 4 then
        g(1) = -2*(200*x(1)*A+1-x(1))
        g(2) = 2*(100*A+10.1*(x(2)-1)+9.9*(x(4)-1))
        g(3) = -2*(180*x(3)*B+1.-x(3))
        g(4) = 2*(90*B+10.1*(x(4)-1)+9.9*(x(2)-1))
    end
endfunction
x0 = [-3 -1 -3 -1];

refMsg = msprintf(_("%s: Wrong type for output argument #%d: Real scalar expected.\n"), "woodFG", 1);
assert_checkerror("[ fopt, xopt, gopt ] = optim ( woodFG, x0, ""qn"", imp=-1);", refMsg);
assert_checkerror("[ fopt, xopt, gopt ] = optim ( woodFG, x0, ""gc"", imp=-1);", refMsg);
assert_checkerror("[ fopt, xopt, gopt ] = optim ( woodFG, x0, ""nd"", imp=-1);", refMsg);
