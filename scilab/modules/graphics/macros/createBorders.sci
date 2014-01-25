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

function ret = createBorders(constType, varargin)
    [lhs,rhs]=argn(0)

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected."), "createBorders", 1));
    end

    ret = [];
    if constType == "none" then
        ret = tlist(["NoBorder"]);
    elseif constType == "line" then
        arg1 = "black"; //color
        arg2 = 1; //thickness
        arg3 = %f; //rounded

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "constant" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            t = typeof(arg3);
            if (t <> "boolean" & t <> "string") | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"), "createBorders", 4));
            end

            if t == "string" then
                if arg3 == "on" then
                    arg3 = %t;
                else
                    arg3 = %f;
                end
            end
        end

        ret = tlist(["LineBorder", "color", "thickness", "rounded"], arg1, arg2, arg3);
    elseif constType == "bevel" then
        arg1 = "raised"; //type
        arg2 = "black"; //hl out color
        arg3 = "black"; //hl in color
        arg4 = "black"; //shadow out color
        arg5 = "black"; //shadow in color

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 4));
            end
        end

        if size(varargin) > 3 then
            arg4 = varargin(4);
            if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 5));
            end
        end

        if size(varargin) > 4 then
            arg5 = varargin(5);
            if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 6));
            end
        end

        ret = tlist(["BevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"], arg1, arg2, arg3, arg4, arg5);
    elseif constType == "softbevel" then
        arg1 = "raised"; //type
        arg2 = "black"; //hl out color
        arg3 = "black"; //hl in color
        arg4 = "black"; //shadow out color
        arg5 = "black"; //shadow in color

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 4));
            end
        end

        if size(varargin) > 3 then
            arg4 = varargin(4);
            if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 5));
            end
        end

        if size(varargin) > 4 then
            arg5 = varargin(5);
            if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 6));
            end
        end

        ret = tlist(["SoftBevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"], arg1, arg2, arg3, arg4, arg5);
    elseif constType == "etched" then
        arg1 = "raised"; //type
        arg2 = "black"; //hl color
        arg3 = "black"; //shadow color

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "string" | size(arg1, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 4));
            end
        end

        ret = tlist(["EtchedBorder", "type", "hl", "shadow"], arg1, arg2, arg3);
    elseif constType == "titled" then
        arg1 = createBorders("etched"); //border
        arg2 = "Title"; //title
        arg3 = "leading"; //justification
        arg4 = "helvetica"; //fontname
        arg5 = "normal"; //font angle
        arg6 = 10; //font size
        arg7 = "normal"; //font weight
        arg8 = "top"; //position
        arg9 = "black"; //color

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> ["LineBorder","BevelBorder","SoftBevelBorder","EtchedBorder","EmptyBorder","MatteBorder"] then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A border expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "string" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "string" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 4));
            end
        end

        if size(varargin) > 3 then
            arg4 = varargin(4);
            if typeof(arg4) <> "string" | size(arg4, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 5));
            end
        end

        if size(varargin) > 4 then
            arg5 = varargin(5);
            if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 6));
            end
        end

        if size(varargin) > 5 then
            arg6 = varargin(6);
            if typeof(arg6) <> "constant" | size(arg6, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 7));
            end
        end

        if size(varargin) > 6 then
            arg7 = varargin(7);
            if typeof(arg7) <> "string" | size(arg7, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 8));
            end
        end

        if size(varargin) > 7 then
            arg8 = varargin(8);
            if typeof(arg8) <> "string" | size(arg8, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 9));
            end
        end

        if size(varargin) > 8 then
            arg9 = varargin(9);
            if typeof(arg9) <> "string" | size(arg9, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 10));
            end
        end

        ret = tlist(["TitledBorder", "border", "title", "justification", "fontname", "fontangle", "fontsize", "fontweight", "position", "color"], ...
            arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    elseif constType == "empty" then
        arg1 = 0; //top
        arg2 = 0; //left
        arg3 = 0; //bottom
        arg4 = 0; //right

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "constant" | size(arg1, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "constant" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "constant" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 4));
            end
        end

        if size(varargin) > 3 then
            arg4 = varargin(4);
            if typeof(arg4) <> "constant" | size(arg4, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 5));
            end
        end

        ret = tlist(["EmptyBorder", "top", "left", "bottom", "right"], arg1, arg2, arg3, arg4);
    elseif constType == "compound" then
        arg1 = createBorders("etched"); //out border
        arg2 = createBorders("etched"); //in border

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> ["LineBorder","BevelBorder","SoftBevelBorder","EtchedBorder","EmptyBorder","MatteBorder","TitledBorder"] then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A border expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> ["LineBorder","BevelBorder","SoftBevelBorder","EtchedBorder","EmptyBorder","MatteBorder","TitledBorder"] then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A border expected.\n"), "createBorders", 3));
            end
        end

        ret = tlist(["CompoundBorder", "outer", "inner"], arg1, arg2);
    elseif constType == "matte" then
        arg1 = 0; //top
        arg2 = 0; //left
        arg3 = 0; //bottom
        arg4 = 0; //right
        arg5 = "black"; //color

        if size(varargin) > 0 then
            arg1 = varargin(1);
            if typeof(arg1) <> "constant" | size(arg1, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 2));
            end
        end

        if size(varargin) > 1 then
            arg2 = varargin(2);
            if typeof(arg2) <> "constant" | size(arg2, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 3));
            end
        end

        if size(varargin) > 2 then
            arg3 = varargin(3);
            if typeof(arg3) <> "constant" | size(arg3, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 4));
            end
        end

        if size(varargin) > 3 then
            arg4 = varargin(4);
            if typeof(arg4) <> "constant" | size(arg4, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A integer expected.\n"), "createBorders", 5));
            end
        end

        if size(varargin) > 4 then
            arg5 = varargin(5);
            if typeof(arg5) <> "string" | size(arg5, "*") <> 1 then
                error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "createBorders", 6));
            end
        end

        ret = tlist(["MatteBorder", "top", "left", "bottom", "right", "color"], arg1, arg2, arg3, arg4, arg5);
    else
        error(999, msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "createBorders", 1, "line, bevel, softbevel, etched, titled, empty, compound, matte"));
    end
endfunction
