      subroutine spt(m, n, nel, it, ptr, 
     $               A_R,  A_I,  A_mnel,  A_icol, 
     $               At_R, At_I, At_mnel, At_icol)
*
*     PURPOSE                                       T 
*        transpose a sparse (scilab) matrix : At = A
*
*     PARAMETERS
*        inputs
*           m : number of rows of A
*           n : number of columns of A
*         nel : integer, number of non-nul elements of A
*          it : -1 for a sparse boolean,  0 a sparse real and 1 for a  complex sparse
*         A_R : double float array (1..nel), values of the non nul elements
*               stored row by row with increasing column indices within a row
*         A_I : the same for complex values (case it =1) 
*      A_icol : integer array (1..nel), columns indices : A_icol(k)
*               is column indice of the element stored in A_R(k)
*      A_mnel : integer array (1..n) A_mnel(i) is the number of non
*               nul elements of row i
*
*       outputs
*        At_R, At_I, At_icol, At_mnel : the same for the transposed matrix
*
*       work array : ptr of size n
*               
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer m, n, nel, it, ptr(*)
      integer A_icol(nel), A_mnel(m), At_icol(nel), At_mnel(n)
      double precision A_R(nel), A_I(nel), At_R(nel), At_I(nel)

      integer i, j, ka, kb, l

*     compute At_mnel (At_mnel(i) = number of non nul elements of row i of At) 
      call iset(n, 0, At_mnel, 1)  ! init At_mnel with 0
      do ka = 1, nel
         j = A_icol(ka)
         At_mnel(j) = At_mnel(j) + 1
      enddo

*     now compute ptr(i) such that it is the index in At_icol
*     (and At_R and At_I) of the first non nul element of row i of At
      call sz2ptr(At_mnel, n-1, ptr)

*     now transpose
      ka = 0
      do i = 1, m
         do l = 1, A_mnel(i)
            ka = ka + 1
            j = A_icol(ka)
            kb = ptr(j) 
            At_icol(kb) = i
            if ( it .ge. 0 ) At_R(kb) = A_R(ka)
            if ( it .eq. 1 ) At_I(kb) = A_I(ka)
            ptr(j) = ptr(j) + 1
         enddo
      enddo

      end

