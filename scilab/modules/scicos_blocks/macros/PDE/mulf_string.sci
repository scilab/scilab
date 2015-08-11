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

function vect=mulf_string(M,vec)
    // fonction pour la multiplication matrice     //
    // (string) * vecteur(string)                  //
    // sortie :                                    //
    //    - vect (String) : Vecteur de taille de M //
    // entr�es :                                   //
    //    - M (String) : Matrice                   //
    //    - N (String) : Vecteur                   //
    //---------------------------------------------//
    vect=[];
    N=size(vec,"*");
    for i=1:N
        line=[];
        for j=1:N
            line=[line,"("+M(i,j) + ")*(" +vec(j)+")"];
        end
        som="";
        for i=1:N
            som=som + "+" + line(i);
        end
        vect=[vect;som];
    end
endfunction
