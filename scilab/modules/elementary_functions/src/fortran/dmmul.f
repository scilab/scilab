c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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


