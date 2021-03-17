// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = %sp_mean(x, orient)
    
    // CHECK ARGUMENTS
    // ---------------
    // Number of inputs already checked in mean() that calls %sp_mean()
    // orient
    if ~isdef("orient","l") then
        orient = "*"
    else
        ok = %t
        try
            if size(orient,"*") > 1
                msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
                error(msprintf(msg, "mean", 2))
            end
        catch
            ok = %f
        end
        if ~ok | and(type(orient)<>[1 10]) | ..
                 (orient<>["r" "c" "m"] & orient<>1:2)
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "mean", 2, "1,2,""r"",""c"",""m"""))
        end
        if orient == "m" then
            orient = find(size(x) > 1, 1);
            if orient == [] then
                orient = "*"
            end
        end
    end

    // ----------
    // PROCESSING
    // ----------
    if isempty(x) then
        if orient=="*"
            y = mean([])
        else
            y = sparse(mean([], orient))
        end
        return
    end
    if orient == "*" then
        y = sum(x) / size(x,"*")
    else
        if orient=="r" then
            orient = 1
        elseif orient=="c"
            orient = 2
        end
        y = sum(x, orient) ./ size(x,orient)
    end
endfunction
