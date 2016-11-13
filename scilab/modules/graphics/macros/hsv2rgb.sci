// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [r,g,b]=hsv2rgb(h,s,v)

    if and(argn(2)<>[1 3]) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "hsv2rgb", 1, 3));
    end

    if argn(2)==1 then
        v=h(:,3);s=h(:,2);h=h(:,1)
    else
        h = h(:);s = s(:);v = v(:)
    end

    h=6*h
    k=fix(h-6*%eps)
    f=h-k

    t=1-s
    n=1-s.*f
    p=1-s.*(1-f)
    e=ones(h)

    K=bool2s(k*ones(1,6)==ones(k)*(0:5))
    r=sum(K.*[e n t t p e],2)
    g=sum(K.*[p e e n t t],2)
    b=sum(K.*[t t p e e n],2)
    f=v ./ max([r(:);g(:);b(:)])

    if argn(1)==1 then
        r=[f.*r,f.*g,f.*b]
    else
        r=f.*r;g=f.*g;b=f.*b
    end
endfunction
