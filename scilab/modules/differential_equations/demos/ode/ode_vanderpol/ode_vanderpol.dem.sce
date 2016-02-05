// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

//
// Van der Pol ODE
//

function demo_ode_vanderpol()

    text = ["Van der Pol:"; ..
    "dy1/dt= y2"; ..
    "dy2/dt=5*(1-y1^2)*y2-y1"; ..
    ""; ..
    "A trajectory is plotted by clicking on the"; ..
    "  LEFT button of the mouse."; ..
    "  The trajectory is updated as you move the mouse."; ..
    "  To fix the trajectory, click again on the LEFT button."; ..
    "You can start over by clicking on the LEFT button again"; ..
    "  or stop everything by clicking on the RIGHT button." ];

    messagebox(text,"modal");


    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("ode_vanderpol.dem.sce");


    function yprim=f(t,y)
        yprim=[y(2)
        mu*(1-y(1)^2)*y(2)-y(1)]
    endfunction

    mu = 5;

    xmin = -3.5;
    xmax =  3.5;
    ymin = -9;
    ymax =  9;

    xr = xmin:0.5:xmax;
    yr = ymin:1:ymax;

    fchamp(f,1,xr,yr);
    xlabel("y(1)","fontsize",3)
    ylabel("y(2)","fontsize",3)
    a=gca();a.margins(3)=0.2
    title([_("Van der Pol vector field")
    "dy1/dt=y2"
    "dy2/dt=5*(1-y1^2)*y2-y1"],"fontsize",3)

    t0=0; dt=0.05; tmax=15;
    t=t0:dt:tmax;
    oldx0=10*xmax; oldy0=10*ymax;
    dx=0.1; dy=0.1;
    rtol=0.0001; atol=rtol;
    while (%t)
        [b,x0,y0]=xclick();
        if or(b==[2 5,-1000]) then break,end;
        if or(b==[0 3]) & x0>=xmin & x0<=xmax & y0>=ymin & y0<=ymax then
            drawlater();
            sol=ode([x0;y0],t0,t,rtol,atol,f);
            xpoly(sol(1,:)',sol(2,:)');
            p=gce();p.thickness=2;p.foreground=5;
            drawnow();
            rep=[x0,y0,-1];
            while rep(3)==-1 then
                rep=xgetmouse();
                x0=rep(1); y0=rep(2);
                if (xmin<x0 & x0<xmax & ymin<y0 & y0<ymax) & (abs(x0-oldx0)>=dx | abs(y0-oldy0)>=dy) then
                    drawlater();
                    sol=ode([x0;y0],t0,t,rtol,atol,f);
                    p.data=[sol(1,:)' sol(2,:)'];
                    drawnow();
                    oldx0=x0; oldy0=y0;
                end
            end
        end
    end

endfunction

demo_ode_vanderpol()
clear demo_ode_vanderpol;
