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

function M=Disc_diff_oper2(N,h)
    // Cette fonction construit l'ecriture de la discretisation de l'operateur d2u/dt2 //
    // sous forme matricielle.                                                         //
    // sortie :                                                                        //
    //    - M (Double) : est la matrice elementaire de discretisation de l'operateur   //
    // entrees :                                                                       //
    //    - N (Entier) : est le nombre de noeuds                                       //
    //    - h (Double) : est le pas de discretisation h= (b-a)/N  (i.e x(i)= i* h)     //
    //---------------------------------------------------------------------------------//
    // operateur d'ordre 2 d2u/dx2

    h1=h^2;

    // Matrice identique quel que soit le centrage
    Mij=[[(1:N) (2:N) (1:N-1)]',[(1:N) (1:N-1) (2:N)]'];
    Mmn=[N, N];
    Mval=[(-2/h1)*ones(N,1); (1/h1)*ones(N-1,1); (1/h1)*ones(N-1,1)];
    M=sparse(Mij, Mval, Mmn);

endfunction

