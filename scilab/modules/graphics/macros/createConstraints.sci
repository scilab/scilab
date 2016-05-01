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

function ret = createConstraints(constType, varargin)
    [lhs,rhs]=argn(0)

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected."), "createConstraints", 1));
    end

    ret = [];
    if constType == "nolayout" | constType == "none" then
        ret = tlist(["NoLayoutConstraint"]);
    elseif constType == "grid" then
        ret = tlist(["GridConstraints"]);
    elseif constType == "border" then
        arg1 = "center"
        arg2 = [-1 -1];

        if size(varargin) <> [1 2] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "createConstraints", 2, 3));
        end

        arg1 = varargin(1);
        if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createConstraints", 2));
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "constant" | size(arg2, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createConstraints", 3));
            end
        end

        ret = tlist(["BorderConstraint","position", "preferredsize"], arg1, arg2);
    elseif constType == "gridbag" then
        arg1 = [0 0 0 0];
        arg2 = [0 0];
        arg3 = "none";
        arg4 = "center";
        arg5 = [0 0];
        arg6 = [-1 -1];

        if size(varargin) <> [1:6] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "createConstraints", 2, 7));
        end

        arg1 = varargin(1);
        if typeof(arg1) <> "constant" | size(arg1, "*") <> 4 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createConstraints", 2));
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "constant" | size(arg2, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createConstraints", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createConstraints", 4));
            end
        end

        if size(varargin) > 3 then
            arg4 = varargin(4);
            if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createConstraints", 5));
            end
        end

        if size(varargin) > 4 then
            arg5 = varargin(5);
            if typeof(arg5) <> "constant" | size(arg5, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createConstraints", 6));
            end
        end

        if size(varargin) > 5 then
            arg6 = varargin(6);
            if typeof(arg6) <> "constant" | size(arg6, "*") <> 2 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "createConstraints", 7));
            end
        end

        ret = tlist(["GridBagConstraints","grid","weight","fill","anchor","padding","preferredsize"], arg1, arg2, arg3, arg4, arg5, arg6);
    else
        error(999, msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "createConstraints", 1, "none/nolayout, border, grid, gridbag"));
    end
endfunction
