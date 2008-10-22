      subroutine spextr(A_m, A_n, A_nel, A_mnel, A_icol, A_R, A_I,
     $                  B_m, B_n, B_nel, B_mnel, B_icol, B_R, B_I,
     $                  it, i, ni, j, nj, nel_max, ptr, p, ierr)
*
*     PURPOSE
*        perform a sparse matrix extraction, ie, computes the
*        sparse matrix B such that :  B = A(i,j) where i and
*        j are vectors of indices of length ni and nj respectively.
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
*
      implicit none
      integer A_m, A_n, A_nel, A_mnel(*), A_icol(*)
      integer B_m, B_n, B_nel, B_mnel(*), B_icol(*)
      integer it, i(*), ni, j(*), nj, nel_max, ptr(*), p(*), ierr
      double precision A_R(*), A_I(*), B_R(*), B_I(*)

*     internal vars
      integer k, kk, ka, kb, ib, ia, ja, ptrb
      logical allrow, allcol, j_in_order

*     external functions and subroutines called
      integer dicho_search, dicho_search_bis
      logical is_in_order
      external  dicho_search, dicho_search_bis, is_in_order, qsorti, 
     $          insert_in_order, icopy, unsfdcopy, sz2ptr

      ierr = 0
      allrow = ni.lt.0  ! ni < 0 for a A(:,j) extraction
      allcol = nj.lt.0  ! nj < 0 for a A(i,:) extraction

      if (allrow) then 
         ni = A_m
      endif

      if (allcol) then 
         nj = A_n
      else
         j_in_order = is_in_order(j, nj)
         if ( .not. j_in_order ) call qsorti(j, p, nj)
      endif

      B_m = ni
      B_n = nj

      call sz2ptr(A_mnel, A_m, ptr)

      kb = 1
      do ib = 1, B_m
         B_mnel(ib) = 0

         if (allrow) then
            ia = ib
         else
            ia = i(ib)
         endif 

         if ( A_mnel(ia) .gt. 0 ) then

            if (allcol) then
               if ( kb+A_mnel(ia) .ge. nel_max ) then  ! not enought memory
                  ierr = -1
                  return
               endif
               B_mnel(ib) = A_mnel(ia)
               call icopy(A_mnel(ia), A_icol(ptr(ia)), 1, B_icol(kb), 1)
               if ( it .ge. 0 )
     $           call unsfdcopy(A_mnel(ia), A_R(ptr(ia)), 1, B_R(kb), 1)
               if ( it .eq. 1 )
     $           call unsfdcopy(A_mnel(ia), A_I(ptr(ia)), 1, B_I(kb), 1)
               kb = kb + A_mnel(ia)

            elseif ( nj .gt. A_mnel(ia)  .and.  j_in_order ) then 
               ! algorithm with loop on the row ia of A
               do ka = ptr(ia), ptr(ia+1)-1
                  ja = A_icol(ka)
                  k = dicho_search(ja, j, nj)
                  if ( k .ne. 0 ) then  ! we have found (the smallest) k such that ja = j(k)
 100                 if (kb .gt. nel_max) then ! not enought memory
                        ierr = -1
                        return
                     endif
                     B_mnel(ib) = B_mnel(ib) + 1
                     B_icol(kb) = k
                     if ( it .ge. 0 ) B_R(kb) = A_R(ka)
                     if ( it .eq. 1 ) B_I(kb) = A_I(ka)
                     kb = kb + 1
                     if ( k .lt. nj) then  ! verify if j(k+1) = ja
                        k = k + 1
                        if ( j(k) .eq. ja ) goto 100
                     endif
                  endif
               enddo

            elseif ( nj .gt. 2*A_mnel(ia)  .and.  .not.j_in_order ) then 
               ! algorithm with loop on the row ia of A but the array
               ! j is not in order : nevertheless we may work with j(p(.)) where
               ! p is the permutation which reorders the array j
               ptrb = kb
               do ka = ptr(ia), ptr(ia+1)-1
                  ja = A_icol(ka)
                  k = dicho_search_bis(ja, j, p, nj)
                  if ( k .ne. 0 ) then  ! we have found (the smallest) k such that ja = j(p(k))
 200                 if (kb .gt. nel_max) then ! not enought memory
                        ierr = -1
                        return
                     endif
                     B_mnel(ib) = B_mnel(ib) + 1
                     call insert_in_order( B_icol, ptrb, kb, p(k), it, 
     $                                     B_R, B_I, A_R(ka), A_I(ka) ) 
                     kb = kb + 1
                     if ( k .lt. nj) then  ! verify if j(p(k+1)) = ja
                        k = k + 1
                        if ( j(p(k)) .eq. ja ) goto 200
                     endif
                  endif
               enddo

            else  
               ! algorithm with loop on j(1),...,j(nj)
               do k = 1, nj
                  kk = dicho_search(j(k), A_icol(ptr(ia)), A_mnel(ia))
                  if ( kk .ne. 0 ) then ! we have found kk such that j(k) = A_icol(ptr(ia)-1+kk)
                     ka = kk + ptr(ia) - 1
                     if (kb .gt. nel_max) then ! not enought memory
                        ierr = -1
                        return
                     endif
                     B_mnel(ib) = B_mnel(ib) + 1
                     B_icol(kb) = k
                     if ( it .ge. 0 ) B_R(kb) = A_R(ka)
                     if ( it .eq. 1 ) B_I(kb) = A_I(ka)
                     kb = kb + 1
                  endif
               enddo
            endif
         endif
      enddo

      B_nel = kb - 1

      end

