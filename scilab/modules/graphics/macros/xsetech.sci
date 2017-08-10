//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
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
//
function []=xsetech(wrect, frect, logflag, arect)

    // 1 <= RHS <= 3
    if (argn(2) > 3 | argn(2) < 1) then
        error(77, sprintf(_("%s: Wrong number of input argument(s): %d to %d expected."), "xsetech", 1, 3));
    end

    if ~exists("wrect", "local")
        wrect = [];
    else
        // Check wrect
        if (type(wrect) <> 1 | isreal(wrect) == %f | size(wrect) <> [1 4]) then
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "wrect"));
        end
    end

    if ~exists("frect", "local")
        frect = [];
    else
        // Check frect
        if (type(frect) <> 1 | isreal(frect) == %f | size(frect) <> [1 4]) then
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "frect"));
        end
        frect = [frect(1), frect(2); frect(3), frect(4)]
    end

    if ~exists("logflag", "local")
        logflag = [];
    else
        // check logflag
        if (type(logflag) <> 10 | or(logflag == ["ll" "nn" "nl" "ln"]) == %f) then
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "logflag"));
        end
    end

    if ~exists("arect", "local")
        arect = [];
    else
        if (type(arect) <> 1 | isreal(arect) == %f | size(arect) <> [1 4]) then
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "arect"));
        end
    end

    // Check if an existing axes matches wrect
    a = gca()
    curParent = a.parent;
    allAxes = curParent.children;
    found = %F;
    for kAxes = 1:size(allAxes, "*")
        if allAxes(kAxes).type == "Axes" & and(allAxes(kAxes).axes_bounds(:)==wrect(:)) then
            a = sca(allAxes(kAxes));
            found = %T;
            break;
        end
    end

    if ~found then
        a = newaxes(curParent);
        if ~isempty(wrect) then
            a.axes_bounds = wrect;
        end
    end

    if ~isempty(frect) then
        a.data_bounds = frect;
    end
    if ~isempty(logflag) then
        a.log_flags = logflag;
    end
    if ~isempty(arect) then
        a.margins = arect;
    end

endfunction
