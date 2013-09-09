// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [m,n,nx]=%lss_size(x,flag)
    // only to be called by size function for dynamical systems
    //!

    [lhs,rhs]=argn(0)

    [a,b,c,d]=x(2:5);[m,w]=size([c,d]),[w,n]=size([b;d]);
    if rhs==2 then
        if type(flag)==10 then
            if part(flag,1)=="r" then
                flag=1;
            elseif part(flag,1)=="c" then
                flag=2;
            end
        end
    end
    if lhs==1
        if rhs==1 then
            m=[m,n]
        elseif flag==1
            m=m
        elseif flag==2
            m=n
        elseif flag=="*" then
            m=m*n
        end
    elseif lhs==2 then
        return
    elseif lhs==3 then
        [nx,nx]=size(a);
    end;
endfunction
