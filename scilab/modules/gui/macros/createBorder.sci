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

function ret = createBorder(constType, varargin)
    [lhs,rhs]=argn(0)

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected."), "createBorder", 1));
    end

    ret = [];
    if constType == "none" then
        ret = tlist(["NoBorder"]);
    elseif constType == "line" then
        if rhs <> [2:4] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "createBorder", 2, 4));
        end

        //color
        arg1 = varargin(1);
        if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 2));
        end

        if rhs == 2 then
            ret = tlist(["LineBorder", "color"], arg1);
            return;
        end

        //thickness
        arg2 = varargin(2);
        if typeof(arg2) <> "constant" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 3));
        end

        if rhs == 3 then
            ret = tlist(["LineBorder", "color", "thickness"], arg1, arg2);
            return;
        end

        //rounded
        arg3 = varargin(3);
        t = typeof(arg3);
        if (t <> "boolean" & t <> "string") | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"), "createBorder", 4));
        end

        if t == "string" then
            if arg3 == "on" then
                arg3 = %t;
            else
                arg3 = %f;
            end
        end

        ret = tlist(["LineBorder", "color", "thickness", "rounded"], arg1, arg2, arg3);
    elseif constType == "bevel" then
        if rhs <> [2 4 6] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d, %d or %d expected."), "createBorder", 2, 4, 6));
        end

        //type
        arg1 = varargin(1);
        if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 2));
        end

        if rhs == 2 then
            ret = tlist(["BevelBorder", "type"], arg1);
            return;
        end

        //hl or hl out
        arg2 = varargin(2);
        if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 3));
        end

        //shadow or hl in
        arg3 = varargin(3);
        if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 4));
        end

        if rhs == 4 then
            ret = tlist(["BevelBorder", "type", "hlouter", "shadowouter"], arg1, arg2, arg3);
            return;
        end

        //shadow out
        arg4 = varargin(4);
        if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 5));
        end

        //shadow in
        arg5 = varargin(5);
        if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 6));
        end

        ret = tlist(["BevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"], arg1, arg2, arg3, arg4, arg5);
    elseif constType == "softbevel" then
        if rhs <> [2 4 6] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d, %d or %d expected."), "createBorder", 2, 4, 6));
        end

        //type
        arg1 = varargin(1);
        if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 2));
        end

        if rhs == 2 then
            ret = tlist(["SoftBevelBorder", "type"], arg1);
            return;
        end

        //hl or hl out
        arg2 = varargin(2);
        if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 3));
        end

        //shadow or hl in
        arg3 = varargin(3);
        if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 4));
        end

        if rhs == 4 then
            ret = tlist(["SoftBevelBorder", "type", "hlouter", "shadowouter"], arg1, arg2, arg3);
            return;
        end

        //shadow out
        arg4 = varargin(4);
        if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 5));
        end

        //shadow in
        arg5 = varargin(5);
        if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 6));
        end

        ret = tlist(["SoftBevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"], arg1, arg2, arg3, arg4, arg5);
    elseif constType == "etched" then

        if rhs > 4 then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "createBorder", 1, 4));
        end

        if rhs == 1 then
            ret = tlist(["EtchedBorder"]);
            return;
        end

        //type or color
        arg1 = varargin(1);
        if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer or a string expected.\n"), "createBorder", 2));
        end

        if rhs == 2 then
            ret = tlist(["EtchedBorder", "type"], arg1);
            return;
        end

        //color
        arg2 = varargin(2);
        if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 3));
        end

        if rhs == 3 then
            ret = tlist(["EtchedBorder", "hl", "shadow"], arg1, arg2);
            return;
        end

        //color
        arg3 = varargin(3);
        if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 4));
        end

        ret = tlist(["EtchedBorder", "type", "hl", "shadow"], arg1, arg2, arg3);
    elseif constType == "titled" then
        if rhs <> [2:7] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "createBorder", 2, 7));
        end

        //string or border
        arg1 = varargin(1);
        if typeof(arg1) <> ["string", "LineBorder","BevelBorder","SoftBevelBorder","EtchedBorder","EmptyBorder","MatteBorder"] then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A border or a string expected.\n"), "createBorder", 2));
        end

        if rhs == 2 then
            if typeof(arg1) == "string" then
                ret = tlist(["TitledBorder", "title"], arg1);
            else
                ret = tlist(["TitledBorder", "border"], arg1);
            end
            return;
        end

        //title
        arg2 = varargin(2);
        if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 3));
        end

        if rhs == 3 then
            ret = tlist(["TitledBorder", "border", "title"], arg1, arg2);
            return;
        end

        //justification
        arg3 = varargin(3);
        if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 4));
        end

        //position
        arg4 = varargin(4);
        if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 5));
        end

        if rhs == 5 then
            ret = tlist(["TitledBorder", "border", "title", "justification", "position"], arg1, arg2, arg3, arg4);
            return;
        end

        //font
        arg5 = varargin(5);
        if typeof(arg5) <> "BorderFont" then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A Font Border expected.\n"), "createBorder", 6));
        end

        if rhs == 6 then
            ret = tlist(["TitledBorder", "border", "title", "justification", "position", "font"], ...
            arg1, arg2, arg3, arg4, arg5);
            return;
        end

        arg6 = varargin(6);
        if typeof(arg6) <> "string" | size(arg6, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 6));
        end

        ret = tlist(["TitledBorder", "border", "title", "justification", "position", "font", "color"], ...
        arg1, arg2, arg3, arg4, arg5, arg6);
    elseif constType == "empty" then
        if rhs <> [1 5] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected."), "createBorder", 1, 5));
        end

        if rhs == 1 then
            ret = tlist(["EmptyBorder"]);
            return;
        end

        arg1 = varargin(1);
        if typeof(arg1) <> "constant" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 2));
        end

        arg2 = varargin(2);
        if typeof(arg2) <> "constant" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 3));
        end

        arg3 = varargin(3);
        if typeof(arg3) <> "constant" | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 4));
        end

        arg4 = varargin(4);
        if typeof(arg4) <> "constant" | size(arg4, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 5));
        end

        ret = tlist(["EmptyBorder", "top", "left", "bottom", "right"], arg1, arg2, arg3, arg4);
    elseif constType == "compound" then

        if rhs <> [1 3] then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected."), "createBorder", 1, 3));
        end

        if rhs == 1 then
            ret = tlist(["CompoundBorder"]);
            return;
        end

        arg1 = varargin(1);
        if typeof(arg1) <> ["LineBorder","BevelBorder","SoftBevelBorder","EtchedBorder","EmptyBorder","MatteBorder","TitledBorder"] then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A border expected.\n"), "createBorder", 2));
        end

        arg2 = varargin(2);
        if typeof(arg2) <> ["LineBorder","BevelBorder","SoftBevelBorder","EtchedBorder","EmptyBorder","MatteBorder","TitledBorder"] then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A border expected.\n"), "createBorder", 3));
        end

        ret = tlist(["CompoundBorder", "outer", "inner"], arg1, arg2);
    elseif constType == "matte" then

        if rhs <> 6 then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected."), "createBorder", 6));
        end

        //top
        arg1 = varargin(1);
        if typeof(arg1) <> "constant" | size(arg1, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 2));
        end

        //left
        arg2 = varargin(2);
        if typeof(arg2) <> "constant" | size(arg2, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 3));
        end

        //bottom
        arg3 = varargin(3);
        if typeof(arg3) <> "constant" | size(arg3, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 4));
        end

        //right
        arg4 = varargin(4);
        if typeof(arg4) <> "constant" | size(arg4, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorder", 5));
        end

        //color
        arg5 = varargin(5);
        if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "createBorder", 6));
        end

        ret = tlist(["MatteBorder", "top", "left", "bottom", "right", "color"], arg1, arg2, arg3, arg4, arg5);
    else
        error(999, msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "createBorder", 1, "line, bevel, softbevel, etched, titled, empty, compound, matte"));
    end
endfunction
