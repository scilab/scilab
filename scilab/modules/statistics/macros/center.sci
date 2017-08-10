
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Carlos Klimann
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

function [s]=center(x,orient)
    //
    //This function  computes s,  the centered version  of the
    //numerical matrix x.
    //
    //For a vector  or a matrix x, s=center(x)  returns in the
    //(i,j)   coefficient   of   the   matrix  s   the   value
    //(x(i,j)-xbar), where  xbar is the mean of  the values of
    //the coefficients of x.
    //
    //s=center(x,'r') (or, equivalently, s=center(x,1)) is the
    //rowwise center reduction of  the values of x. It returns
    //in  the entry  s(i,j) the  value  (x(i,j)-xbarv(j)) with
    //xbarv(j) the mean of the values of the j column.
    //
    //s=center(x,'c') (or, equivalently, s=center(x,2)) is the
    //columnwise  centre reduction  of  the values  of x.   It
    //returns in the  entry s(i,j) the value (x(i,j)-xbarh(i))
    //with xbarh(i) the mean of the values of the i row.
    //

    [lhs,rhs]=argn(0)
    if (rhs<1)|(rhs>2) then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"center",1,2)), end
    if x==[] then s=%nan, return, end
    [m n]=size(x);
    if rhs==1
        xbar=(sum(x)/(m*n))
        s=x-(ones(m,n)*xbar)
    elseif orient=="c"|orient==2 then
        xbar=sum(x,"c")/n
        s=x-(xbar*ones(1,n))
    elseif orient=="r"|orient==1 then
        xbar=sum(x,"r")/m
        s=x-(ones(m,1)*xbar)
    else error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"center",2,"r","c",1,2)),
    end
endfunction
