// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [a, ka] = setdiff(a, b)
    // returns a values which are not in b

    rhs = argn(2);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "setdiff", 2));
    end

    if b == [] then //b is empty reurn a!
        ka = 1:size(a,"*");
        return
    end

    [a,ka] = unique(a);
    na = size(a,"*");

    b = unique(b(:));

    if type(a) == 10 then
        [x,k] = gsort([a(:); b], "g", "i");
    else
        [x,k] = gsort([a(:); b], "g", "d");
    end
    if type(a) == 1 then x=x($:-1:1);k=k($:-1:1);,end
    d = find(x(2:$)==x(1:$-1));  //index of common entries in sorted table
    if d <> [] then
        k([d;d+1]) = [];
    end

    keep = find(k <= na);
    a = a(k(keep));
    ka = ka(k(keep));
endfunction
