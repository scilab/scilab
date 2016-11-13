//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Claude Gomez <claude.gomez@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

funcprot(0);

function demo_sliding_pendulum()

    //
    // Sliding pendulum

    //  demo_help  demo_sliding_pendulum

    //pendulum parameters
    g = 10;
    l = 1;
    m = 1;
    M = 1;
    k = 0.5;

    //initial point
    global omega;
    omega = 3.3;
    x0    = [1;1+cos(omega)/3;0];
    u0    = [0;0;0];
    y0    = [x0;u0;0];
    ud0   = [0;-g;0];
    yd0   = [u0;ud0;0];

    //set observation times
    t0=0;
    T=t0:0.05:20;

    //dassl parameters
    info = list([],0,[],[],[],0,0);
    atol = [0.0001;0.0001;0.0001;0.0001;0.0001;0.0001;0.001];
    rtol = atol;

    // dae integration
    sol=dassl([y0,yd0],t0,T,rtol,atol,pendg,info);

    //draw pendulum at its initial position
    H = build_sliding_pendulum ()
    draw_sliding_pendulum(H,y0(1:3))

    //visualization of the result
    realtimeinit(0.05);realtime(0)
    for i=1:size(sol,2)
        realtime(i)
        if is_handle_valid(H) then
            draw_sliding_pendulum(H,sol(2:4,i))
        else
            break;
        end
    end

endfunction


function [res,ires]=pendg(t,y,ydot)

    //DAE external for sliding pendulum

    x=y(1:3);
    u=y(4:6);
    lambda=y(7);
    xp=ydot(1:3);
    up=ydot(4:6);

    res=[xp-u;
    (M+m)*up(1)     + m*l*(cos(x(3))*up(3)-sin(x(3))*u(3)^2) + lambda*fx(x(1),x(2)) + k*u(1);
    (M+m)*(up(2)+g) + m*l*(sin(x(3))*up(3)+cos(x(3))*u(3)^2) + lambda*fy(x(1),x(2)) + k*u(2);
    m*l*(cos(x(3))*up(1)+sin(x(3))*up(2))  + m*l^2*up(3) + m*g*sin(x(3));
    -(fx(x(1),x(2))*u(1)+fy(x(1),x(2))*u(2))];
    ires=0;
endfunction

function res=fx(x,y)
    global omega;
    res=-2*x+omega*sin(omega*x)/3;
endfunction

function res=fy(x,y)
    res=1;
endfunction


function  H=build_sliding_pendulum ()

    global omega;

    //build the sliding pendulum figure and graphic objects,
    //return the handle on moving parts

    f = scf(100001);
    clf(f,"reset");

    a=gca();
    drawlater()
    f.axes_size = [610,676];
    xmin=-1.5;
    xmax=1.5;
    ymin=-1.1;
    ymax=2.35
    a.data_bounds=[xmin ymin;xmax ymax]
    isoview();
    //the framework
    xrect(xmin,ymax,xmax-xmin,ymax-ymin)
    //the curve
    vx=[xmin:0.01:xmax]'; vy=vx.*vx+cos(omega*vx)/3;
    xpoly(vx,vy,"lines")
    c=gce();c.foreground=5;
    //the pendulum segment
    x=0; y=0; teta=0;
    xp=x+l*sin(teta); yp=y-l*cos(teta);
    r=0.05 // the bullet half diameter
    xp1=x+(l-r)*sin(teta); yp1=y-(l-r)*cos(teta);
    xpoly([x;xp1],[y;yp1],"lines")
    p=gce();p.thickness=2;
    //the pendulum bullet
    xfarc(xp-r,yp+r,2*r,2*r,0,360*64)
    b=gce()
    H=glue([p,b]) //return the handle on segment and bullet
endfunction



function  draw_sliding_pendulum (H,state)
    //draw the pendulum in a given state
    x = state(1);
    y = state(2);
    teta = state(3);

    // bullet half diameter
    b = H.children(1);
    r = b.data(3)/2;

    xp=x+l*sin(teta); yp=y-l*cos(teta);
    xp1=x+(l-r)*sin(teta); yp1=y-(l-r)*cos(teta);
    drawlater()
    p = H.children(2);p.data=[x, y; xp1, yp1];
    b = H.children(1); b.data=[xp-r,yp+r,2*r,2*r,0,360*64];
    drawnow();
    a=gca();
    a.title.text="sliding pendulum, ""upside down camel""";
    a.title.font_size=3;
endfunction

funcprot(1);
