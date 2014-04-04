//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
