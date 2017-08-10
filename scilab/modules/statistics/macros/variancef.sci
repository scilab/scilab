
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul BIGNIER : m parameter added
// Copyright (C) 2013 - Samuel GOUGEON : http://bugzilla.scilab.org/11209 fixed
// Copyright (C) 2000 - INRIA - Carlos Klimann
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

function [s, m] = variancef(x, fre, orien, m)
    //
    //This function  computes  the variance  of the values  of   a vector or
    //matrix x, each  of  them  counted with  a  frequency signaled   by the
    //corresponding values of the integer vector or matrix fre with the same
    //type of x.
    //
    //For a vector or matrix  x, s=variancef(x,fre) (or s=variancef(x,fre,'*') returns
    //in scalar s the variance  of all the  entries of x, each value counted
    //with the multiplicity indicated by the corresponding value of fre.
    //
    //s=variancef(x,fre,'r')(or,   equivalently, s=variancef(x,fre,1)) returns in each
    //entry of the row vector s  of type 1xsize(x,'c')  the variance of each
    //column of x, each value counted with the multiplicity indicated by the
    //corresponding value of fre.
    //
    //s=variancef(x,fre,'c')(or, equivalently,   s=variancef(x,fre,2)) returns in each
    //entry of  the column vector  s of type   size(x,'c')x1 the variance of
    //each row of  x, each value counted with  the multiplicity indicated by
    //the corresponding value of fre.
    //
    //The input argument m represents the a priori mean. If it is present, then the sum is
    //divided by n. Otherwise ("sample variance"), it is divided by n-1.
    //
    //

    [lhs,rhs] = argn(0)
    if rhs<2 | rhs>4 then
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "variancef", 2, 4))
    end
    if x==[] | fre==[] | fre==0
        s = %nan
        return
    end
    if rhs==2 then
        sumfre = sum(fre)
        if sumfre <= 1 then
            msg = gettext("%s: Wrong value for input argument #%d: Must be > %d.\n")
        error(msprintf(msg, "variancef", 2, 1)), end
        m = meanf(x,fre)
        s = sum((abs(x-m).^2).*fre) / (sumfre-1)
        return
    end
    biased = %f
    if rhs==4 then
        if typeof(m)~="constant" then
            tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
            error(msprintf(tmp, "variancef", ))
        elseif orien=="*" then
            if ~isscalar(m) then
                tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
                error(msprintf(tmp, "variancef", ))
            end
        elseif orien=="r" | orien==1 then
            if size(m)~=[1 size(x,"c")] & ~isscalar(m) then
                tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
                error(msprintf(tmp, "variancef", ))
            end
        elseif orien=="c" | orien==2 then
            if size(m)~=[size(x,"r") 1] & ~isscalar(m) then
                tmp = gettext("%s: Wrong value of m : a priori mean expected.\n")
                error(msprintf(tmp, "variancef", ))
            end
        end
        if isnan(m) then
            biased = %t; // Compute the biased variance
        end
    end
    if orien=="*",
        sumfre = sum(fre)
        if sumfre <= 1 then
            msg = _("%s: Wrong value for input argument #%d: Must be > %d.\n")
        error(msprintf(msg, "variancef", 2, 1)),end
        if rhs<4 then
            m = meanf(x,fre)
            s = sum((abs(x-m).^2).*fre) / (sumfre-1)
        elseif biased == %t
            m = meanf(x,fre)
            s = sum((abs(x-m).^2).*fre) / sumfre
        else
            s = sum((abs(x-m).^2).*fre) / sumfre
        end
    elseif orien=="r" | orien==1,
        sumfre = sum(fre, "r")
        if or(sumfre==0) then
            msg = _("%s: Wrong value for input argument #%d: Must be > %d.\n")
            error(msprintf(msg, "variancef", 2, 1))
        end
        if rhs<4 | biased == %t then
            m = meanf(x,fre,"r")
        elseif isscalar(m) then
            m = m*ones(1, size(x,"c"));
        end
        m2 = ones(size(x,"r"),1)*m
        if rhs<4 then
            s = sum((abs(x-m2).^2).*fre, "r") ./ (sumfre-1)
        else
            s = sum((abs(x-m2).^2).*fre, "r") ./ sumfre
        end
    elseif orien=="c" | orien==2,
        sumfre = sum(fre, "c")
        if or(sumfre==0) then
            msg = _("%s: Wrong value for input argument #%d: Must be > %d.\n")
            error(msprintf(msg, "variancef", 2, 1))
        end
        if rhs<4 | biased == %t then
            m = meanf(x,fre,"c")
        elseif isscalar(m) then
            m = m*ones(size(x,"r"), 1);
        end
        m2 = m*ones(1,size(x,"c"))
        if rhs<4 then
            s = sum((abs(x-m2).^2).*fre, "c") ./ (sumfre-1)
        else
            s = sum((abs(x-m2).^2).*fre, "c") ./ sumfre
        end
    else
        msg = _("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n")
        error(msprintf(msg, "variancef", 3, "*", "c", "r", 1, 2))
    end

endfunction
