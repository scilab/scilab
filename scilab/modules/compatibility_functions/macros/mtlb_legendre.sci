// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=mtlb_legendre(n,x,flag)
    // Emulation function for legendre() Matlab function

    rhs=argn(2)
    if ndims(n) <> 2 | size(n,"*") <> 1 | floor(n)-n <> 0 | n<0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: A positive integer value expected.\n"),"mtlb_legendre",1));
    end

    m = 0:n;
    xtemp = matrix(x,1,-1);
    xone=find(xtemp==1);
    xminusone=find(xtemp==-1);
    xtemp(xone)=nearfloat("pred",1);
    xtemp(xminusone)=nearfloat("succ",-1);
    y = legendre(n,m,xtemp);

    if (rhs == 2) then
        y = legendre(n,m,xtemp);
    elseif rhs == 3 then
        if flag == "norm" then
            y = legendre(n,m,xtemp,"norm");
        elseif flag == "sch" then
            y = legendre(n,m,xtemp);
            if n == 0
                y = ones(y);
            else
                for j = 1:size(x,"*")
                    for i = 1:n+1
                        if modulo(i,n+1) <> 1 then
                            y((j-1)*(n+1)+i) = (-1)^m(i)*sqrt(2*prod(1:(n-m(i)))/prod(1:(n+m(i))))*y((j-1)*(n+1)+i);
                        end
                    end
                end
            end
        end
    end

    if n==0 then
        y = matrix(y,size(x));
    elseif ndims(x)>2 | (ndims(x) == 2 & size(x,1)>1 & size(x,2)>1) then
        y = matrix(y,[n+1 size(x)]);
    else
        y = matrix(y,[n+1 size(x,"*")]);
    end
endfunction
