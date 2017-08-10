//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

function ret = createBorderFont(varargin)
    [lhs,rhs]=argn(0)

    fontangle = "normal";
    fontweight = "normal";

    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected."), "createBorderFont", 1));
    end

    fontname = varargin(1);
    if typeof(fontname) <> "string" | size(fontname, "*") <> 1 then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorderFont", 1));
    end

    fontsize = varargin(2);
    if typeof(fontsize) <> "constant" | size(fontsize, "*") <> 1 then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer value expected.\n"), "createBorderFont", 2));
    end

    if rhs > 2 then
        fontweight = varargin(3);
        if typeof(fontweight) <> "string" | size(fontweight, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer value expected.\n"), "createBorderFont", 3));
        end

        if fontweight <> ["normal", "bold"] then
            error(999, msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "createBorderFont", 3, "normal", "bold"));
        end
    end

    if rhs > 3 then
        fontangle = varargin(4);
        if typeof(fontangle) <> "string" | size(fontangle, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer value expected.\n"), "createBorderFont", 4));
        end

        if fontangle <> ["normal", "italic"] then
            error(999, msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "createBorderFont", 4, "normal", "italic"));
        end
    end

    ret = tlist(["BorderFont", "name", "size", "angle", "weight"], fontname, fontsize, fontangle, fontweight);

endfunction
