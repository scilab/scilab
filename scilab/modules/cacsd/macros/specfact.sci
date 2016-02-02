// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [w0,l]=specfact(a,b,c,d)

    r=d+d';w0=sqrtm(r);
    p=ricc(a-b/r*c,-b/r*b',c'/r*c,"cont");
    //a'*p+p*a+(c'-p*b)*inv(r)*(c-b'*p) is zero
    l=w0\(c-b'*p)
endfunction
