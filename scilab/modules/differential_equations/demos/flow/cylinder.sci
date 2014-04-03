//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Part II, Slanted Cylinder
// =============================================================================

function Yd=traj(tt, Y, surface)
    [p, q, r, s, t] = surface(Y(1), Y(3))
    w           = -(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+gravity)/(1+p^2+q^2)
    Yd          = [Y(2) ; p*w ; Y(4) ; q*w ];
endfunction


function [p, q, r, s, t]=SlantedCylinder(x, y)
    a = -tan(slope/180*%pi);
    z = sqrt(1-x^2)
    p = x/z;
    q = a //pente
    r = 1/(z^3)
    s = 0
    t = 0
endfunction




// calculate_traj
// =============================================================================

function Y = calculate_traj(g_x, g_y, g_V, g_Vdir, t, gravity, slope)

    function r=cyllim(t, y),
        r=[y(1)-0.95
        y(1)+0.95
        y(3)
        y(3)-14]
    endfunction

    Y0(1) = g_x;//x
    Y0(2) = g_V*cos(g_Vdir*%pi/180);//v_x
    Y0(3) = g_y;//y
    Y0(4) = g_V*sin(g_Vdir*%pi/180);//v_y
    Y     = ode("root", Y0, t(1), t, 1d-10, 1.D-10, list(traj, SlantedCylinder), 4, cyllim);//traj d'ecoulement
endfunction

// draw_bille
// =============================================================================

function draw_bille(Y)
    g_tx = Y(1, :)';
    g_ty = Y(3, :)';
    g_tz = -sqrt(1-g_tx^2-g_ty^2);
endfunction

