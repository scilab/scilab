// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
