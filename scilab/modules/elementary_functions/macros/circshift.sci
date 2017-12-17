// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function R = circshift(M, sh, d)

    // SPECIAL SIMPLE CASES
    // --------------------
    if argn(2) == 0 then
        R = []
        return
    end

    Fname = "circshift"

    // M must be sizeable (including for custom types)
    try
        s = size(M)
        n = size(M,"*")
    catch
        msg = _("%s: Argument #%d: Wrong type. A sizeable type expected.\n");
        error(msprintf(msg, Fname, 1));
    end

    if or(n == [0, 1]) then
        R = M
        return
    end

    nd = length(s)    // Number of Dimensions (without using ndims())

    // CHECKING INPUT ARGUMENTS. DEFAULTS
    // ----------------------------------
    if ~isdef("sh", "l") then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n");
        error(msprintf(msg, Fname, 2, 3));
    end
    if and(type(sh) ~= [1 8]) | ~and(sh == int(sh)) then
        msg = _("%s: Argument #%d: Integer(s) expected.\n");
        error(msprintf(msg, Fname, 2));
    end
    if length(sh) > nd then
        msg = _("%s: Argument #%d: Must be in the interval [%s, %s].\n");
        error(msprintf(msg, Fname, 2, "1", msprintf("%s\n", nd)));
    end
    sh = double(sh);
    //
    if ~isdef("d", "l") then
        if length(sh) == 1 then
            d = find(s > 1, 1)
            if d == [] then
                d = 1
            end
        else
            d = 1:length(sh)
        end
    else
        if and(type(d) ~= [1 8]) | ~and(d == int(d)) then
            msg = _("%s: Argument #%d: Integer(s) expected.\n");
            error(msprintf(msg, Fname, 3));
        end
        d = double(d);
        if (length(d) == 1 & (d < 0 | d > nd)) | (length(d) > 1 & or(d < 1 | d > nd)) then
            msg = _("%s: Argument #%d: Must be in the interval [%s, %s].\n");
            error(msprintf(msg, Fname, 3, "1", msprintf("%d\n", nd)));
        end
    end

    // PROCESSING
    // ----------
    R = M

    // Shift of linearized indices
    if length(d) == 1 & d == 0 then
        if sh > 0 then
            R(:) = M([n-sh+1:n 1:(n-sh)]);
        else
            R(:) = M([1-sh:n 1:-sh]);
        end
        return
    end

    // Shifts of ranges:
    for i = 1:length(sh)
        si = s(d(i));
        if si > 1 then
            D = pmodulo(sh(i), si)
            if D ~= 0 then
                S = emptystr(1, nd) + ":"
                S(d(i)) = "[si-D+1:si 1:si-D]"
                S = strcat(S, ",")
                execstr("R = R("+S+")")
            end
        end
    end
endfunction
