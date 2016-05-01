
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
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

function [s] = mtlb_var (x,w,dim)
    //
    // mtlb_var --
    //   This function computes  the  variance  of  the values of  a  vector or
    //   matrix x.
    //   It provides the same service as Octave and Matlab.
    // Arguments
    //   x may be a real or a complex vector or matrix.
    //   s is a real scalar or real vector.
    //     If x is a vector, s = mtlb_var(x) is the variance of x.
    //     If x is a matrix, s = mtlb_var(x) is a row vector containing the variance of each column of x.
    //   w : type of normalization to use. Valid values are, depending on the number of columns m of x :
    //     w = 0 : normalizes with m-1, provides the best unbiased estimator of the variance [default].
    //     w = 1: normalizes with m, this provides the second moment around the mean.
    //   dim : the dimension along which the variance is computed (default is 1, i.e. column by column)
    //     If dim is 2, the variance is computed row by row.
    //

    if x==[] then s=%nan, return, end
    [lhs,rhs]=argn(0)
    if rhs==0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"mtlb_var",1,2))
    end
    [m n]=size(x);
    
    if rhs == 1 then
        w = 0;
    end
    if rhs<=2 then
        dim=1;
    end
    if dim==1 then
        if w==0 then
            if m==1 then
                error(msprintf(gettext("%s: The normalization m - 1 factor is zero.\n"),"mtlb_var"))
            end
        end
    end
    if dim==2 then
        if w==1 then
            if n==1 then
                error(msprintf(gettext("%s: The normalization n - 1 factor is zero.\n"),"mtlb_var"))
            end
        end
    end
    
    query = warning("query");
    warning("off")
    if rhs == 1 | w == 0 then
        s = variance(x,dim);
    else
        s = variance(x,dim,mean(x, dim));
    end
    warning(query)

endfunction

