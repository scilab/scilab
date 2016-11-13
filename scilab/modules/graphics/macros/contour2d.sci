// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function levels = contour2d(x, y, z, nz, style, strf, leg, rect, nax, void)

    [lhs, rhs] = argn(0);

    if rhs == 0 then
        scf();
        levels = contour2d(1:10, 1:10, rand(10,10), 5, rect = [0, 0, 11, 11]);
        return;
    end

    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "contour2d", 4));
    end

    if type(x) <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour2d", 1));
    end

    if type(y) <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour2d", 2));
    end

    if type(z) <> 1 & type(z) <> 13 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix or Scilab function expected.\n"), "contour2d", 3));
    end

    if type(nz) <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "contour2d", 4));
    end

    if ~isvector(x) then
        error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour2d", 1));
    end

    if ~isvector(y) then
        error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour2d", 2));
    end

    opts = [];
    levels = [];

    if ~exists("style", "local") then
        if size(nz, "*") == 1 then
            style = 1:nz;
        else
            style = 1:size(nz, "*");
        end
    end

    if exists("strf", "local") then
        yflag = part(strf, 2);
        if or(yflag == ["2" "4" "6" "8"]) then
            rect = [min(x), min(y), max(x), max(y)];
            yflag = string(evstr(yflag) - 1);
            strf = part(strf, 1) + yflag + part(strf, 3);
        end
        opts = [opts,"strf=strf"];
    end

    if exists("leg", "local") then
        opts = [opts, "leg=leg"];
    end
    rectSpecified = %f;
    if exists("rect","local") then
        opts = [opts, "rect=rect"];
        rectSpecified = %t;
    end
    if exists("nax","local") then
        opts = [opts, "nax=nax"];
    end
    if exists("logflag","local") then
        opts = [opts, "logflag=logflag"];
    end
    if exists("frameflag","local") then
        opts = [opts,"frameflag=frameflag"];
    else
        frameflag = [];
    end
    if exists("axesflag","local") then
        opts = [opts, "axesflag=axesflag"];
    end
    opts = strcat([opts, "style=style(c)"], ",");

    if or(type(z) == [11 13]) then
        fun = z;
        //to avoid redfinition warning
        clear z
        if type(fun) == 11 then
            comp(fun);
        end
        z = feval(x, y, fun);
    end

    [xc, yc] = contour2di(x, y, z, nz);
    fpf = xget("fpf");
    if fpf == "" then
        fpf = "%.3g";
    end

    fig = gcf();
    a = gca();
    autoc = a.auto_clear;
    if autoc == "on" then
        if ~isempty(a.children) then
            delete(a.children);
        end
    end
    v = fig.immediate_drawing;
    fig.immediate_drawing = "off";
    a.auto_clear = "off";
    cnt = 0;


    // we draw the contour with call to plot2d for each level line
    // however the data_bounds will be always reset after each plot
    // so we must compute before the bounding rectangle (xmin,ymin,xmax,ymax)
    // and give it to each plot2d. This is useful for frameflag = (2,4,6,8)
    // since the data_bounds are given by the extrema of x and y
    if ( frameflag == 2 | frameflag == 4 | frameflag == 6 | frameflag == 8 )
        // get the bounding rectangle
        rect = [min(x), min(y), max(x), max(y)];
        // set the option accordingly
        if ~rectSpecified then
            opts = strcat([opts, "rect=rect"], ",");
        end
        // the rect will be taken into account
        frameflag = frameflag - 1 ;
    elseif (~rectSpecified) then
        // get rect any way for clipping
        rect = [min(x), min(y), max(x), max(y)];
    end

    k = 1;
    n = yc(k);
    c = 0;
    level = %inf;
    while k < length(xc)
        n = yc(k);
        if xc(k) ~= level then
            c = c + 1;
            level = xc(k);
            levels = [level levels];
            if cnt>0 then
                glue(a.children(1:cnt));
                cnt = 0;
            end
        end
        err = execstr("plot2d(xc(k + (1:n)), yc(k + (1:n))," + opts + ")", "errcatch", "m");
        frameflag = 0;

        // add a test to see if plot2d call succeed
        // and, if not, restore good figure property values before exiting
        if err <> 0
            mprintf(gettext("Error %d : in plot2d called by contour2d"), err);
            fig.immediate_drawing = v;
            a.auto_clear = autoc;
            return;
        end

        unglue(a.children(1))
        cnt = cnt + 1;

        if stripblanks(fpf) <> "" then
            labelText = " " + msprintf(fpf, level);
            labelPos = [xc(k + 1 + n/2), yc(k + 1 + n/2)];
            labelBox = stringbox(labelText, labelPos(1), labelPos(2));
            // check that the text is not outside the box
            // better than clipping to avoid half cut strings
            if labelBox(1,1) > rect(1) & labelBox(2,1) > rect(2) & ..
                labelBox(1,3) < rect(3) & labelBox(2,3) < rect(4) then
                xstring(labelPos(1), labelPos(2), labelText)
                e = gce();
                e.clip_state = "off";
                cnt = cnt + 1;
            end
        end
        k = k + n + 1;
    end

    if cnt > 0 then
        glue(a.children(1:cnt));
        cnt = 0;
    end
    set("current_entity", a);
    fig.immediate_drawing = v;
endfunction
