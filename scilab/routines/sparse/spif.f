      subroutine spif(A_m, A_n, A_nel, A_it, A_mnel, A_icol, A_R, A_I,
     $                B_m, B_n, B_it, B_R, B_I,
     $                C_m, C_n, C_nel, C_it, C_mnel, C_icol, C_R, C_I,
     $                i, pi, ni, j, pj, nj, nelmax, ierr)

*
*     PURPOSE
*        do the scilab operation A(i,j) = B : where A is a sparse matrix
*        (real if it = 0, complex if it=1), B is a full matrix and i and 
*        j are 2 arrays of indices i(1..ni), j(1..nj).
*        The result being stored in C (a sparse). nelmax is the max number of
*        elements for the resulting matrix C.
*
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer A_m, A_n, A_nel, A_it, A_mnel(*), A_icol(*), 
     $        B_m, B_n, B_it,
     $        C_m, C_n, C_nel, C_it, C_mnel(*), C_icol(*),
     $        i(*), pi(*), ni, j(*), pj(*), nj, nelmax, ierr
      double precision A_R(*), A_I(*), B_R(B_m,*), B_I(B_m,*), 
     $                 C_R(*), C_I(*)

*     local var
      logical allrow, allcol, B_is_scalar
      integer ii, i1, i2, imax, k, ka, kc, A_mnel_ii

*     external functions and subroutines
      logical is_in_order
      external is_in_order, isorti, set_perm_id, copy_fullrow2sprow,
     $         insert_row, iset, copy_sprow

      ierr = 0
      allrow = ni.lt.0  ! ni < 0 for A(:,j) = B
      allcol = nj.lt.0  ! nj < 0 for A(i,:) = B

      if (allrow) then 
         ni = A_m
      else 
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

      if ( (B_m .eq. 1) .and. (B_n .eq.1) ) then
         B_is_scalar = .true.
         B_m = ni
         B_n = nj
      else
         B_is_scalar = .false.
      endif
         
      ka = 1
      kc = 1

      if (allrow .and. allcol) then  ! *** A(:,:) = B
         do ii = 1, B_m
            call copy_fullrow2sprow(ii, kc, C_it, C_mnel(ii), C_icol,
     $                              C_R, C_I, B_m, B_n, B_it, B_R, B_I, 
     $                              B_is_scalar, nelmax, ierr)
            if (ierr .ne. 0) return
         enddo

      elseif (allrow) then            ! *** A(:,j) = B
         call iset(A_m, 0, C_mnel, 1)
         do ii = 1, A_m
           call insert_row(ka, A_it, A_mnel(ii), A_icol, A_R, A_I, 
     $                     kc, C_it, C_mnel(ii), C_icol, C_R, C_I,
     $                     j, pj, nj, ii, B_m, B_it, B_R, B_I,
     $                     B_is_scalar, nelmax, ierr)
            if (ierr .ne. 0) return
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
            i2 = min(ii-1, A_m)
            call copy_sprow(i1,i2, ka, A_it, A_mnel, A_icol, A_R, A_I,
     $                             kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                             nelmax, ierr)
            if (ierr .ne. 0) return
            if (allcol) then         ! *** A(i,:) = B
               call copy_fullrow2sprow(pi(k), kc, C_it, C_mnel(ii), 
     $                                 C_icol, C_R, C_I, 
     $                                 B_m, B_n, B_it, B_R, B_I, 
     $                                 B_is_scalar, nelmax, ierr)
               ka = ka + A_mnel(ii)
            else                     ! *** A(i,j) = B
               if ( ii .gt. A_m ) then
                  A_mnel_ii = 0
               else
                  A_mnel_ii = A_mnel(ii)
               endif
               call insert_row(ka, A_it, A_mnel_ii, A_icol, A_R, A_I, 
     $                         kc, C_it, C_mnel(ii), C_icol, C_R, C_I,
     $                         j, pj, nj, pi(k), B_m, B_it, B_R, B_I,
     $                         B_is_scalar, nelmax, ierr)
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
