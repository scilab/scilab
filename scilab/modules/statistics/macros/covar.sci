
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

function [s]=covar(x,y,fre)
    //
    //This function computes the  covariance  of two variables x
    //and  y where x  is a numerical vector of  length n, y is a
    //numerical   vector  of length m and    fre is an  array of
    //dimensions  nxm.   In fre  the  element  of indices  (i,j)
    //corresponds to the number or frequencies of x_i&y_j.
    //
    //References: Wonacott,  T.H. & Wonacott, R.J.; Introductory Statistics,
    //J.Wiley & Sons, 1990.
    //
    //
    //small correction (bug number 1072) date : 8 nov 2004
    //
    [lhs,rhs]=argn(0)
    if rhs <> 3 then error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"covar",3)), end
    if x==[] | y==[] then s=%nan; return, end
    [lfre cfre]=size(fre);
    [lx cx]=size(x)
    [ly cy]=size(y)
    if lx<>1 & cx<>1 then error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector expected.\n"),"covar",1)), end
    if ly<>1 & cy<>1 then error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector expected.\n"),"covar",2)), end
    fre(isnan(fre))=0
    lx=lx*cx;
    cy=ly*cy;
    if lx<>lfre then error(msprintf(gettext("%s: Wrong value for input argument #%d: Same number of line as first input argument expected.\n"),"covar",3)), end
    if cy<>cfre then error(msprintf(gettext("%s: Wrong value for input argument #%d: Same number of column as first input argument expected.\n"),"covar",3)), end
    x=matrix(x,lx,1);
    y=matrix(y,1,cy);
    fr=fre/sum(fre)
    s=sum(((x-sum(x.*sum(fr,"c")))*ones(1,cy)) ..
    .* (fr) .* (ones(lx,1)*(y-sum(y.* sum(fr,"r")))))
endfunction
