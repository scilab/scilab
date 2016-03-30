// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = nthroot(x,n)

    rhs = argn(2);

    // If the number of input arguments is wrong
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "nthroot", 2));
    end

    // If x or n are not real
    if typeof(x) <> "constant" | ~isreal(x)  then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real scalar or matrix expected.\n"),"nthroot", 1));
    end

    if typeof(n) <> "constant" | ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real scalar or matrix expected.\n"),"nthroot", 2));
    end

    // If n is a vector which size is different from x's
    if (size(n,"*")>1 & size(n,"*")<>size(x,"*")) then
        error(msprintf(gettext("%s: Wrong sizes for input argument #%d and #%d: Same sizes expected.\n"),"nthroot", 1, 2));
    end
    if isempty(x)
        y = [];
        return
    end
    if isempty(n)
        y = x;
        return
    end

    reste = modulo(n,2);
    // Making 'reste' one element
    if (size(n,"*")>1) then
        reste = or(reste(find(x<0))<>0);
    end
    y = x;

    if (or(n==0) & (x>=0 | isnan(x))) then
        // If n = 0 and x = 1 or x = %nan then y = %nan
        y(find((x==1 | isnan(x)) & n==0)) = %nan;
        // If n = 0 and x>1 then y = %inf
        y(find(x>1 & n==0)) = %inf;
        // If n = 0 and x = %eps then y = 0
        y(find(x==%eps & n==0)) = 0;
    end

    if (x==[]) then
        y = [];
    elseif (n==[]) then
        y = x;
        // If n = %nan and x is positive then y = %nan
    elseif (isnan(n) & or(x >= 0)) then
        y(find(x>=0)) = %nan;
    elseif (or (or(x(:)<0) & (or(n~=fix(n)) | reste==0 | reste==%f))) then
        error(msprintf(gettext("%s: If x is negative, then n must contain odd integers only.\n"),"nthroot"));
        // If n ~=0 and n ~= %nan
    elseif (or(n~=0) & ~isnan(n)) then
        // If x = 0 and n is negative and n i~= %nan
        [m1,m2] = size(x(find(x==0 & n<0 & ~isinf(n))));
        y(find(x==0 & n<0 & ~isinf(n))) = (x(find(x==0 & n<0 & ~isinf(n)))+ones(m1,m2)) .*%inf;
        if (size(n,"*") == 1) then
            // If x = 0 and n is positive and n ~= %nan
            y(find(x==0 & (n>0 |isinf(n)))) = x(find(x==0 & (n>0 |isinf(n)))).^(1 ./n(find(n<>0)));
            // If x is positive
            y(find(x>0)) = x(find(x>0)).^(1 ./n(find(n<>0)));
            // If x is negative
            y(find(x<0)) = sign(x(find(x<0))).*(abs(x(find(x<0)))).^(1 ./n(find(n<>0)));
        else
            // If x = 0 and n is positive and n ~= %nan
            y(find(x==0 & (n>0 |isinf(n)))) = x(find(x==0 & (n>0 |isinf(n)))).^(1 ./n(find(x==0 & n<>0)));
            // If x is positive
            y(find(x>0 & n<>0)) = x(find(x>0 & n<>0)).^(1 ./n(find(x>0 & n<>0)));
            // If x is negative
            y(find(x<0 & n<>0)) = sign(x(find(x<0 & n<>0))).*(abs(x(find(x<0)))).^(1 ./n(find(x<0 & n<>0)));
        end
    end

endfunction
