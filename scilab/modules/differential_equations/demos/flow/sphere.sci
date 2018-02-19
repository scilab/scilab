//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.
//


// traj
// =============================================================================

function Yd=traj(tt, Y, surface)
    g=9.81
    [p, q, r, s, t]=surface(Y(1), Y(3))
    w=-(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+g)/(1+p^2+q^2)
    Yd=[Y(2)
    p*w
    Y(4)
    q*w]
    if size(Yd, 1)<>4|~isreal(Yd) then pause, end
endfunction


// sphere
// =============================================================================

function  [p, q, r, s, t]=sphere(x, y)
    //z=f(x, y)=sqrt(1-x^2-y^2)
    //
    //df/dx=-x/sqrt(1-x^2-y^2)
    //df/dy=-y/sqrt(1-x^2-y^2)
    //df/dxx=(y^2-1)/(1-x^2-y^2)^(3/2)
    //df/dyy=(y^2-1)/(1-x^2-y^2)^(3/2)
    //df/dxy=(-xy)/(1-x^2-y^2)^(3/2)

    z = -sqrt(1-(x^2+y^2));
    w = z^3
    p = -x/z;
    q = -y/z;
    r = (y^2-1)/w;
    s = (-x*y)/w;
    t = (x^2-1)/w;

endfunction

// calculate_traj
// =============================================================================

function Y = calculate_traj(g_r, g_t, g_V, g_Vdir, t)
    function r=sphlim(t, y), r=y(1)^2+y(3)^2-0.9999, endfunction
    Y0(1) = g_r*cos(g_t*%pi/180);     // x
    Y0(2) = g_V*cos(g_Vdir*%pi/180);  // v_x
    Y0(3) = g_r*sin(g_t*%pi/180);     // y
    Y0(4) = g_V*sin(g_Vdir*%pi/180);  // v_z
    Y     = ode("root", Y0, t(1), t, 1d-10, 1.D-10, list(traj, sphere), 1, sphlim);//traj d'ecoulement
endfunction



