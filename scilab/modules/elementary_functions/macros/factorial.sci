// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [f, p, m] = factorial(n)

    // This function returns the factorial n. If n is a vector, matrix or
    //  hypermatrix it returns the element wise factorial
    // Input : n, a scalar/vector/matrix/hypermat of positives integers.
    // Output : 
    //  - f: the factorials
    //  - p: the powers of 10
    //  - m: the mantissae, in [1, 10[

    [lhs, rhs] = argn();

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    if rhs <> 1 then
        msg = gettext("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "factorial", 1));
    end

    if (type(n) <> 1) | (n~=[] & (or((n-floor(n)<>0)) | or(n<0))) then
        msg = gettext("%s: Argument #%d: Non-negative integers expected.\n")
        error(msprintf(msg, "factorial", 1));
    end

    // TRIVIAL CASE
    if n==[]
        f = []
        return
    end

    // PROCESSING
    // ----------
    f = n
    f(n==0) = 1
    f(n>170) = %inf;
    k = find(n>0 & n<171);
    if k~=[] then
        ntemp = cumprod(1:max(n(k)))
        f(k) = ntemp(n(k))
        //f(k) = gamma(n(k)+1)  // slower
    end

    if lhs>1 then
        p = n
        m = n
        p(n==0) = 0
        m(n==0) = 1
        if k~=[]
            p(k) = int(log10(f(k)));
            m(k) = f(k) ./ (10 .^(p(k)))
        end

        // General Stirling formula:
        // n! ~ sqrt(2*%pi*n)*(n/%e)^n * A
        // with A = sum(C ./ (n.^0:5))
        // and  C = [1, 1/12, 1/288, -139/51840, -571/2488320, 163879/209018880]
        // log(n!) = (log(2*%pi)+log(n))/2 + n*[log(n)-1] + log(A)
        //         =  log(2*%pi)/2 + log(n)*(n+1/2) - n + log(A)
        k = find(n>170)
        if k ~= []
            // (a straightforward processing with gammaln(n+1) is less accurate
            // than the following, by a factor of 1 to 100 (see unit tests))
            num = [1,  1,   1,  -139,    -571,    163879,     5246819,   -534703531]
            den = [1, 12, 288, 51840, 2488320, 209018880, 75246796800, 902961561600]
            // Next terms are listed herebelow. They don't increase the accuracy within %eps
            //num = [num,    -4483131259,    432261921612371,     6232523202521089]
            //den = [den, 86684309913600, 514904800886784000, 86504006548979712000]
            c = num ./ den
            q = length(c);
            A = sum((ones(length(k),1)*c) ./ (n(k)(:)*ones(1,q)).^(ones(length(k),1)*(0:q-1)),"c");
            // Careful processing of the  fractional part: we try to always
            // keep decimals with an integer part as small as possible (and so
            // the biggest number of fractional digits as possible):
            tmp = log10(n(k)(:))
            in = int(tmp).*n(k)(:)
            tmp2 = (tmp - int(tmp)).*n(k)(:);
            in = in + int(tmp2);
            fr = tmp2 - int(tmp2);
            
            tmp2 = tmp/2;
            in = in + int(tmp2);
            fr = fr + (tmp2 - int(tmp2)); 
            
            tmp = n(k)(:)/log(10);
            in = in - int(tmp);
            fr = fr - (tmp-int(tmp)) + log10(2*%pi)/2 + log10(A);
            in = in + floor(fr);
            fr = fr - floor(fr);
            p(k) = in;
            m(k) = 10 .^ fr
            m = matrix(m, size(n));
        end
    end
endfunction
