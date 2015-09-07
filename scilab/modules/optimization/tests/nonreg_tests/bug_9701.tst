//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9701 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9701
//
// <-- Short Description -->
// optim gateway was failing for large problems.
//

function [f, g, ind] = optimCost(x, ind, n)
    xref = (1:n)'
    if ind == 1 | ind == 4 then
        f = 0.5*sum((x-xref).^2)
    else
        f = 0
    end
    if ind == 1 | ind == 4 then
        g = x-xref
    else
        g = zeros(x)
    end
endfunction

n = 50000;
x0 = ones(n, 1);

try optim(list(optimCost, n), x0, "qn", "ar",1000,1000, imp=2); catch msg = lasterror(); end
refMsg = msprintf(_("Can not allocate %.2f MB memory.\n"), (n * ((n+13) / 2) * 8) / 1e6);
assert_checkequal(msg(1), refMsg);
