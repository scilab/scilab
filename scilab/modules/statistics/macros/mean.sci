
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


function [y] = mean(x,orient)

    //
    // NOTES
    //    - modified by bruno to handle the case when x is an hypermatrix
    //    - restricted to matrix or hypermatrix of "constants" (that is
    //      real or complex numbers)
    //    - mean([]) return nan but mean([],orient) return [] (this
    //      is strange but corresponds to matlab behavior)

    [lhs,rhs] = argn()
    // some arguments checking
    if rhs == 0 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"), "mean", 1, 2)),
    else
        if type(x) <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Real vector or matrix expected.\n"), "mean", 1))
        end

        if rhs == 2 then
            if orient == "r" then
                orient = 1;
            end

            if orient == "c" then
                orient = 2;
            end

            if orient == "m" then
                flag = find(size(x) > 1, 1);
                if flag == [] then
                    y = mean(x);
                    return
                else
                    y = mean(x, flag);
                    return
                end
            end
            if floor(orient) <> orient | orient < 1 | orient > length(size(x)) then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar or vector expected.\n"), "mean", 2)),
            end
        end
    end

    if rhs == 1 then
        if x == [] then
            y = %nan;
            return
        end

        y = sum(x) / size(x, "*");
    else
        if x == [] then
            y = [];
            return
        end

        y = sum(x, orient) / size(x, orient);
    end
endfunction
