// Copyright (C) 2012-2013 - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
// Copyright (C) 1993 - 1995 - Anders Holtsberg
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function C = cov(varargin)
    // Covariance matrix
    //
    // Syntax
    //   C = cov(x)
    //   C = cov(x, 0)
    //   C = cov(x, 1)
    //   C = cov(x, y)
    //   C = cov(x, y, 0)
    //   C = cov(x, y, 1)
    //
    // Parameters
    // x: a nobs-by-1 or nobs-by-nvar matrix of doubles
    // y: a nobs-by-1 or nobs-by-nvar matrix of doubles
    // C: a square matrix of doubles, the empirical covariance
    //
    // Description
    // If x is a nobs-by-1 matrix,
    // then cov(x) returns the variance of x,
    // normalized by nobs-1.
    //
    // If x is a nobs-by-nvar matrix,
    // then cov(x) returns the nvar-by-nvar covariance matrix of the
    // columns of x, normalized by nobs-1.
    // Here, each column of x is a variable and
    // each row of x is an observation.
    //
    // If x and y are two nobs-by-1 matrices,
    // then cov(x, y) returns the 2-by-2 covariance matrix of x and
    // y, normalized by nobs-1, where nobs is the number of observations.
    //
    // cov(x, 0) is the same as cov(x) and
    // cov(x, y, 0) is the same as cov(x, y).
    // In this case, if the population is from a normal distribution,
    // then C is the best unbiased estimate of the covariance matrix.
    //
    // cov(x, 1) and cov(x, y, 1) normalize by nobs.
    // In this case, C is the second moment matrix of the
    // observations about their mean.
    //
    // The covariance of X and Y is defined by:
    //
    // Cov(X, Y) = E( (X-E(X)) (Y-E(Y))^T )
    //
    // where E is the expectation.
    //
    // This function is compatible with Matlab.
    //
    // Examples
    // x = [1; 2];
    // y = [3; 4];
    // C = cov(x, y)
    // expected = [0.5, 0.5; 0.5, 0.5]
    // C = cov([x, y])
    //
    // x = [230; 181; 165; 150; 97; 192; 181; 189; 172; 170];
    // y = [125; 99; 97; 115; 120; 100; 80; 90; 95; 125];
    // expected = [
    // 1152.4556, -88.911111
    // -88.911111, 244.26667
    // ]
    // C = cov(x, y)
    // C = cov([x, y])
    //
    // // Source [3]
    // A = [
    // 4.0 2.0 0.60
    // 4.2 2.1 0.59
    // 3.9 2.0 0.58
    // 4.3 2.1 0.62
    // 4.1 2.2 0.63
    // ];
    // S = [
    // 0.025 0.0075 0.00175
    // 0.0075 0.007 0.00135
    // 0.00175 0.00135 0.00043
    // ];
    // C = cov(A)
    //
    // Bibliography
    // [1] http://en.wikipedia.org/wiki/Covariance_matrix
    // [2] "Introduction to probability and statistics for engineers and scientists.", Sheldon Ross
    // [3] NIST/SEMATECH e-Handbook of Statistical Methods, 6.5.4.1. Mean Vector and Covariance Matrix, http://www.itl.nist.gov/div898/handbook/pmc/section5/pmc541.htm


    [lhs, rhs]=argn()
    //
    if (rhs == 1) then
        x = varargin(1)
        //
        // Check type
        if (typeof(x) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1));
        end
        nobs = size(x, "r")
        r = 1/(nobs-1)
        A = x
    elseif (rhs == 2) then
        //
        x = varargin(1)
        y = varargin(2)
        //
        // Check type
        if (typeof(x) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1));
        end
        if (typeof(y) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer or a real matrix expected.\n"),"cov", 2));
        end
        //
        // Check size
        nobs = size(x, "r")
        if (size(y, "*") == 1) then
            if (y <> 0 & y <> 1)
                error(msprintf(gettext("%s: Wrong value for input argument #%d: %d or %d expected.\n"),"cov", 2, 0, 1));
            end
            if (y == 1) then
                r = 1/nobs
                A = x
            elseif (y == 0) then
                r = 1/(nobs-1)
                A = x
            end
        else
            if (size(x) <> [nobs 1]) then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: %dx%d expected.\n"),"cov", 1, nobs, 1));
            end
            if (size(y) <> [nobs 1]) then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: %dx%d expected.\n"),"cov", 2, nobs, 1));
            end
            r = 1/(nobs-1)
            A = [x, y]
        end
    elseif (rhs == 3) then
        //
        x = varargin(1)
        y = varargin(2)
        nrmlztn = varargin(3)
        //
        // Check type
        if (typeof(x) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1));
        end
        if (typeof(y) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 2));
        end
        if (typeof(nrmlztn) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer expected.\n"),"cov", 3));
        end
        //
        // Check size
        nobs = size(x, "r")
        if (size(x) <> [nobs 1]) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %dx%d expected.\n"),"cov", 1, nobs, 1));
        end
        if (size(y) <> [nobs 1]) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %dx%d expected.\n"),"cov", 2, nobs, 1));
        end
        if (size(nrmlztn, "*") <> 1) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer expected.\n"),"cov", 3));
        end
        //
        // Check content
        if (nrmlztn <> 0 & nrmlztn <> 1)
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %d or %d expected.\n"),"cov", 3, 0, 1));
        end
        A = [x, y]
        if (nrmlztn == 1) then
            r = 1/nobs
        else
            r = 1/(nobs-1)
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"),"cov", 1, 2, 3));
    end
    //
    // Compute with A in the general case
    nvar = size(A, "c")
    nobs = size(A, "r")
    for i = 1:nvar
        A(:,i) = A(:,i) - mean(A(:,i))
    end
    C = zeros(nvar, nvar)
    for i = 1:nvar
        C(i,i) = A(:,i)'*A(:,i)*r
        for j = i+1:nvar
            C(i,j) = A(:,i)'*A(:,j)*r
            C(j,i) = C(i,j)
        end
    end
endfunction
