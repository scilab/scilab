// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// ODE definition

function demo_ode_lorenz()

    function ydot=lorenz(t,y)
        x    = y(1);
        a    = [-10,10,0;28,-1,-x;0,x,-8/3];
        ydot = a*y
    endfunction

    function j=jacobian(t,y)
        x  = y(1);
        yy = y(2);
        z  = y(3);
        j  = [-10,10,0;28-z,-1,-x;-yy,x,-8/3]
    endfunction

    // Integration

    y0       = [-3;-6;12];
    t0       = 0;
    step     = 0.01;
    t1       = 10;
    instants = t0:step:t1;
    y        = ode(y0,t0,instants,lorenz,jacobian);

    // Visualization - animation

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("ode_lorenz.dem.sce");
    title(_("Lorenz differential equation"))

    function h = poly3d(x,y,z)
        xpoly(x,y);h=gce();h.data(:,3)=z
    endfunction

    curAxe = gca();

    drawlater()
    curAxe                 = gca();
    curAxe.view            = "3d"
    curAxe.axes_visible    = "on"
    curAxe.data_bounds     = [min(y,"c")';max(y,"c")']
    curAxe.margins(3)      = 0.2;
    curAxe.title.text      = [_("Lorenz differential equation")
    "dy1/dt = -10*y1 + 10*y2"
    "dy2/dt =  28*y1 -    y2 -  y1*y3"
    "dy3/dt =          y1*y2 - 8/3*y3"
    ]
    curAxe.grid            = curAxe.hidden_axis_color*ones(1,3);
    curAxe.x_label.text    = "y1"
    curAxe.y_label.text    = "y2"
    curAxe.z_label.text    = "y3"

    //the trace
    p = poly3d(y(1,1),y(2,1),y(3,1));
    drawnow()

    //Animate
    for k=1:size(y,2)
        sleep(10)
        if ~is_handle_valid(my_handle) then
            break;
        end

        if is_handle_valid(p) then
            p.data=[p.data;
            y(1:3,k)'];
        else
            break;
        end
    end
endfunction

demo_ode_lorenz();
clear demo_ode_lorenz;
