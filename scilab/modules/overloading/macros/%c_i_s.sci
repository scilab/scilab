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

function M=%c_i_s(varargin)
    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    N=varargin(rhs-1)//inserted matrix
    index=varargin(1) //
    if size(M,"*")<>0 then
        error(msprintf(_("%s: Affection of a string in a matrix of numbers is not implemented.\n"),"%c_i_s"));
    end
    if rhs==3 then
        if type(index)==10 then  //M.x=N or M.entries=N
            M=struct()
            M(index)=N
            if index=="entries" then //M.entries=N
                // change struct to cell
                f=getfield(1,M);f(1)="ce"
                M=setfield(1,f,M)
            end
            return
        elseif type(index)==15 then
            //M(i).x=N or M(i,j,..).x=N or M.x(i,j,..)or M(i,j..)
            //check for a name in the index list
            isstr=%f; for ii=index,if type(ii)==10 then  isstr=%t,break,end,end
            if isstr then
                M=createstruct(index,N)
                if type(index(1))<>10 & index(2)=="entries" then
                    // change struct to cell
                    f=getfield(1,M);f(1)="ce"
                    M=setfield(1,f,M)
                end
            else
                M(index(:))=N
            end
            return
        end

    elseif rhs>4 then //more than 2 indices: insertion of a string in an empty matrix
        M=generic_i_hm("",varargin(:))
    else //should not occur (hard coded case)
        M=var
    end
endfunction

