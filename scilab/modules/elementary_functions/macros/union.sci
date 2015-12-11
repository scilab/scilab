// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [x,ka,kb] = union(a, b, orient)
    // returns the union of  unique components of  vector a and b
    // author Serge Steer INRIA

    rhs = argn(2);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "union", 2));
    end

    if rhs < 3 then
        if argn(1)==1 then
            x=unique([a(:);b(:)])
            x=x';
        else
            kab=[1:size(a,"*"), -(1:size(b,"*"))]
            [x,k]=unique([a(:);b(:)])
            x=x'
            kab=kab(k)
            ka=kab(kab>0)
            kb=-kab(kab<0)
        end
    else
        if  orient==1|orient=="r" then
            if argn(1)==1 then
                x=unique([a;b],"r")
            else
                kab=[1:size(a,"r"), -(1:size(b,"r"))]
                [x,k]=unique([a;b],"r")
                kab=kab(k)
                ka=kab(kab>0)
                kb=-kab(kab<0)
            end
        elseif orient==2|orient=="c" then
            if argn(1)==1 then
                x=unique([a b],"c")
            else
                kab=[1:size(a,"c"), -(1:size(b,"c"))]
                [x,k]=unique([a b],"c")
                kab=kab(k)
                ka=kab(kab>0)
                kb=-kab(kab<0)
            end

        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %d,''%s'',%d or ''%s'' expected\n"),"unique",3,1,"r",2,"c"));
        end
    end
endfunction
