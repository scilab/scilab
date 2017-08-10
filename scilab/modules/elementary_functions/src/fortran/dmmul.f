c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine dmmul(A , na, B, nb, C, nc, l, m, n)
*
*     PURPOSE
*        computes the matrix product C = A * B
*            C   =   A   *   B
*          (l,n)   (l,m) * (m,n)
*       
*     PARAMETERS
*        input 
*        -----
*        A : (double) array (l, m) with leading dim na
*                 
*        B : (double) array (m, n) with leading dim nb
*    
*        na, nb, nc, l, m, n : integers
*
*        output 
*        ------
*        C : (double) array (l, n) with leading dim nc
*
*     NOTE
*        (original version substituted by a call to the blas dgemm)

      implicit none

      integer na, nb, nc, l, m, n
      double precision A(na,m), B(nb,n), C(nc,n)

      call dgemm('n','n', l, n, m, 1.d0, A, na, B, nb, 0.d0, C, nc)

      end


