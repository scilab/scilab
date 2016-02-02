// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [pg,U]=hrmt(v)
    // Syntax: [pg,U]=hrmt(v)
    // Finds unimodular U and pg = gcd of a row of polynomials v
    // such that v*U = [pg,0]
    //!
    [n,m]=size(v)
    if n>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A row vector expected.\n"),"hrmt",1))
    end
    pg=v(1)
    U=eye(m,m)
    for k=2:m
        [pg,uk]=bezout(pg,v(k))
        U(:,k-1:k)=U(:,k-1:k)*uk(:,[2 1])
    end
endfunction
