      subroutine spifp(A_m, A_n, A_nel, A_mnel, A_icol, A_R, A_I, A_it, 
     $                 i, ni, j, nj, ptr, ka, B_it, B_R, B_I, flag)
*
*     PURPOSE
*        look if in place insertion is possible and 
*        if yes perform it.
*            
*     PARAMETERS
*        work arrays : ptr (size A_m), ka (size(ni x nj))
*
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer A_m, A_n, A_nel, A_mnel(*), A_icol(*)
      integer A_it, i(*), ni, j(*), nj, ptr(*), ka(ni,*), B_it, flag
      double precision A_R(*), A_I(*), B_R(ni,*), B_I(ni,*)

*     internal vars
      integer ii, jj, ia, ja, k, pl

*     external functions and subroutines called
      integer  dicho_search
      external dicho_search,  sz2ptr

      call sz2ptr(A_mnel, A_m-1, ptr)

      do ii = 1, ni
         ia = i(ii)
         do jj = 1, nj
            ! we can not insert a zero so we test if B(ii,jj) = 0 or not.
            ! note : if B is real B_I(ii,jj) = B_R(ii,jj)  so the following bad trick works...
            if (B_R(ii,jj).eq.0.d0 .and. B_I(ii,jj).eq.0.d0) then
               flag = 0
               return
            endif
            ja = j(jj)
            pl = dicho_search(ja, A_icol(ptr(ia)), A_mnel(ia))
            if ( pl .ne. 0 ) then
               ka(ii,jj) =  pl + ptr(ia) - 1
            else
               flag = 0
               return
            endif
         enddo
      enddo

      do jj = 1, nj      
         do ii = 1, ni
            k = ka(ii,jj)
            A_R(k) = B_R(ii,jj)
            if (A_it .eq. 1) then
               if (B_it .eq. 1) then
                  A_I(k) = B_I(ii,jj)
               else
                  A_I(k) = 0.d0
               endif
            endif
         enddo
      enddo

      flag = 1

      end
