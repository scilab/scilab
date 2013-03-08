// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function [s] = variance(x, orien, w)
    //
    //This function computes  the  variance  of  the values of  a  vector or
    //matrix x.
    //
    //For a vector or   a matrix x,  s=variance(x)  returns in  the scalar s  the
    //variance of all the entries of x.
    //
    //s=variance(x,'r')    (or,  equivalently,    s=variance(x,1)) is     the  rowwise
    //variance. It returns in each entry of the row vector s the variance of
    //each column of x.
    //
    //s=variance(x,'c') (or, equivalently, s=variance(x,2)) is the columnwise standard
    //deviation.   It  returns in  each  entry  of the   column vector y the
    //variance of each row of x.
    //
    //

    // Checking and normalizing input arguments:
    // ----------------------------------------
    [lhs,rhs] = argn(0)
    if rhs==0 then
        tmp = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(tmp, "variance", 1, 2))
    end

    if x==[] then
        s = %nan
        return
    end

    if ~isdef("orien","local") then
        orien = "*"
    end

    transposed = %f // to refer and process as in "r", we priorly transpose any "c" request
    if orien=="r" | orien==1 | orien=="c" | orien==2 | orien=="*"
        if orien=="c" | orien==2 then
            x = x.'
            transposed = %t
            orien = "r"
        end
    else
        tmp = gettext("%s: Wrong value for input argument: ''%s'', ''%s'', %d or %d expected.\n")
        error(msprintf(tmp, "variance", "c", "r", 1, 2))
    end

    if rhs<3 then
        w = 0
    elseif typeof(w)~="constant" | and(w~=[0 1]) then
        tmp = gettext("%s: Wrong value of w : %s; %d or %d expected.\n")
        error(msprintf(tmp, "variance", string(w), 0, 1))
    end

    // Calculations
    // ------------
    d = size(x, orien) - 1 + w    // denominator
    if orien=="*" then
        m = mean(x)
    else
        m = mean(x, orien).*.ones(size(x,1),1)
    end
    s = sum(abs(x - m).^2, orien) / d

    if transposed
        s = s.'
    end

endfunction

