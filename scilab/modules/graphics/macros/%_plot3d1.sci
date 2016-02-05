//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
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
//

//
// This is the demonstration script of plot3d1
// used when calling plot3d1 without any parameter.
//

function [] = %_plot3d1()
    x = %pi * [-1:0.05:1]';
    z = sin(x)*cos(x)';
    plot3d1(x, x, z, 70, 70);
    f = gcf();
    f.color_map = jetcolormap(32);
endfunction
