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


