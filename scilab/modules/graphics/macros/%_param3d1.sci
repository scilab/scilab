// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

// This is the demonstration script of param3d1
// used when calling param3d1 without any parameter.

function [] = %_param3d1()
    [X, Y] = ndgrid(-11:0.5:9);
    R = sqrt(X.*X + Y.*Y) + %eps;
    Z = sin(R)./R;
    
    clf
    drawlater
    subplot(1,3,1)
    param3d1(X, Y, Z, 150, 85, flag=[2,4])
    
    subplot(1,3,2)
    param3d1(X, Y, Z, 150, 85, flag=[2,4])
    gce().children.foreground = color("green");
    
    subplot(1,3,3)
    param3d1(X, Y, Z, 150, 85, flag=[2,4])
    curves = gce().children;
    curves(1:2:$).foreground = color("orange");
    gca().box = "back_half";
    
    gcf().children.foreground = color("grey70"); // box & ticks color for the 3 figures
    gcf().children.sub_ticks = [4 4 4];
    gcf().axes_size = [1000 370];
    drawnow
endfunction
