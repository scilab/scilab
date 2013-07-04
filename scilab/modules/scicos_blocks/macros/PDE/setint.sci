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

function [xi,w] = setint()
    // la fonction fournit les point d'int�gration x(i) et les poids      //
    // pour la formule quadratique gaussienne.                            //
    //  Sorties:                                                          //
    //     - x(4,4):  x(:,i) is the Gaussian points of order i.           //
    //     - w(4,4):  w(:,i) is the weights of quadrature of order i.     //
    //                                                                    //
    // Reference: Finite element, An introduction, Vol. 1 by E.Becker,    //
    // G.Carey, and J.Oden, pp. 94.                                       //
    //--------------------------------------------------------------------//
    xi(1,1) = 0;
    w(1,1) = 2;		// Gaussian quadrature of order 1

    xi(1,2) = -1/sqrt(3);
    xi(2,2) = -xi(1,2);
    w(1,2) = 1;
    w(2,2) = w(1,2);		// Gaussian quadrature of order 2

    xi(1,3) = -sqrt(3/5);
    xi(2,3) = 0;
    xi(3,3) = -xi(1,3);
    w(1,3) = 5/9;
    w(2,3) = 8/9;
    w(3,3) = w(1,3);	        // Gaussian quadrature of order 3

    xi(1,4) = - 0.861136311594053;
    xi(2,4) = - 0.339981043584856;
    xi(3,4) = -xi(2,4);
    xi(4,4) = -xi(1,4);
    w(1,4) = 0.347854845137454;
    w(2,4) = 0.652145154862646;
    w(3,4) = w(2,4);
    w(4,4) = w(1,4);		// Gaussian quadrature of order 4

endfunction
//--------------------------- FIN DE SETINT --------------------------//
