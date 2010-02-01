// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
