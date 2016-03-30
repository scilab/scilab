// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1991 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [m,x]=secto3d(seclist,npas)
    //Etant donnee une surface 3D decrite par une liste (seclist) de sections
    // dans le plan  (x,z),  [m [,x]]=secto3d(seclist [,npas]) retourne une
    // matrice m  contenant une discretisation reguliere de la surface:
    //   la i-ieme ligne de la matrice m correspond a la i-ieme section,
    //   la j-ieme colonne de m  correspondant a l'abscisse x(j).
    //Chaque section seclist(i) est decrite par une matrice a 2 lignes,
    // correspondants respectivement aux coordonnees selon l'axe des x et des z.
    //%methode
    // Il est tout d'abord calcule les bornes extremales xmin,xmax sur
    // l'axe des x pour l'ensemble des sections, l'intervalle [xmin,xmax]
    // est ensuite decoupe en npas egaux, chaque section est ensuite interpolee
    // lineairement en chaque point de cette discretisation.
    //
    // si npas est omis il est par defaut pris egal au nombre de sections.
    //%autre syntaxe
    // [m]=secto3d(seclist ,x) dans ce cas le vecteur x sera pris comme decoupage
    // de l'axe des x pour l'ensemble des sections
    //%voir aussi
    // la macro plot3d
    //!


    [lhs,rhs]=argn(0)

    if and(argn(2)<>[1 2]) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "secto3d", 1, 2));
    end

    n=size(seclist)
    //
    if rhs<2 then npas=n,end
    if prod(size(npas))==1 then
        sec=seclist(1);sec=sec(1,:)
        xmax=max(sec);xmin=min(sec)
        for k=2:n
            sec=seclist(k);sec=sec(1,:)
            xmax=max(xmax,max(sec))
            xmin=min(xmin,min(sec))
        end
        pas=(xmax-xmin)/npas
        x=[xmin-pas xmin:pas:xmax xmax+pas]
    else
        x=npas
    end
    nx=prod(size(x))
    m(n,nx)=0;
    for k=1:n
        sec=seclist(k)
        [ms,ns]=size(sec)
        sec=[[x(1);sec(2,1)],sec,[x(nx);sec(2,ns)]]
        m(k,:)=interpln(sec,x);
    end
endfunction
