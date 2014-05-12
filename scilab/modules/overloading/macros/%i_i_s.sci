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

function M=%i_i_s(varargin)
    //insertion of an integer matrix in an matrix of double


    rhs=argn(2)
    index=varargin(1)

    if rhs==3&(type(index)==10|type(index)==15) then //struct or cell creation
        N=varargin(rhs-1)//inserted matrix
        M=createstruct(index,N)
        if type(index(1))<>10 & index(2)=="entries" then
            // change struct to cell
            f=getfield(1,M);f(1)="ce"
            M=setfield(1,f,M)
        end
    elseif rhs>4 then//more than 2 indices: insertion of an integer in a matrix of numbers
        M=varargin($)
        if M<>[] then
            varargin($-1)=double(varargin($-1)),
            def=0;
        else
            def=iconvert(0,inttype(varargin($-1)))
        end
        M=generic_i_hm(def,varargin(:))
    else //type conversion
        M=varargin($)
        if M==[] then
            N=varargin($-1)
            M=iconvert(0,inttype(N))
            M(varargin(1:$-2))=varargin($-1)
        else
            M(varargin(1:$-2))=double(varargin($-1))
        end
    end
endfunction
