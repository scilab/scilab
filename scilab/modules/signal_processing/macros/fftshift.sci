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

function x = fftshift(x,job)
    if argn(2)<2 then job="all",end
    deff("sel=fun(sk)","c=ceil(sk/2);sel=[c+1:sk,1:c]")
    if job=="r" then job=1,elseif job=="c" then job=2,end
    ind=list()
    if job=="all" then
        for sk=size(x),ind($+1)=fun(sk),end
    else
        for sk=size(x),ind($+1)=:,end;
        ind(job)=fun(size(x,job))
    end
    x=x(ind(:))
endfunction
