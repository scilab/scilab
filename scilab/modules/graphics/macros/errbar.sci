// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=errbar(x,y,em,ep)

    // Rajoute des barres d'erreur sur un graphique 2D
    // x et y decrivent les courbes (voir plot2d)
    // em et ep sont deux matrices la barre d'erreur au point
    // <x(i,j),y(i,j)> va de <x(i,j),y(i,j)-em(i,j)> a <x(i,j),y(i,j)+em(i,j)>
    // x,y,em et ep sont donc des matrices (p,q), q courbes contenant chacunes
    // p points.
    // Exemple : taper errbar()
    //      x=0:0.1:2*%pi;
    //   y=<sin(x);cos(x)>';x=<x;x>';plot2d(x,y);
    //   errbar(x,y,0.05*ones(x),0.03*ones(x));

    [lhs,rhs]=argn(0)

    if rhs == 0 then   // demo
        x = 0:0.1:2*%pi;
        y = [sin(x);cos(x)]';
        x = [x;x]';
        plot2d(x,y);
        errbar(x,y,0.05*ones(x),0.03*ones(x));
        return;
    else
        if rhs<4 then
            error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "errbar", 4));
        end
    end

    show_window();
    [n1,n2] = size(x);
    y1      = matrix(y-em,1,n1*n2);
    x1      = matrix(x,1,n1*n2);
    y2      = matrix(y+ep,1,n1*n2);
    xsegs([x1;x1],[y1;y2]);
    my_axe  = gca();
    my_axe.clip_state = "clipgrf";

endfunction
