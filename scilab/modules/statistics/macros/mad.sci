
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// corrected by bruno pincon (2006-08-16) (fix for bug 2092)
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

function [s2]=mad(x,orien)
    //
    //This function   computes the mean  absolute  deviation of a  vector or
    //matrix x.
    //
    //For  a vector or  matrix x,  s2=mad(x)  returns in scalar s2  the mean
    //absolute deviation of all the entries of x.
    //
    //s2=mad(x,'r')(or, equivalently,  s2=mad(x,1)) returns in each entry of
    //the column vector s2 the mean absolute deviation of each column of x.
    //
    //s2=mad(x,'c')(or, equivalently, s2=mad(x,2))  returns in each entry of
    //the column vector s2 the mean absolute deviation of each row of x.
    //
    //Reference: Wonacott  T.H.& Wonacott  R.J. .-  Introductory Statistics,
    //5th edition, John Wiley, 1990.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    [lhs,rhs]=argn()
    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"mad",1,2)),
    end

    if x==[] then s2=%nan, return, end

    [nrow,ncol] = size(x);
    if rhs==1 then
        s2=sum(abs(x-mean(x)))/(nrow*ncol)
    else // rhs == 2
        if orien=="r" | orien==1 then
            s2=sum(abs(x-(ones(nrow,1)*mean(x,1))),1)/nrow
        elseif orien=="c" | orien==2 then
            s2=sum(abs(x-(mean(x,2)*ones(1,ncol))),2)/ncol
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"mad",2, "r","c",1,2)),
        end
    end
endfunction
