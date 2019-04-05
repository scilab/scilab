// Copyright (C) 1993 - 1995 - Anders Holtsberg
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
// Copyright (C) 2012-2013 - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function C = cov(x,y,nrmlztn)
    // Sample covariance or cross covariance matrix
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
    // y: a nobs-by-1 or nobs-by-mvar matrix of doubles
    // C: a square matrix of doubles, the sample (cross)covariance 
    //
    // Description
    // If x is a vector of length nobs then cov(x) returns the sample
    // variance of x, normalized by nobs-1.
    //
    // If x  and y are vectors of length nobs whose components are a sample
    // of nobs observations of two randown variables X and Y ∈ R
    // cov(x, y) returns the 2-by-2 sample covariance matrix of x and
    // y, normalized by nobs-1, where nobs is the number of observations, i.e.
    // an unbiased estimator of the covariance matrix 
    // E[(X-E[X])^2]         E[(X-E[X])*(Y-E[Y]]
    // E[(X-E[X])*(Y-E[Y]]   E[(Y-E[Y])^2]
    //
    // If x is a nobs-by-nvar matrix whose rows are a sample of nobs
    // observations of a random vector X ∈ R^nvar
    // cov(x) returns the nvar-by-nvar sample covariance matrix of X
    // normalized by nobs-1, i.e. an unbiased estimator of the covariance
    // matrix E[(X-E[X])(X-E[X])^T]
    //
    // If x is a nobs-by-nvar matrix and y a nobs-by-mvar matrix whose
    // rows are samples of nobs observations of random vectors X ∈ R^nvar
    // Y ∈ R^mvar
    // cov(x) returns the nvar-by-mvar sample cross-covariance matrix of X
    // and Y normalized by nobs-1, i.e. an unbiased estimator of the 
    // cross-covariance matrix E[(X-E[X])(Y-E[Y])^T]
    // 
    // cov(x, 0) is the same as cov(x) and
    // cov(x, y, 0) is the same as cov(x, y).
    //
    // cov(x, 1) and cov(x, y, 1) normalize by nobs.
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
    if rhs ==0
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"),"cov", 1, 2, 3));
    elseif rhs == 1
        y = 0
    end
    //
    // Check type
    if (typeof(x) <> "constant")
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1));
    end

    if (rhs <= 2) then
        //
        if (typeof(y) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer or a real matrix expected.\n"),"cov", 2));
        end
        //
        // Check size
        if isscalar(x) & isscalar(y)
            if y == 0 || y == 1
                C = 0;
            else
                C = zeros(2,2);
            end
            return
        elseif isscalar(y)
            if y <> 0 & y <> 1
                error(msprintf(gettext("%s: Wrong value for input argument #%d: %d or %d expected.\n"),"cov", 2, 0, 1));
            end
            //
            if isvector(x)
                nobs = length(x)
                r = 1 / ( nobs - 1 + y)
                C = r * (norm(x)^2 - nobs*mean(x)^2);
            else
                nobs = size(x, "r")
                r = 1 / (nobs - 1 + y)
                C = %_cov(x,r);
            end
            return
        end
        nrmlztn = 0;
    end

    if (rhs == 3)
        if (typeof(y) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 2));
        end
        if (typeof(nrmlztn) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer expected.\n"),"cov", 3));
        end
        if ( ~isscalar(nrmlztn) || (nrmlztn <> 0 & nrmlztn <> 1))
            error(msprintf(gettext("%s: Wrong value for input argument #%d: %d or %d expected.\n"),"cov", 3, 0, 1));
        end
    end

    if isvector(x) & isvector(y)
        nobs = length(x)
        if nobs <> length(y)
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected"),"cov", 1, 2));
        end
        r = 1 / (nobs - 1 + nrmlztn);
        mx = mean(x);
        my = mean(y);
        C = r*[norm(x)^2 - nobs*mx^2, x'*y - nobs*mx*my
                                   0, norm(y)^2 - nobs*my^2];
        C(2,1)=C(1,2);
    elseif size(x,"r") <> size(y,"r")
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n"),"cov", 1, 2));
    else
        nobs = size(x,"r")
        r = 1 / (nobs - 1 + nrmlztn);
        C = %_cov(x,y,r);
    end

endfunction
