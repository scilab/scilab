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

function ret = createLayoutOptions(constType, varargin)
    [lhs,rhs]=argn(0)

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected."), "createLayoutOptions", 1));
    end

    ret = [];
    if constType == "nolayout" | constType == "none" then
        ret = tlist(["OptNoLayout"]);
    elseif constType == "border" then
        arg1 = [0 0]

        //hpad, vpad
        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "constant" | size(arg1, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createLayoutOptions", 2));
            end
        end

        ret = tlist(["OptBorder","padding"], arg1);
    elseif constType == "grid" then
        arg1 = [0 0];
        arg2 = [0 0];

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "constant" | size(arg1, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createLayoutOptions", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "constant" | size(arg2, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createLayoutOptions", 3));
            end
        end
        ret = tlist(["OptGrid","grid","padding"], arg1, arg2);
    elseif constType == "gridbag" then
        ret = tlist(["OptGridBag"]);
    else
        error(999, msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "createLayoutOptions", 1, "none/nolayout, border, grid, gridbag"));
    end
endfunction
