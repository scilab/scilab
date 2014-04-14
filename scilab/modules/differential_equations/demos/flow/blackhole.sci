//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Part III, black hole, z=f(x, y)=-(1/sqrt(x^2+y^2))
// =============================================================================



// traj
// ====================================

function Yd=traj(tt, Y, surface)
    [p, q, r, s, t] = surface(Y(1), Y(3));
    w           = -(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2);
    Yd          = [ Y(2) ; p*w ; Y(4) ; q*w ];
endfunction


// blackhole
// ====================================

function  [p, q, r, s, t]=blackhole(x, y)

    //z=f(x, y)=-(1/sqrt(x^2+y^2));
    //
    //p=df/dx
    //q=df/dy
    //r=df/dxx
    //t=df/dyy
    //s=df/dxy

    z = -(1/sqrt(x^2+y^2));
    p = -x*z^3;
    q = -y*z^3;
    r = z^3*(3*x^2*z^2-1);
    s = 3*x*y*z^5;
    t = z^3*(3*y^2*z^2-1);

endfunction

// calculate_traj
// ====================================

function Y = calculate_traj(g_r, g_t, g_V, g_Vdir, t)

    function r=blackholelim(t, y),
        r=[y(1)-2
        y(1)+2
        y(3)-2
        y(3)+2
        -(1/sqrt(y(1)^2+y(2)^2))+13]
    endfunction


    Y0(1) = g_r*cos(g_t*%pi/180);      // x
    Y0(2) = g_V*cos(g_Vdir*%pi/180);   // v_x
    Y0(3) = g_r*sin(g_t*%pi/180);      // y
    Y0(4) = g_V*sin(g_Vdir*%pi/180);   // v_y

    Y     = ode("root", Y0, t(1), t, 1d-10, 1.D-10, list(traj, blackhole), 5, blackholelim); //traj d'ecoulement

endfunction


