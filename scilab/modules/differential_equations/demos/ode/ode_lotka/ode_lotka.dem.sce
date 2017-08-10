// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

//
// sharks and sardins: Lotka-Volterra ODE
//

function demo_ode_lotka()

    text = ["Lotka-Volterra:"; ..
    "dy1/dt=  3*y1 - 2*y1*y2"; ..
    "dy2/dt= -2*y2 +   y1*y2"; ..
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
    demo_viewCode("ode_lotka.dem.sce");


    function yprim=f(t,y)
        yprim=[aa*y(1)-cc*y(1)*y(2)
        -bb*y(2)+ff*y(1)*y(2)]
    endfunction


    aa = 3;
    bb = 2;
    cc = 2;
    ff = 1;

    xmin = 0;
    xmax = 9.5;
    ymin = 0;
    ymax = 5.5;

    fx = xmin:0.5:xmax;
    fy = ymin:0.5:ymax;

    fchamp(f,1,fx,fy);
    xlabel("y(1)","fontsize",3)
    ylabel("y(2)","fontsize",3)
    a=gca();
    a.margins(3)=0.2;
    title([_("Lotka-Volterra vector field")
    "dy1/dt=  3*y1 - 2*y1*y2";
    "dy2/dt= -2*y2 +   y1*y2"],"fontsize",3)


    t0       = 0;
    tmax     = 10;
    t        = t0:0.05:tmax;
    oldx0    = 10*xmax;
    oldy0    = 10*ymax;
    dx       = 0.1;
    dy       = 0.1;
    rtol     = 0.0001;
    atol     = rtol;

    while (%t)
        [b,x0,y0]=xclick();
        if or(b==[2 5 -1000]) then break end;
        if or(b==[0 3]) & xmin<x0 & x0<xmax & ymin<y0 & y0<ymax then
            sol=ode([x0;y0],t0,t,rtol,atol,f);
            xpoly(sol(1,:)',sol(2,:)');
            p=gce();
            p.thickness=2;
            p.foreground=5;
            rep=[x0,y0,-1];
            while rep(3)==-1 then
                rep=xgetmouse();
                x0=rep(1);
                y0=rep(2);
                if (xmin<x0 & x0<xmax & ymin<y0 & y0<ymax) & (abs(x0-oldx0)>=dx | abs(y0-oldy0)>=dy) then
                    sol=ode([x0;y0],t0,t,rtol,atol,f);
                    p.data=[sol(1,:)' sol(2,:)'];
                    oldx0=x0;
                    oldy0=y0;
                end
            end
        end
    end

endfunction

demo_ode_lotka()
clear demo_ode_lotka;
