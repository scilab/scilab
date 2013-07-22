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

function [xc,x]=unimesh1D(N,a,b)
    // maillage pour les volumes finis 1D en incluant les noeuds    //
    // aux limites.                                                 //
    // sorties :                                                    //
    //    - x (Double) : vecteur colonne representant les noeuds   //
    //    - xc (Double) : vecteur colonne representant les cellules //
    //      (les volumes de contr�le).                              //
    //    - N (Entier) : est le nombre de noeuds                    //
    //    - a, b (Doubles) :correspondent aux deux points limites   //
    //--------------------------------------------------------------//

    deltax=(b-a)/(N-1);
    dx2=deltax/2;
    x=(a:deltax:b)';
    xc=[a-dx2;(dx2:deltax:b)';b+dx2];

endfunction
