      subroutine spreshape(A_m, A_n, A_mnel, A_icol, A_R, A_I,
     $                     B_m, B_n, B_mnel, B_icol, B_R, B_I,
     $                     nel, it, ij, ptr, p)
*
*     PURPOSE
*        reshape a sparse matrix, ie, computes the
*        sparse matrix B such that :  B = matrix(A, m_B, n_B)
*        with  m_B * n_B = m_A * n_A  (this condition being
*        verified at the calling level)
*
*        depending upon the argument it, A may be (and so B) :
*           a/ a boolean sparse (it = -1). In this case the arrays
*              R_A, I_A, R_B, I_B are not used.
*           b/ a real sparse (it = 0). The arrays I_A and I_B are not used.
*          
*           c/ a complex sparse (it = 1)
*
*     ptr : work array of size max(A_n,B_m)+1
*     p : work arrays of size (nel) 
*     ij: work array of size (2,nel) 
*      
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer A_m, A_n, A_mnel(*), A_icol(*)
      integer B_m, B_n, B_mnel(*), B_icol(*)
      integer nel, it, ij(2,*), ptr(*), p(*)
      double precision A_R(*), A_I(*), B_R(*), B_I(*)

*     internal vars
      integer i,j, k, l, ka, kb, num

*     1/ stack the matrix indices by column order
      call iset(A_n+1, 0, ptr, 1)
      do k = 1, nel
         j = A_icol(k)
         ptr(j+1) = ptr(j+1) + 1  
      enddo
      ptr(1) = 1     
      do j = 2, A_n
         ptr(j) = ptr(j) + ptr(j-1)
      enddo

      ka = 0
      do i = 1, A_m
         do l = 1, A_mnel(i)
            ka = ka + 1
            j = A_icol(ka)
            k = ptr(j) 
            ij(1,k) = i
            ij(2,k) = j
            p(k) = ka
            ptr(j) = ptr(j) + 1
         enddo
      enddo
     
*     2/ applies the reshaping onto the indices and computes B_mnel
      call iset(B_m, 0, B_mnel, 1)  ! init B_mnel to 0
      do k = 1, nel
         num = ij(1,k) + (ij(2,k)-1)*A_m - 1
         ij(2,k) = num/B_m + 1
         ij(1,k) = num+1 - B_m*(ij(2,k)-1)
         B_mnel(ij(1,k)) = B_mnel(ij(1,k)) + 1
      enddo

*     3/ computes the others parts of B
      call sz2ptr(B_mnel, B_m, ptr)
      do k = 1, nel
         i = ij(1,k)
         kb = ptr(i)
         B_icol(kb) = ij(2,k)
         if ( it .ge. 0 ) B_R(kb) = A_R(p(k))
         if ( it .eq. 1 ) B_I(kb) = A_I(p(k))
         ptr(i) = ptr(i) + 1
      enddo

      end

