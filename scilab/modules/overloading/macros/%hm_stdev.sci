// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function x = %hm_stdev(m, d, ms)

    if argn(2) < 3 then
        ms = %f
    end
    if argn(2) == 3 & type(ms)~=1 & typeof(ms) ~= "hypermat" then
        error(msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"stdev",3));
    end
    if argn(2) == 1 | d == "*" then
        if argn(2) == 3 then
            x = stdev(m(:), "*", ms);
        else
            x = stdev(m(:), "*");
        end
        return
    elseif d == "r" then
        d = 1;
    elseif d == "c" then
        d = 2;
    end
    dims = size(m);
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
            x = stdev(matrix(m(I),dims(d),-1), 1, ms);
        else
            x = stdev(matrix(m(I),dims(d),-1), 1, ms(:)');
        end
    else
        x = stdev(matrix(m(I),dims(d),-1), 1);
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
