// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1995-2010 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function d=%r_diag(a,k)
    // %r_diag - implement diag function for  rational matrix ,..
    if argn(2)<2 then
        k=0,
    else
        if type(k)<>1 then error(53,2);end
        if size(k,"*")<>1 then error(89,2);end
        if ~isreal(k) then error(52,2);end
    end
    [m,n]=size(a.num)
    if m<>1&n<>1 then
        d=rlist(diag(a.num,k),diag(a.den,k),a.dt)
    else
        mn=max(m,n)
        den=ones(mn,mn)
        den=den-diag(diag(den,k))+diag(a.den,k)
        d=rlist(diag(a.num,k),den,a.dt)
    end
endfunction
