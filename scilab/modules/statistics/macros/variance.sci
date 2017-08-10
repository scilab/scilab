// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// Copyright (C) 2013 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [s, m] = variance(x, orien, m)
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
    //The input argument m represents the a priori mean. If it is present, then the sum is
    //divided by n. Otherwise ("sample variance"), it is divided by n-1.
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

    if rhs==3 then
        if typeof(m)~="constant" then
            tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
            error(msprintf(tmp, "variance"))
        elseif orien=="*" then
            if ~isscalar(m) then
                tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
                error(msprintf(tmp, "variance"))
            end
        elseif orien=="r" | orien==1 then
            if size(m)~=[1 size(x,"c")] & ~isscalar(m) then
                tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
                error(msprintf(tmp, "variance"))
            end
        elseif orien=="c" | orien==2 then
            if size(m)~=[size(x,"r") 1] & ~isscalar(m) then
                tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
                error(msprintf(tmp, "variance"))
            end
        end
    end

    transposed = %f // to refer and process as in "r", we priorly transpose any "c" request
    if orien=="r" | orien==1 | orien=="c" | orien==2 | orien=="*"
        if orien=="c" | orien==2 then
            x = x.'
            transposed = %t
            orien = "r"
        end
    else
        tmp = gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n")
        error(msprintf(tmp, "variance", 2, "c", "r", 1, 2))
    end

    // Calculations
    // ------------

    d = size(x, orien) - 1 + exists("m","local") // Denominator. If m is given, then the a priori mean is known and we divide by size(n,orien)

    if rhs == 3 & isnan(m) then
        // This will compute the "biased variance": the denominator is size(x,orien) but the a priori mean is not considered as provided.
        rhs = 2
    end
    if orien=="*" then
        if rhs < 3 then
            m = mean(x)
        end
    else
        if rhs < 3 then
            m = mean(x, orien).*.ones(size(x,1),1)
        else
            if isscalar(m) then
                if or(m==[0 1]) then
                    tmp = _("%s: The significance of input argument #%d has been modified. Please refer to the variance help page.\n")
                    warning(msprintf(tmp, "variance", 3))
                end
                // If m is a scalar, extend it to the size of x.
                // If lhs==1, we do not need to perform this operation, because in the following 'x - m', m can be a scalar
                m = m*ones(x)
            else
                if transposed then
                    m = m.';
                end
                m = m.*.ones(size(x,1),1)
            end
        end
    end

    s = sum(abs(x - m).^2, orien) / d

    m = m(1, :);
    if transposed then
        s = s.'
        m = m.'
    end

endfunction
