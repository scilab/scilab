// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function colordef(varargin)
    //Set default color values to display different color schemes

    if size(varargin)<1 | size(varargin)>2 then
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n");
        error(msprintf(msg, "colordef", 1, 2));
    end
    a1 = varargin(1);
    if size(varargin) == 1 then //Only one argument
        if type(a1)<>10 then //the first argument should be a string
            msg = gettext("%s: Wrong type for input argument #%d: String array expected.\n");
            error(msprintf(msg, "colordef", 1));
        end
        f = gdf();
        a = gda();
    end

    if size(varargin) == 2 then //Two arguments
        a2 = varargin(2);
        if type(a2) <> 10 then //the second argument should be a string
            msg = gettext("%s: Wrong type for input argument #%d: String array expected.\n");
            error(msprintf(msg, "colordef", 2));
        end

        if type(a1)==9 then //First argument is an handle
            f = a1;
            a1 = a2;
            if f.type <> "Figure" then
                msg = gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n");
                error(msprintf(msg, "colordef", 1,"handle on a figure, ""new"""));
            end
            scf(f);
            a = gca();
        elseif a1=="new" then //First argument is "new"
            n = winsid();
            if n==[] then
                n = 0;
            else
                n = max(n+1);
            end
            f = scf(n);
            a = gca();
            a1 = a2;
        else  //First argument is NOT an handle and is not "new"
            msg = gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n");
            error(msprintf(msg, "colordef", 1, "handle on a figure, ""new"""));
        end
    end

    select convstr(a1)
    case "default"
        sdf();
        sda();

    case "white"
        f.color_map = [jetcolormap(64); [1 1 1]*0.8];
        f.background = 65;
        a.background = -2;
        a.foreground = -1;
        a.font_color = a.foreground;
        a.mark_foreground = a.foreground;
        a.title.font_foreground = -1;
        a.x_label.font_foreground = -1;
        a.y_label.font_foreground = -1;
        a.z_label.font_foreground = -1;

    case "black"
        if getos() == "Windows" then
            c = 0.2
        else
            c = 0.35
        end
        f.color_map  = [jetcolormap(64); [1 1 1]*c];
        f.background = 65;
        a.background = -1;
        a.foreground = -2;
        a.font_color = a.foreground;
        a.mark_foreground = a.foreground;
        a.title.font_foreground = -2;
        a.x_label.font_foreground = -2;
        a.y_label.font_foreground = -2;
        a.z_label.font_foreground = -2;

    case "none"
        f.color_map  = [hsvcolormap(64)];
        f.background = -1;
        a.background = -1;
        a.foreground = -2;
        a.font_color = a.foreground;
        a.mark_foreground = a.foreground;
        a.title.font_foreground = -2;
        a.x_label.font_foreground = -2;
        a.y_label.font_foreground = -2;
        a.z_label.font_foreground = -2;
    else
        msg = gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n");
        error(msprintf(msg, "colordef", size(varargin),"""default"", ""white"", ""black"", ""none"""));
    end
endfunction
