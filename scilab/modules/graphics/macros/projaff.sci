// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1991 - INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=projaff(x,y,pt)
// projaff - projection d'un point sur une droite
//%Syntaxe
// d=projaff(x,y,pt)
//%Parametres
// x: vecteur des abscisses des 2 points definissant la droite
// y: vecteur des ordonnes des 2 points definissant la droite
// pt: coordonnes du point a projeter [abscisses ordonnees]
//%Methode
//l'equation de la droite est :
//(y2-y1)*(x-x1)-(x2-x1)*(y-y1)=0
//celle de la normale passant par le point:
//(x2-x1)*(x-xp)+(y2-y1)*(y-yp)=0
//
//!

if argn(2)<>3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "projaff", 3));
end

dx=x(2)-x(1);dy=y(2)-y(1)
m=[dy -dx;dx dy]
d=m\ [-x(2)*y(1)+x(1)*y(2);m(2,:)*[pt(1);pt(2)]]
endfunction
