// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [w0,l]=specfact(a,b,c,d)

    r=d+d';w0=sqrtm(r);
    p=ricc(a-b/r*c,-b/r*b',c'/r*c,"cont");
    //a'*p+p*a+(c'-p*b)*inv(r)*(c-b'*p) is zero
    l=w0\(c-b'*p)
endfunction
