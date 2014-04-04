//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
        error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorderFont", 1));
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
