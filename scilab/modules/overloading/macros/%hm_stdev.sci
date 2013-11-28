// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x = %hm_stdev(m, d, ms)

    if argn(2) < 3 then
        ms = %f
    end
    if argn(2) == 3 & type(ms)~=1 & typeof(ms) ~= "hypermat" then
        error(msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"stdev",3));
    end
    if argn(2) == 1 | d == "*" then
        if argn(2) == 3 then
            x = stdev(m.entries, "*", ms);
        else
            x = stdev(m.entries, "*");
        end
        return
    elseif d == "r" then
        d = 1;
    elseif d == "c" then
        d = 2;
    end
    dims = double(m.dims);
    if d > size(m,d) then
        x = zeros(m);
        return
    end
    N = size(dims, "*");
    p1 = prod(dims(1:d-1));// step to build one vector on which stdev is applied
    p2 = p1*dims(d);//step for beginning of next vectors
    ind = (0:p1:p2-1)';// selection for building one vector
    deb = (1:p1);
    I = ind*ones(deb)+ones(ind)*deb;

    ind = (0:p2:prod(dims)-1);
    I = ones(ind).*.I+ind.*.ones(I);

    if argn(2) == 3 then
        if isscalar(ms) then
            x = stdev(matrix(m.entries(I),dims(d),-1), 1, ms);
        else
            x = stdev(matrix(m.entries(I),dims(d),-1), 1, ms.entries');
        end
    else
        x = stdev(matrix(m.entries(I),dims(d),-1), 1);
    end
    dims(d) = 1;
    if d == N then
        dims = dims(1:$)
    else
        dims(d) = 1
    end
    if size(dims, "*") == 2 then
        x = matrix(x, dims(1), dims(2))
    else
        x = hypermat(dims, x)
    end

endfunction
