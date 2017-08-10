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

function M=%msp_i_s(varargin)
    // %msp_i_s(i,j,b,a) insert matlab sparse matrix b into full matrix a
    // M(i,j)=b
    //!

    [lhs,rhs]=argn(0)

    M=varargin(rhs)
    N=varargin(rhs-1)//inserted matrix
    index=varargin(1) //

    if rhs==3&(type(index)==10|type(index)==15) then
        M=createstruct(index,N)
        if type(index(1))<>10 & index(2)=="entries" then
            // change struct to cell
            f=getfield(1,M);f(1)="ce"
            M=setfield(1,f,M)
        end
    elseif rhs>4 then //more than 2 indices:

        //insertion of a sparse matrix in a matrix of numbers
        error(msprintf(_("%s: Insertion of a sparse matrix in a matrix of numbers is not defined.\n"),"%msp_i_s"));
    else
        i=varargin(1);
        j=varargin(2);
        M=varargin(4);
        b=varargin(3);
        if rhs==4 then
            M(i,j)=full(b)
        else
            M=b;//b=j
            M(i)=full(j)
        end
    end
endfunction
