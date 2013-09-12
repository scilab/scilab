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
// This is the demonstration script of plot2d
// used when calling plot2d without any parameter.
//

function [] = %_plot2d()
    x=(0:0.1:2*%pi)';
    plot2d(x, [sin(x),sin(2*x),sin(3*x)], style=[-1,-2,3], rect=[0,-2,2*%pi,2], axesflag=1);
endfunction
