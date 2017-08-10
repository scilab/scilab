// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1990 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x,y,z]=eval3dp(fun,p1,p2)
    // eval3dp - retourne une representation par facettes d'une fonction z=f(u,v)
    //%Syntaxe
    //  [x,y,z]=eval3dp(fun,p1,p2)
    //%Parametres
    //  fun    : macro (de syntaxe : [x,y,z]=fun(p1,p2) ) definissant la
    //           fonction f .
    //           Attention lors de l'appel de fun p1 et p2 sont des vecteurs
    //           et la macro doit retourner x,y,z tels que :
    //           [x(i),y(i),z(i)]=f(p1(i),p2(i))
    //
    //  p1     : vecteur reel donnant la discretisation des valeurs du
    //           parametre u
    //  p2     : vecteur reel donnant la discretisation des valeurs du
    //           parametre v
    //  x      : matrice 4xn dont chaque colonne contient les abscisses
    //           d'une facette
    //  y      : matrice 4xn dont chaque colonne contient les ordonnees
    //           d'une facette
    //  z      : matrice 4xn dont chaque colonne contient les cotes
    //           d'une facette
    //%Exemple
    //  deff('[x,y,z]=scp(p1,p2)',['x=p1.*sin(p1).*cos(p2)';
    //                             'y=p1.*cos(p1).*cos(p2)';
    //                             'z=p1.*sin(p2)'])
    //  [x,y,z]=eval3dp(scp,0:0.3:2*%pi,-%pi:0.3:%pi);
    //  fac3d(x,y,z,35,45,'x@y@z')
    //%Voir aussi
    // plot3d fac3d
    //!

    [lhs,rhs]=argn(0)
    if rhs<3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "eval3dp", 3));
    end

    n1=prod(size(p1))
    n2=prod(size(p2))
    //on calcule la valeur de la fonction en tous le couples (p1(i),p2(j))
    [vx,vy,vz]=fun(ones(1,n2).*.matrix(p1,1,n1),matrix(p2,1,n2).*.ones(1,n1))
    p1=[];p2=[];

    //on genere les facettes
    ind=ones(1,n1-1).*.[0 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1];
    // ind=[1,2,n1+2,n1+1 , 2,3,n1+3,n1+2, ....  ,n1-1,n1,2n1,2n1-1

    ind2=ones(1,n2-1).*.ind+((0:n2-2)*n1).*.ones(ind);

    nx=prod(size(ind2))
    x=matrix(vx(ind2),4,nx/4);
    y=matrix(vy(ind2),4,nx/4);
    z=matrix(vz(ind2),4,nx/4);
endfunction
