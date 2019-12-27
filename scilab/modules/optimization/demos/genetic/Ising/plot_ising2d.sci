// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function plot_ising2d(Var)
    drawlater;
    a = gca();
    Size = size(Var,1);
    a.data_bounds = [0.5,0.5;Size+0.5,Size+0.5];
    for i=1:Size
        for j=1:Size
            if (Var(i,j)==1) then
                plot(i,j,"go");
            else
                plot(i,j,"r+");
            end
        end
    end
    xtitle("Ising 2D model");
    drawnow;
endfunction
