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
