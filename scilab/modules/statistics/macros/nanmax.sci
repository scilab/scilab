
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 200 - INRIA - Carlos Klimann
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


function [s,index] = nanmax(x,orient)
    //
    //This function gives for a  real or a  numerical matrix x a his largest
    //element m (but ignoring the NANs).
    //
    //For x,  a numerical vector or matrix,  m=nanmax(x) returns in scalar m
    //the largest  element of x (ignoring the  NANs).  The form  [m,index] =
    //nanmax(x,orient) gives in addition of the value of the largest element
    //of x (ignoring the NANs) in scalar m, and    the index of this element
    //in x, as a  2-vector.
    //
    //m=nanmax(x,'r') gives in the 1xsize(x,2) matrix m the largest elements
    //(ignoring the    NANs)   of each   column    of  x.   If   the    form
    //[m,index]=nanmax(x,'r')  is used,    the elements of  the  1xsize(x,2)
    //matrix index are the indexes  of the  largest  elements  (ignoring the
    //NANs) of each column of x in the corresponding column.
    //
    //m=nanmax(x,'c') gives in the size(x,2)x1 matrix m the largest elements
    //(ignoring    the    NANs) of    each   row   of   x.    If  the   form
    //[m,index]=nanmax(x,'c') is  used,   the  elements of   the size(x,2)x1
    //matrix index are  the  indexes of the  largest  elements (ignoring the
    //NANs) of each row of x in the corresponding row.
    //
    //
    // correction of bug 1239 (Bruno)

    [lhs, rhs] = argn(0)
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanmax",1,2))
    end
    if rhs==1 then  orient="*",end
    if orient==1 then orient="r",end
    if orient==2 then orient="c",end
    if x==[]|(size(x,"*")==1&isnan(x)) then s=[],index=[],return,end
    isn=isnan(x)
    x(isn)=-%inf
    [s,index]=max(x,orient)
    s(find(and(isn,orient)))=%nan

endfunction
