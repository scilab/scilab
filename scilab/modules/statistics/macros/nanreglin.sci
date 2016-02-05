// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [a, b] = nanreglin(x, y, dflag)
    // Solves a linear regression
    // y=a(p,q)*x+b(p,1) + epsilon  where y or x contain NaNs.
    // x : matrix (q,n) and y matrix (p,n)
    // dflag is optional if 1 a display of the result is done
    //!

    [lhs, rhs] = argn(0);
    if rhs < 2 then
        error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanreglin",2,3))
    end
    if rhs <= 2 then
        dflag = 0;
    end
    [n1, n2] = size(x);
    [p1, p2] = size(y);
    if n2 <> p2 then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"nanreglin",1,2));
    end
    if ~(or(isnan(x)) | or(isnan(y))) then
        error(msprintf(_("%s: No NaNs detected, please use %s() instead.\n"), "nanreglin", "reglin"))
    end

    a = zeros(p1, n1);
    b = zeros(p1, 1);
    for i=1:p1
        // A column of x defines an element of y, but each line of y defines an independent problem.
        // If x2(:, j) or y2(i, j) contains a %nan, then both x2(:, j) and y2(j) are removed.
        y2 = y(i, find(~isnan(y(i,:))));
        x2 = x(:, find(~isnan(y(i,:))));
        nanX = isnan(x);
        if or(isnan(x)) then // At least one NaN is x or y.
            columns = floor((find(nanX==%t)-1)./(n1+1)+1);
            x2(:, columns) = [];
            y2(1, columns) = [];
        end
        [a(i, :), b(i)] = reglin(x2, y2, dflag);
    end

endfunction
