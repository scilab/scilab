// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
