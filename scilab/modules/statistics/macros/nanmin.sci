
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
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


function [s,index] = nanmin(x,orient)
    //
    //This function gives for a real or a numerical matrix  x a his smallest
    //element m (but ignoring the NANs).
    //
    //For x,  a numerical vector or matrix,  m=nanmin(x) returns in scalar m
    //the smallest element of x  (ignoring the NANs).   The form [m,index] =
    //nanmin(x,orient)   gives  in addition  of  the  value  of the smallest
    //element of x (ignoring the NANs) in scalar  m, index the index of this
    //element in x, as a 2-vector.
    //
    //m=nanmin(x,'r')    gives in  the  1xsize(x,2)   matrix  m the smallest
    //elements  (ignoring  the  NANs) of  each  column  of  x.   If the form
    //[m,index]=nanmin(x,'r')  is used,   the  elements of  the  1xsize(x,2)
    //matrix are the indexes of the smallest elements (ignoring the NANs) of
    //each column of x in the corresponding column.
    //
    //m=nanmin(x,'c')   gives in   the  size(x,2)x1 matrix   m  the smallest
    //elements  (ignoring  the  NANs)  of  each row   of  x.    If the  form
    //[m,index]=nanmin(x,'c')  is   used, the  elements   of the size(x,2)x1
    //matrix are the indexes of the smallest elements (ignoring the NANs) of
    //each row of x in the corresponding row.
    //
    //

    [lhs, rhs] = argn(0)
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanmin",1,2))
    end
    if rhs==1 then  orient="*",end
    if orient==1 then orient="r",end
    if orient==2 then orient="c",end
    if x==[]|(size(x,"*")==1&isnan(x)) then s=[],index=[],return,end
    isn=isnan(x)
    x(isn)=%inf
    [s,index]=min(x,orient)
    s(find(and(isn,orient)))=%nan

endfunction
