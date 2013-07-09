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

function [eq_pts_mes]=eval_pts_df(a,h,N,mesures)
    // Cette fonction renvoie les equations de sorties correspondent aux        //
    // points de mesures en approchant leus solution au noeud le plus poche     //
    // sortie :                                                                 //
    //    - eq_pts_mes (String) : vecteur des �quations de sorties choisit      //
    // entrees :                                                                //
    //    - a (Double) : limite inf�rieur du domaine [a b]                      //
    //    - h (Double) : est le pas de discretisation h=(b-a)/N (i.e x(i)= i* h)//
    //    - N (Entier) : est le nombre de noeuds                                //
    //    - mesures (Double) : vecteur des abcaisses des points de mesures      //
    //--------------------------------------------------------------------------//

    u=emptystr(N,1);;
    for i=1:N
        u(i)="x["+string(i-1)+"]";
    end
    nmes=size(mesures,"*");
    eq_pts_mes=emptystr(nmes,1);
    for npt=1:nmes
        // num�ro d'element auquel npt appartient
        nel=int((mesures(npt)-a)/h)+1;
        if (mesures(npt)-(a+h*(nel-1)) <= (a+h*nel)-mesures(npt)) then
            l=nel;
        else
            l=nel+1;
        end
        eq_pts_mes(npt) = u(l);
    end

endfunction

