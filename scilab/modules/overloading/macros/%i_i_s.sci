// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
            setfield(1,f,M)
        end
    elseif rhs>4 then//more than 2 indices: insertion of an integer in a matrix of numbers
        M=varargin($)
        if M<>[] then
            varargin($-1)=double(varargin($-1)),
            def=0;
        else
            def=iconvert(0,inttype(varargin($-1)))
        end
        M=mlist(["hm","dims","entries"],int32(size(M)),M(:))
        varargin($)=M;
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
