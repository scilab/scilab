// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function M=%infer_i_s(varargin)

    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    N=varargin(rhs-1)//inserted matrix
    index=varargin(1) //

    if rhs==3&(type(index)==10|type(index)==15) then
        if type(index)<>15 then
            M=struct()
            M(index)=N
            if index=="entries" then
                // change struct to cell
                f=getfield(1,M);f(1)="ce"
                M=setfield(1,f,M)
            end
        else
            M=createstruct(index,N)
            if type(index(1))<>10 & index(2)=="entries" then
                // change struct to cell
                f=getfield(1,M);f(1)="ce"
                M=setfield(1,f,M)
            end
        end
        return
    end
endfunction
