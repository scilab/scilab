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

function M=generic_i_s(varargin)

    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    N=varargin(rhs-1)//inserted matrix
    index=varargin(1) //
    if rhs==3&(type(index)==10|type(index)==15) then
        if type(index)<>15 then
            M=struct()
            M(index)=N
        else
            M=createstruct(index,N)

        end
        if index($)=="entries" then
            // change struct to cell
            f=getfield(1,M);f(1)="ce"
            M=setfield(1,f,M)
        end
        return
    elseif rhs==3 &(type(index)<>10 & type(index)<>15) then
        error(msprintf(_("%s: Wrong type for input argument #%d.\n"),"generic_i_s",1));
    else
        error(msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"generic_i_s",3));
    end
endfunction

