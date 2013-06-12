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

function [psi,dpsi]=shape(xi,n);
    // la fonction ``shape'' evalue les valeurs des fonction de base et      //
    // d�riv�es en un point xi.                                              //
    // Sorties :                                                             //
    //    - psi (Double) :  valeur de la fonction de base a xi.              //
    //    - dpsi (Double) : valeur de la d�riv�e de la fonction de base a xi.//
    // Entr�es :                                                             //
    //    - xi (Entier) : le point ou la fonction de base est �valu�e.       //
    //    - n (Entier) : la base des functions. n=2,linear,n=3,quadratic,    //
    //      n=4, cubic.                                                      //
    //   Reference: Finite element. An introduction y E.Becker, G.Carey,     //
    //   and J.Oden, Vol.1., pp. 95-96.                                      //
    //-----------------------------------------------------------------------//

    select n
    case 2 then
        // Linear base function
        psi(1) = (1-xi)/2;
        psi(2) = (1+xi)/2;
        dpsi(1) = -0.5;
        dpsi(2) = 0.5;
        return

    case 3 then
        // quadratic base function
        psi(1) = xi*(xi-1)/2;
        psi(2) = 1-xi*xi;
        psi(3) = xi*(xi+1)/2;
        dpsi(1) =  xi-0.5;
        dpsi(2) = -2*xi;
        dpsi(3) = xi + 0.5;
        return

    case 4 then
        // cubic  base function
        psi(1) = 9*(1/9-xi*xi)*(xi-1)/16;
        psi(2) = 27*(1-xi*xi)*(1/3-xi)/16;
        psi(3) = 27*(1-xi*xi)*(1/3+xi)/16;
        psi(4) = -9*(1/9-xi*xi)*(1+xi)/16;

        dpsi(1) = -9*(3*xi*xi-2*xi-1/9)/16;
        dpsi(2) = 27*(3*xi*xi-2*xi/3-1)/16;
        dpsi(3) = 27*(-3*xi*xi-2*xi/3+1)/16;
        dpsi(4) = -9*(-3*xi*xi-2*xi+1/9)/16;
    else
        break
    end
endfunction
//--------------------------- FIN DE SHAPE ---------------------------//
