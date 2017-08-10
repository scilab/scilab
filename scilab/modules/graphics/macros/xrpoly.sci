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

function []=xrpoly(orig,n,r,theta)
    // permet de tracer un polygone regulier
    //
    // orig : centre du polygone
    // n    : nombre de segments
    // r    : rayon du cercle le circonscrit
    // theta : angle, 0 si omis
    // Attention utilise l'echelle d'un appel precedent de plot2d.
    //!
    [lhs,rhs]=argn(0);
    select rhs
    case 3 then b=eye(2,2);
        // Transposed in order to post-multiply
    case 4 then b=[cos(theta) sin(theta); -sin(theta) cos(theta)];
    else error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "xrpoly", 3, 4));
    end;
    //
    ang=2*%pi/n
    pt=(0:n)'*ang;xy=r*[cos(pt),sin(pt)]
    xy=ones(n+1,1)*matrix(orig,1,2)+xy*b;
    xpoly(xy(:,1),xy(:,2),"lines",1);
endfunction
