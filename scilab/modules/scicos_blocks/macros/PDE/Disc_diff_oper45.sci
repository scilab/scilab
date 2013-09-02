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

function M=Disc_diff_oper45(N,h,DF_type)
    // Cette fonction construit l'ecriture de la discretisation de l'operateur d2u/dtdx //
    // ou de l'operateur du/dx sous forme matricielle.                                  //
    // sortie :                                                                         //
    //    - M (Double) : est la matrice elementaire de discretisation de l'operateur    //
    // entrees :                                                                        //
    //    - N (Entier) : est le nombre de noeuds                                        //
    //    - h (Double) : est le pas de discretisation h= (b-a)/N  (i.e x(i)= i* h)      //
    //    - DF_type (Entier) : 0 pour les differences finies centrees, 1 pour les       //
    //      decentrees a gauche et 2 pour les decentrees � droite                       //
    //----------------------------------------------------------------------------------//

    if ( DF_type == 0) then
        // cas centr�e :
        Mij=[[(1:N-1) (2:N)]',[(2:N) (1:N-1)]'];
        Mmn=[N, N];
        Mval=[(1/(2*h))*ones(N-1,1); (-1/(2*h))*ones(N-1,1)];
        M=sparse(Mij, Mval, Mmn);

    elseif ( DF_type == 1 | DF_type == 2) then
        // les cas d�centr�e
        Mij=[[(1:N) (2:N)]',[(1:N) (1:N-1)]'];
        Mmn=[N, N];
        Mval=[(1/h)*ones(N,1); (-1/h)*ones(N-1,1)];
        M=sparse(Mij, Mval, Mmn);

        if ( DF_type == 2) then
            M=-M';
        end
    end

endfunction

