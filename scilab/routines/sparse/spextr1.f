      subroutine spextr1(A_m, A_n, A_nel, A_mnel, A_icol, A_R, A_I,
     $                   B_m, B_n, B_nel, B_mnel, B_icol, B_R, B_I,
     $                   it, i, ni, nel_max, ptr, ierr)
*
*     PURPOSE
*        perform the sparse matrix extraction  B = A(i)
*        where i is a vector of indices of length ni
*
*        With one vector of index, the extraction operation must be
*        meant as if A would be stacked as a big sparse
*        column vector...
*
*        depending upon the argument it, A may be (and so B) :
*           a/ a boolean sparse (it = -1). In this case the arrays
*              A_R, A_I, B_R, B_I are not used.
*           b/ a real sparse (it = 0). The arrays A_I and B_I are not used.
*          
*           c/ a complex sparse (it = 1)
*            
*     CAUTION
*        nel_max is the maximum non zeros elements authorized
*        for B => if this limit is not enought the error
*        indicator ierr is set to -1 (else 0) 
*
*     AUTHOR
*        Bruno Pincon

      implicit none
      integer A_m, A_n, A_nel, A_mnel(*), A_icol(*)
      integer B_m, B_n, B_nel, B_mnel(*), B_icol(*)
      integer it, i(*), ni, nel_max, ptr(*), ierr
      double precision A_R(*), A_I(*), B_R(*), B_I(*)

*     internal vars
      integer k, ka, kb, ia, ja, ii

*     external functions and subroutines called
      integer  dicho_search
      external dicho_search, sz2ptr

      ierr = 0
      if (  ni.lt.0 ) then   ! the case B = A(:) is not treated by this routine
         ierr = -2           ! but via the reshape routine (spmat). So this error
         return              ! must not appeared
      endif

      kb = 1

      if (A_m .eq. 1) then ! B must be in this case a row vector
         do k = 1, ni
            ka = dicho_search(i(k), A_icol(1), A_mnel(1) ) 
            if (ka .ne. 0) then
               if (kb .gt. nel_max) then ! test memoire
                  ierr = -1
                  return
               endif
               B_icol(kb) = k
               if (it .ge. 0)  B_R(kb) = A_R(ka)
               if (it .eq. 1)  B_I(kb) = A_I(ka)
               kb = kb + 1
            endif
         enddo
         B_mnel(1) = kb - 1

      else
         call sz2ptr(A_mnel, A_m-1, ptr)  ! need ptr to acces fastly at the beginning of a row 

         if (A_n .gt. 1) then
            do k = 1, ni
               ii = i(k)
               ia = mod(ii-1, A_m) + 1
               ja = (ii-1)/A_m + 1
               ka = dicho_search(ja, A_icol(ptr(ia)), A_mnel(ia) ) 
            
               if (ka .ne. 0) then
                  if (kb .gt. nel_max) then ! test memoire
                     ierr = -1
                     return
                  endif
                  ka = ka + ptr(ia) - 1
                  B_mnel(k) = 1
                  B_icol(kb) = 1
                  if (it .ge. 0)  B_R(kb) = A_R(ka)
                  if (it .eq. 1)  B_I(kb) = A_I(ka)
                  kb = kb + 1
               else
                  B_mnel(k) = 0
               endif
            enddo
         else     ! A is a sparse column => binary (dicho) search not useful
            do k = 1, ni
               ii = i(k)
               if ( A_mnel(ii) .gt. 0 ) then
                  if (kb .gt. nel_max) then ! test memoire
                     ierr = -1
                     return
                  endif
                  ka = ptr(ii)
                  B_mnel(k) = 1
                  B_icol(kb) = 1
                  if (it .ge. 0)  B_R(kb) = A_R(ka)
                  if (it .eq. 1)  B_I(kb) = A_I(ka)
                  kb = kb + 1
               else
                  B_mnel(k) = 0
               endif
            enddo
         endif
      endif

      B_nel = kb - 1

      end
