// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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

function [p]=perctl(x,y)

    //compute the matrix p  of percentils (in increasing  order,
    //column first) of the real  vector or matrix x indicated by
    //the entries of  y, the  values of entries   of  y must  be
    //positive integers between 0 and 100.  p  is a matrix whose
    //type is length(y)x2 and   the content of its first  column
    //are the  percentils values.   The contents of   its second
    //column  are the places  of the computed percentiles in the
    //input matrix x.
    //
    //References:  HYNDMAN,Rob  J.   and  FAN  Yanan,  Sample
    //Quantiles   in  Statistical   Packages,   The  American
    //Statistician, Nov.1996, Vol 50, No.4
    //
    //fixed: 2006-06-12 ( Pierre MARECHAL, Scilab Team )

    [lhs,rhs]=argn(0)
    if rhs<>2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"perctl",2));
    end
    if x==[]|y==[] then p=[];return;end
    if find((y-int(y)))<>[]|max(y)>100|min(y)<1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"perctl",2,1,100))
    end
    if type(x)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Numerical expected.\n"),"perctl",1))
    end
    lenx = size(x)
    lx   = prod(lenx)
    [val,pos] = gsort(x)
    x1 = [matrix(val,lx,1) matrix(pos,lx,1)]
    x1 = x1(lx-[0:lx-1],:)
    ly = length(y)
    y  = gsort((matrix(y,ly,1)/100)*(lx+1))
    y  = y(ly-[0:ly-1])

    // Now we test if there is enough sample for each asked fractions.

    test1 = find(max(floor(y),1) == 1);
    y(test1) = 1;

    test2 = find(min(ceil(y),lx) == lx);
    y(test2) = lx;

    p = x1(floor(y),:)
    w = find(ceil(y)-floor(y)<>0)

    if w<>[] then
        p(w,1) = ((x1(ceil(y(w)),1)-x1(floor(y(w)),1)).*(y(w)-floor(y(w)))+x1(floor(y(w)),1));
    end
endfunction
