// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function contour(x, y, z, nz, theta, alpha, leg, flag, ebox, zlev)

    function contour_demo()
        function z = Surf1(x, y)
            z = x.^2 + y.^3;
        endfunction

        function z = Surf2(x, y)
            z = x.^2 + y.^2;
        endfunction

        x = -1:0.1:1;
        y = x;
        [X,Y] = ndgrid(x,y);
        z = Surf2(X,Y)

        //scf();
        subplot(211);
        contour(x, y, Surf1, 10);
        subplot(212);
        plot3d(x, y, z);
        contour(x, y, z + 0.1, 10, flag = [0 2 4]);
    endfunction

    rhs = argn(2);

    // demo
    if rhs == 0 then
        contour_demo();
        return
    end

    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "contour", 4));
    end

    if type(x) <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour", 1));
    end

    if type(y) <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "contour", 2));
    end

    if type(z) <> 1 & type(z) <> 13 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix or Scilab function expected.\n"), "contour", 3));
    end

    if type(nz) <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "contour", 4));
    end

    if ~isvector(x) then
        error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour", 1));
    end

    if ~isvector(y) then
        error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "contour", 2));
    end

    levels = [];

    opts = [];
    if exists("theta", "local") == 1 then
        opts = [opts, "theta=theta"];
    end
    if exists("alpha", "local") == 1 then
        opts = [opts, "alpha=alpha"];
    end
    if exists("leg"  , "local") == 1 then
        opts = [opts, "leg=leg"];
    end
    if exists("ebox" , "local") == 1 then
        opts = [opts, "ebox=ebox"];
    end
    if exists("flag" , "local") == 0 then
        flag = [2 2 3];
    end
    if exists("zlev" , "local") == 0 then
        zlev = 0;
    end

    if or(type(z) == [11 13]) then
        fun = z;
        clear z //to avoid redfinition warning
        if type(fun) == 11 then
            comp(fun);
        end
        z = feval(x, y, fun);
    end

    job = flag(1);
    if rhs == 4 | job == 2 then
        contour2d(x, y, z, nz);
        return;
    end

    if size(nz, "*") == 1 then
        style = 1:nz;
    else
        style = 1:size(nz, "*");
    end

    opts = strcat([opts, "flag=flag(2:3)"], ",");

    [xc, yc] = contour2di(x, y, z, nz);

    fig = gcf();
    a = gca();
    fg = a.foreground;
    v = fig.immediate_drawing;
    fig.immediate_drawing = "off";
    cnt = 0;

    fpf = xget("fpf");
    if fpf == "" then
        fpf = "%.2g";
    end
    k = 1;
    n = yc(k);
    c = 0;
    level = %inf;
    while k < length(xc)
        n = yc(k)
        if xc(k) ~= level then
            c = c+1;
            level = xc(k);
            levels = [level levels];
            a.foreground = c;
            if cnt > 0 then
                glue(a.children(1:cnt));
                cnt = 0;
            end
        end

        if job == 1 then
            zz = zlev;
        else
            zz = level;
        end
        execstr("param3d(xc(k + (1:n)), yc(k + (1:n)), zz * ones(1, n), " + opts + ")");

        cnt = cnt + 1;
        if stripblanks(fpf) <> "" then
            // don't clip as in contour2d since we are in 3D here
            // and stringbox gives data in 2D
            xstring(xc(k + 1 + n / 2), yc(k + 1 + n/2), " " + msprintf(fpf, level));
            e = gce();
            e.data(3) = zz;
            e.clip_state = "off";
            cnt = cnt + 1;
        end
        k = k + n + 1;
    end
    if cnt > 0 then
        glue(a.children(1:cnt));
        cnt = 0;
    end
    set("current_entity", a);
    a.foreground = fg;
    if flag(3) == 0 then
        a.axes_visible = "off";
        a.box = "off";
    elseif flag(3) == 1 then
        a.axes_visible = "off";
        a.box = "off";
    elseif flag(3) == 2 then
        a.axes_visible = "off";
        a.box = "back_half";
    elseif flag(3) == 3 then
        a.axes_visible = "off";
        a.box = "on";
    elseif flag(3) >= 4 then
        a.axes_visible = "on";
        a.box = "on";
    end
    fig.immediate_drawing = v;
endfunction

