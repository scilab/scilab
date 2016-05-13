// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_pend3d1()

    function h = poly3d(x,y,z)
        xpoly(x,y);
        h = gce();
        h.data(:,3) = z;
    endfunction

    m = 1;  // mass
    g = 10; // gravity
    l = 1;  // length

    theta0 = 0.2;
    phi0   = %pi/4;

    x0=l*[sin(phi0)*cos(theta0);cos(phi0)*cos(theta0);sin(theta0)];


    function c=ggpp(x,u,lambda)
        c = 2*(u'*u+x'*(-2*lambda/m*x-[0;0;g]));
    endfunction

    // initial compatible point
    u0      = [1;-1;0];
    function  r=ff(l),r=ggpp(x0,u0,l);endfunction
    lambda0 = fsolve(0,ff);
    ud0     = -2*lambda0/m*x0-[0;0;g];
    y0      = [x0;u0;lambda0];
    yd0     = [u0;ud0;0];
    t0      = 0;

    //Observation dates for simulation
    T       = t0:0.01:15;

    //Slver tolerances
    info = list([],0,[],[],[],0,0);
    atol = [0.0001;0.0001;0.0001;0.001;0.001;0.001;1];
    rtol = atol;

    //index 1 DAE model
    //-----------------
    function [res,ires]=index1(t,y,ydot)
        x        = y(1:3);
        u        = y(4:6);
        lambda   = y(7);
        xp       = ydot(1:3);
        up       = ydot(4:6);
        lambdap  = ydot(7);
        res(1:3) = xp-u;
        res(4:6) = up+2*lambda/m*x+[0;0;g];
        res(7)   = ggpp(x,u,lambda);
        ires     = 0;
    endfunction

    y1   = dassl([y0,yd0],t0,T,rtol,atol,index1,info);
    x1=y1(2:4,:);

    //index 1 + stabilization  DAE model
    //----------------------------------
    alfa = 20;
    Beta = 20;

    function [res,ires]=index1s(t,y,ydot)
        [res,ires]=index1(t,y,ydot)
        x         = y(1:3);
        u         = y(4:6);
        res(7)    = res(7) + Beta*Beta*gg(x)+2*alfa*ggp(x,u);
    endfunction

    function c=gg(x),c= x'*x-l*l;endfunction
    function c=ggp(x,u),c = 2*x'*u;endfunction

    y1s  = dassl([y0,yd0],t0,T,rtol,atol,index1s,info);
    x1s  = y1s(2:4,:);

    //index 2  DAE model
    function [res,ires]=index2(t,y,ydot)
        x        = y(1:3);
        u        = y(4:6);
        lambda   = y(7);
        xp       = ydot(1:3);
        up       = ydot(4:6);
        lambdap  = ydot(7);
        res(1:3) = xp-u;
        res(4:6) = up+2*lambda/m*x+[0;0;g];
        res(7)   = ggp(x,u);
        ires=0;
    endfunction

    y2 = dassl([y0,yd0],t0,T,rtol,atol,index2,info);
    x2 = y2(2:4,:);

    //disp([norm(x1(:,$),2) norm(x1s(:,$),2) norm(x2(:,$),2) ])

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("pend3d1.dem.sce");

    drawlater()
    title(_("spherical pendulum simulation"),"fontsize",3)
    isoview()
    a                 = gca();
    a.view            = "3d";
    a.box             = "on";
    a.margins         = [0.1 0 0.2 0.1];
    a.grid            =color("lightgray")*ones(1,3);
    a.rotation_angles = [35 45];
    a.axes_visible    = "on";
    a.data_bounds     = [min(x1(1,:)),min(x1(2,:)),min(x1(3,:))
    max(x1(1,:)),max(x1(2,:)),max(x1(3,:))];

    p1=poly3d(x1(1,1),x1(2,1),x1(3,1));
    p1s=poly3d(x1s(1,1),x1s(2,1),x1s(3,1));
    p1s.foreground=color("red");
    p2=poly3d(x2(1,1),x2(2,1),x2(3,1));
    p2.foreground=color("blue");
    l=legend(["index1","index1+stabilization","index2"])
    drawnow();

    step=10;
    for k=1:step:(size(x1,2)-step)
        sleep(10)
        if ~is_handle_valid(my_handle) then
            break;
        end

        if is_handle_valid(p1) then
            p1.data = [p1.data
            x1(:,k:(k+step-1))'];
        end

        if is_handle_valid(p1s) then
            p1s.data = [p1s.data
            x1s(:,k:(k+step-1))'];
        end

        if is_handle_valid(p2) then
            p2.data = [p2.data
            x2(:,k:(k+step-1))'];
        end
    end

endfunction

demo_pend3d1();
clear demo_pend3d1;


