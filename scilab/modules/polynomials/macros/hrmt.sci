// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
