//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [eq_pts_mes]=eval_pts_EF(a,b,nelem,kind,nint,nodes,x,xi,w,N,mesures)
    // Cette fonction renvoie les equations de sorties correspondent aux        //
    // points de mesures en utilisant l'interpolation en polynomme de Lagranges //
    // sortie :                                                                 //
    //    - eq_pts_mes (String) : vecteur des �quations de sorties choisit      //
    // entrees :                                                                //
    //    - a, b (Doubles) : limites du domaine [a b]                           //
    //    - nelem (Entier) : le nombre d'element.                               //
    //    - kind(i) (Entier) : ordre des fonctions de test                      //
    //    - ninit(i) (Entier) :ordre d'integration Gaussian                     //
    //    - x (Double):  vecteur des cordonn�es des points nodales              //
    //    - xi, w (Doubles) : les points Gausse et leurs poids obtenu           //
    //      de setint()                                                         //
    //    - N (Entier) : est le nombre de noeuds                                //
    //    - mesures (Double) : vecteur des abcaisses des points de mesures      //
    //--------------------------------------------------------------------------//

    u=emptystr(N,1);
    h=(b-a)/nelem;
    for i=1:N
        u(i)="x["+string(i-1)+"]";
    end

    nmes=size(mesures,"*");
    eq_pts_mes=emptystr(nmes,1);
    for npt=1:nmes
        // num�ro d'element auquel npt appartient
        nel=int((mesures(npt)-a)/h)+1;
        //dernier point de mesure = b ou a =b-%eps
        if (nel > nelem) then, nel=nelem; end
        // changement de coordon�es dans xi
        n = kind(nel) + 1;
        i1 = nodes(1,nel);
        i2 = nodes(n,nel);
        x1=x(i1); x2=x(i2);
        dx = (x2-x1)/2;
        xx=(mesures(npt)-x1)/dx-1;
        [psi,dpsi]=shape(xx,n);
        uh="";
        for l=1:n
            l1=nodes(l,nel);
            uh=addfv(uh,"("+msprintfv(psi(l))+")*("+u(l1)+")");
        end
        eq_pts_mes(npt) = uh;
    end
endfunction

