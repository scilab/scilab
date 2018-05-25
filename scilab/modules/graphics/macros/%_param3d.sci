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

// This is the demonstration script of param3d
// used when calling param3d without any parameter.

function [] = %_param3d()
    t = (0:0.1:5*%pi)';
    r = (max(t)-t)/10;
    s = gca().axes_bounds; delete(gca()); xsetech(s)  // clears the current axes
    param3d1(r.*sin(t), r.*cos(t), list(t/10, -10), 28, 61,"axis X@axis Y@axis Z",[4,4])
    gce().mark_foreground = color("magenta")
    gce().line_mode = "on";

    gca().foreground = color("grey60");
    gca().sub_ticks = [4 4 4];
    xgrid(color("grey80"),[1 1 1],[8 8 8]);
endfunction
