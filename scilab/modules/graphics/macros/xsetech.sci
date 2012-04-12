//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function []=xsetech(wrect, frect, logflag, arect)

    // 1 <= RHS <= 3
    if (argn(2) > 3 | argn(2) < 1)
        error(77, sprintf(_("%s: Wrong number of input argument(s): %d to %d expected."), "xsetech", 1, 3));
    end

    // If argument is not given, use properties from default axes.
    defaultAxes = gda();
    if ~exists("wrect", "local")
        wrect = defaultAxes.axes_bounds;
    else
        // Check wrect
        if (type(wrect) <> 1 | isreal(wrect) == %f | size(wrect) <> [1 4])
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "wrect"));
        end
    end

    if ~exists("frect", "local")
        frect = defaultAxes.data_bounds;
    else
        // Check frect
        if (type(wrect) <> 1 | isreal(wrect) == %f | size(wrect) <> [1 4])
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "frect"));
        end
        frect = [frect(1), frect(2); frect(3), frect(4)]
    end

    if ~exists("logflag", "local")
        logflag = defaultAxes.log_flags;
    else
        // check logflag
        if (type(logflag) <> 10 | or(logflag == ["ll" "nn" "nl" "ln"]) == %f)
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "logflag"));
        end
    end

    if ~exists("arect", "local")
        arect = defaultAxes.margins;
    else
        if (type(arect) <> 1 | isreal(arect) == %f | size(arect) <> [1 4])
            error(77, sprintf(_("%s: Wrong value given for %s."), "xsetech", "arect"));
        end
    end

    a = newaxes();
    a.axes_bounds = wrect;
    a.data_bounds = frect;
    a.log_flags = logflag;
    a.margins = arect;

endfunction
