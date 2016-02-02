// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function m = mtlb_std(x, flag, dim)

    [lhs, rhs] = argn();

    if rhs < 2 then
        flag = 0;
    elseif rhs < 3 then
        dim = "*";
    end

    m = stdev(x, dim);
    n = size(x, dim);

    if flag == 1 then
        m = m*sqrt(1-1/n);
    end

endfunction
