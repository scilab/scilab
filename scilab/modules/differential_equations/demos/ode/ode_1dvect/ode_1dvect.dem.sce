// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_ode_1dvect()

    oldln = lines();
    lines(0)
    text = ["Examples of ODE''s in 1 dimension"; ..
    "A trajectory is plotted by clicking on the"; ..
    "  LEFT button of the mouse."; ..
    "  The trajectory is updated as you move the mouse."; ..
    "  To fix the trajectory, click again on the LEFT button."; ..
    "You can start over by clicking on the LEFT button again"; ..
    "  or stop everything by clicking on the RIGHT button." ];

    messagebox(text, "modal");

    function yprim = f(t, y), yprim = y^2-t, endfunction
    function z = g(t, y), z = [y-ymin, ymax-y], endfunction

    tmin = -3;
    tmax =  5;
    ymin = -3;
    ymax =  4;

    t = tmin:1:tmax;
    y = ymin:1:ymax;

    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("ode_1dvect.dem.sce");

    nt = size(t, "*");
    ny = size(y, "*");
    fx = ones(nt, ny);
    fy = feval(t, y, f);
    champ(t, y, fx, fy);
    xlabel("t", "fontsize", 3)
    ylabel("y", "fontsize", 3)
    a = gca();
    a.margins(3) = 0.2;
    a.tight_limits = "on"
    a.data_bounds = [tmin, ymin; tmax, ymax]
    title([_("ODE 1D vector field")
    "dy/dt=  y^2 - t"], "fontsize", 3)

    oldt0 = 10*tmax;
    oldy0 = 10*ymax;
    dt                 = 0.1;
    dy                 = 0.1;

    while(%T)
        [b, t0, y0] = xclick();
        if or(b == [2 5 -1000]) then break, end;
        if or(b == [0 3])& tmin<t0 & t0<tmax & ymin<y0 & y0<ymax then
            t1 = t0:0.1:tmax;
            [sol1, rd] = ode("root", y0, t0, t1, 1.d-2, 1.d-4, f, 2, g);
            xpoly(t1(1:size(sol1, "*"))', sol1');
            p1 = gce(), p1.thickness = 2, p1.foreground = 5;

            t2 = t0:-0.1:tmin;
            [sol2, rd] = ode("root", y0, t0, t2, 1.d-2, 1.d-4, f, 2, g);
            xpoly(t2(1:size(sol2, "*"))', sol2');

            p2 = gce(), p2.thickness = 2, p2.foreground = 5;
            rep = [t0, y0, -1];
            while rep(3) == -1 then
                rep = xgetmouse();
                t0 = rep(1); y0 = rep(2);
                if (tmin<t0 & t0<tmax & ymin<y0 & y0<ymax) & (abs(t0-oldt0)> = dt | abs(y0-oldy0)> = dy) then
                    t1 = t0:0.1:tmax;
                    [sol1, rd] = ode("root", y0, t0, t1, 1.d-2, 1.d-4, f, 2, g);
                    t2 = t0:-0.1:tmin;
                    [sol2, rd] = ode("root", y0, t0, t2, 1.d-2, 1.d-4, f, 2, g);
                    p1.data = [t1(1:size(sol1, "*"))' sol1'];
                    p2.data = [t2(1:size(sol2, "*"))' sol2'];
                    oldt0 = t0; oldy0 = y0;
                end
            end
        end
    end
    lines(oldln(1))
endfunction

demo_ode_1dvect();
clear demo_ode_1dvect;
