
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//


function [y] = mean(x,orient)

    //
    // NOTES
    //    - modified by bruno to handle the case when x is an hypermatrix
    //    - restricted to matrix or hypermatrix of "constants" (that is
    //      real or complex numbers)
    //    - mean([]) return nan but mean([],orient) return [] (this
    //      is strange but corresponds to matlab behavior)

    [lhs,rhs] = argn()
    // some arguments checking
    if rhs == 0 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"mean",1,2)),
    else
        if type(x) ~= 1  &  typeof(x) ~= "hypermat" then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Real vector or matrix expected.\n"), "mean",1))
        end
        if type(x) ~= 1 then  // that is x is an hypermat !
            if type(x.entries) ~= 1 then
                error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"mean",1))
            end
        end
        if rhs == 2 then
            if orient == "r" then, orient = 1, end
            if orient == "c" then, orient = 2, end
            if orient == "m" then
                flag=find(size(x)>1,1)
                if flag==[] then
                    y=mean(x)
                    return
                else
                    y=mean(x,flag)
                    return
                end
            end
            if floor(orient)~=orient | orient < 1 | orient > length(size(x)) then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar or vector expected.\n"),"mean",2)),
            end
        end
    end

    if rhs==1 then
        if x==[] then y=%nan;return,end
        y=sum(x)/size(x,"*")
    else
        if x==[] then y=[];return,end
        y=sum(x,orient)/size(x,orient)
    end
endfunction
