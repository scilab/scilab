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

function f2=%r_i_s(varargin)
    //author Serge Steer INRIA
    f2=varargin($-1)
    n= varargin($)
    // %r_i_s(i,j,f2,n)

    if size(varargin)==3 then
        i= varargin(1)
        if (type(i)==10|type(i)==15) then //cell or struct case
            f2=createstruct(i,f2)
            if type(i(1))<>10 & i(2)=="entries" then
                // change struct to cell
                f=getfield(1,f2);f(1)="ce"
                f2=setfield(1,f,f2)
            end
            return
        end
        d=ones(n);
        n(i)=f2.num,d(i)=f2.den
    else
        d=ones(n);
        n(varargin(1:$-2))=f2.num;
        d=ones(n);
        d(varargin(1:$-2))=f2.den;
    end
    f2=rlist(n,d,f2.dt)
endfunction
