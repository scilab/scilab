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

function x = %hm_stdev(m, d, ms)
    // d: 0, 1, 2...
    if argn(2) < 3 then
        ms = %f
    end
    if argn(2) == 3 & type(ms)~=1 & typeof(ms) ~= "hypermat" then
        msg = _("%s: Wrong type for input argument #%d: A real matrix expected.\n")
        error(msprintf(msg, "stdev", 3));
    end
    if d > length(size(m)) then
        msg = _("%s: Wrong value for input argument #%d: integer <= ndims(argument #%d) expected.\n")
        error(msprintf(msg,"stdev",2,1));
    end

    if argn(2) == 1 | d==0 then
        if argn(2) == 3 then
            x = stdev(m(:), "*", ms);
        else
            x = stdev(m(:), "*");
        end
        return
    end
    dims = size(m);
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
    x = matrix(x, dims)
endfunction
