
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// Copyright (C) 2010 - Samuel GOUGEON : bug 7204, code style
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

function gm = geomean(x,orien)
    //
    //This function computes the geometric mean of a vector or matrix x.
    //
    //For a vector  or  matrix  x,  gm=geomean(x) returns in  scalar  gm the
    //geometric mean of all the entries of x.
    //
    //gm=geomean(x,'r')(or,  equivalently,   gm=gmean(x,1)) returns  in each
    //entry of the row vector gm the geometric mean of each column of x.
    //
    //gm=geomean(x,'c')(or,   equivalently,  gm=gmean(x,2)) returns in  each
    //entry of the column vector gm the geometric mean of each row of x.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    //
    [lhs, rhs] = argn(0)
    if rhs == 0 then
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "geomean",1,2))
    end
    if x == [] then
        gm = %nan
        return
    end
    if rhs == 1 then
        gm = prod(x)^(1/size(x,"*"))
    elseif rhs==2
        gm = prod(x,orien).^(1/size(x,orien))
    else
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "geomean",1,2))
    end
endfunction
