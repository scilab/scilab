// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000-2010 - INRIA - Serge Steer
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function M=hypermat(dims,v)
    //initialize an hypermatrix whose dimensions are given in the vector dims
    // all entries are set to 0
    //
    // M data structure contains the vector of matrix dimensions M('dims')
    // and the vector of entries M('entries') such as the leftmost subcripts vary first
    // [M(1,1,..);..;M(n1,1,..);...;M(1,n2,..);..;M(n1,n2,..);...]
    warnobsolete("matrix", "6.1.0");
    [lhs,rhs]=argn(0)
    dims=double(dims)
    if or(floor(dims)<>dims)|or(dims<0) then
        error(msprintf(_("%s: Wrong values for input argument #%d: Elements must be non-negative integers.\n"),"hypermat",1));
    end

    if size(dims, "*") == 1 then
        dims=[1 dims];
    else
        //remove last dimensions equal to 1
        nd = size(dims, "*");
        while nd > 2 & dims(nd) == 1 then
            nd = nd - 1;
        end
        dims = dims(1:nd);
    end

    if argn(2) < 2 then
        v = zeros(prod(dims), 1);
    end

    if size(v, "*") <> double(prod(dims)) then
        error(msprintf(gettext("%s: Number of entries does not match product of dimensions"), "hypermat"));
    end

    M = matrix(v, dims);
endfunction
