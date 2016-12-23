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
function M=%b_i_s(varargin)


    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    //if M<>[] then M=M<>0,end
    N=varargin(rhs-1)//inserted matrix
    index=varargin(1) //
    if rhs==3&(type(index)==10|type(index)==15) then
        M=createstruct(index,N)
        if type(index(1))<>10 & index(2)=="entries" then
            // change struct to cell
            f=getfield(1,M);f(1)="ce"
            M=setfield(1,f,M)
        end
    elseif rhs>4 then //more than 2 indices: insertion of a boolean in a matrix of numbers
        M=varargin($)
        if M<>[] then varargin($-1)=bool2s(varargin($-1)),end
        M=generic_i_hm(%f,varargin(:))
    else //type conversion
        M=varargin($)
        M(varargin(1:$-2))=bool2s(varargin($-1))
    end
endfunction
