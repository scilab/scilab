// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function mesh(varargin)
    // Copyright INRIA
    //
    // Syntax:
    // mesh(z)
    // mesh(x,y,z)
    // mesh(...,<GlobalProperty>)
    // mesh(...,<color>,<GlobalProperty>)
    // mesh(<axes_handle>,...)
    //
    // MESH function draws white surfaces, it's based on surf function with the properties e.color_mode = color('white') and e.color_flag = 0.
    // Inputs
    // x,y,z : matrices which define the grid


    // Copy RHS arguments
    ListArg = varargin;

    drawlater();
    surf(ListArg(:));
    e = gce();
    e.color_mode = color("white");
    e.color_flag = 0;
    drawnow();
endfunction
