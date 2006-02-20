      subroutine spisp(A_m, A_n, A_nel, A_it, A_mnel, A_icol, A_R, A_I,
     $                 B_m, B_n, B_nel, B_it, B_mnel, B_icol, B_R, B_I,
     $                 C_m, C_n, C_nel, C_it, C_mnel, C_icol, C_R, C_I,
     $                 B_ptr_row, i, pi, ni, j, pj, nj, nelmax, ierr)

*
*     PURPOSE
*        do the scilab operation A(i,j) = B : where A is a sparse matrix
*        (real if it = 0, complex if it=1), and B is a sparse matrix and i and 
*        j are 2 arrays of indices i(1..ni), j(1..nj).
*        The result being stored in C (a sparse). nelmax is the max number of
*        elements for the resulting matrix C.
*
*        B_m must be equal to ni and B_n to nj 
*
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer A_m, A_n, A_nel, A_it, A_mnel(*), A_icol(*), 
     $        B_m, B_n, B_nel, B_it, B_mnel(*), B_icol(*), B_ptr_row(*),
     $        C_m, C_n, C_nel, C_it, C_mnel(*), C_icol(*),
     $        i(*), pi(*), ni, j(*), pj(*), nj, nelmax, ierr
      double precision A_R(*), A_I(*), B_R(*), B_I(*), 
     $                 C_R(*), C_I(*)

*     local var
      logical allrow, allcol
      integer ii, i1, i2, imax, ib, k, ka, kb, kc, A_mnel_ii, nbe

*     external functions and subroutines
      logical is_in_order
      external is_in_order, isorti, set_perm_id,
     $         insert_sprow, iset, copy_sprow, sz2ptr

      ierr = 0
      allrow = ni.lt.0  ! ni < 0 for A(:,j) = B
      allcol = nj.lt.0  ! nj < 0 for A(i,:) = B

      if (allrow) then 
         ni = A_m
      else 
         call sz2ptr(B_mnel, B_m, B_ptr_row)
         if ( .not. is_in_order(i, ni) ) then
            call isorti(i, pi, ni)
         else
            call set_perm_id(pi, ni)
         endif
         imax = i(pi(ni))
      endif

      if (allcol) then 
         nj = A_n
      else
         if ( .not. is_in_order(j, nj) ) then
            call isorti(j, pj, nj)
         else
            call set_perm_id(pj, nj)
         endif
      endif

      ka = 1
      kb = 1
      kc = 1

      if (allrow .and. allcol) then  ! *** A(:,:) = B
         call copy_sprow(1, A_m, kb, B_it, B_mnel, B_icol, B_R, B_I,
     $                           kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                           nelmax, ierr)
      elseif (allrow) then            ! *** A(:,j) = B
         call iset(A_m, 0, C_mnel, 1)
         do ii = 1, A_m
            call insert_sprow(ka, A_it, A_mnel(ii), A_icol, A_R, A_I,
     $                        kb, B_it, B_mnel(ii), B_icol, B_R, B_I, 
     $                        kc, C_it, C_mnel(ii), C_icol, C_R, C_I,
     $                        j, pj, nj, nelmax, ierr)
            if (ierr .ne. 0) return
            kb = kb + B_mnel(ii)
         enddo

      else                            ! *** A(i,:) = B or  A(i,j) = B
         call iset(imax, 0, C_mnel, 1)
         i1 = 1

         k = 1
*        loop on k from 1 to ni but some values must be skiped
 100        ii = i(pi(k))
            if (k .lt. ni) then
               if (i(pi(k+1)) .eq. ii) then
                  k = k+1
                  goto 100
               endif
            endif
            ib = pi(k)  ! corresponding row of B
            i2 = min(ii-1, A_m)
            if ( i1 .le. i2 ) then
               call copy_sprow(i1,i2,ka, A_it, A_mnel, A_icol, A_R, A_I,
     $                               kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                               nelmax, ierr)
               if (ierr .ne. 0) return
            endif
            
            kb = B_ptr_row(ib)
            if (allcol) then         ! *** A(i,:) = B

*              copy the sparse row ib of B for the row ii of C
*              (this could be done by copy_sprow with a little modif...) 
               nbe = B_mnel(ib)      
               if (kc + nbe .gt. nelmax) then
                  ierr = -1
                  return
               endif
               C_mnel(ii) = nbe
               call icopy( nbe, B_icol(kb), 1, C_icol(kc), 1)
               call unsfdcopy(nbe, B_R(kb), 1, C_R(kc), 1)
               if (C_it .eq. 1) then ! C is complex
                  if (B_it .eq. 1) then ! B is complex
                     call unsfdcopy(nbe, B_I(kb), 1, C_I(kc), 1)
                  else          ! B is real
                     call dset(nbe, 0.d0, C_I(kc), 1)
                  endif
               endif
               ka = ka + A_mnel(ii)
               kc = kc + nbe
            else                     ! *** A(i,j) = B
               if ( ii .gt. A_m ) then
                  A_mnel_ii = 0
               else
                  A_mnel_ii = A_mnel(ii)
               endif

               call insert_sprow(ka, A_it, A_mnel_ii, A_icol, A_R, A_I,
     $                           kb, B_it, B_mnel(ib), B_icol, B_R, B_I, 
     $                           kc, C_it, C_mnel(ii), C_icol, C_R, C_I,
     $                           j, pj, nj, nelmax, ierr)
            endif
            if (ierr .ne. 0) return

            i1 = ii+1
            
            k = k+1
            if (k .le. ni) goto 100
*        endloop on k
         i1 = i(pi(ni))+1
         i2 = A_m
         call copy_sprow(i1,i2, ka, A_it, A_mnel, A_icol, A_R, A_I,
     $                          kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                          nelmax, ierr)
      endif

      C_nel = kc-1

      end
