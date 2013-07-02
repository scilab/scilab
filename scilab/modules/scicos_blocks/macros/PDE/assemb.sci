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

function [gk,gf]=assemb(gk,gf,ek,ef,nel,n,nodes)
    //  la fonction assemb assemble la matrice de regidit� gk et le second   //
    //  membre gf en bouclant sur les nel- elements.                         //
    //   Sorties :                                                           //
    //      - gk (Double) E/S : matrice globale                              //
    //      - gf (Double) E/S : vecteur qui correspond au scond membre       //
    //   Entr�es :                                                           //
    //      - ek (Double) : matrice �l�mentaire                              //
    //      - ef (Double) : vecteur �l�mentaire du scond membre              //
    //      - nel (Entier) : le nombre d'element.                            //
    //      - n (Entier) :   nombre de points nodales dans un element,       //
    //        e.g. linear n=2; quadrtic, n=3; cubic: n=4.                    //
    //      - nodes (Entier) : nombre des points nodales du noeud dans les   //
    //        nel elements, nodes(1,nel), nodes(2,nel), ..., nodes(n,nel).   //
    //-----------------------------------------------------------------------//

    for i=1:n
        ig = nodes(i,nel);
        // Assemblage global du vecteur gf
        gf(ig) = gf(ig) + ef(i);

        for j=1:n
            jg = nodes(j,nel);
            // Assemblage global de la matrice de regidit� gk
            gk(ig,jg) = gk(ig,jg) + ek(i,j);
        end
    end
endfunction
//--------------------------- FIN DE ASSEMB -----------------------------//
