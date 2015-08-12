      subroutine spif1b(A_m, A_n, A_nel, A_it, A_mnel, A_icol, A_R, A_I,
     $                  B_nel, B_mnel, B_icol, C_it,C_R,C_I,C_is_scalar,
     $                  D_nel, D_it, D_mnel, D_icol, D_R, D_I,
     $                  nelmax, p, q, ierr)

*
*     PURPOSE
*        do the scilab operation A(B) = C : where A is a sparse matrix
*        (real if it = 0, complex if it=1), C is a full vector (or a scalar)
*        and B is a boolean sparse matrix
*        The result being stored in D (a sparse). nelmax is the max number of
*        elements for the resulting matrix D.
*
*        Here B must have the same dimensions than A and the vector C must have
*        B_nel components (or only one).
*
*     AUTHOR
*        Bruno Pincon
*
*
      implicit none
      integer A_m, A_n, A_nel, A_it, A_mnel(*), A_icol(*), 
     $        B_nel, B_mnel(*), B_icol(*), C_it, D_it, D_nel, D_mnel(*), 
     $        D_icol(*), nelmax, p(*), q(*), ierr
      logical C_is_scalar
      double precision A_R(*), A_I(*), C_R(*), C_I(*), D_R(*), D_I(*)

      integer kA, kB, kD, i, in, l, j, j1, j2, kamax
      double precision Ck_R, Ck_I

      if (C_is_scalar) then
         Ck_R = C_R(1)
         if (C_it .eq. 1) then
            Ck_I = C_I(1)
         else
            Ck_I = C_R(1)  ! for the bad trick ....
         endif
      else
*        compute the permutation q to insert row by row (because the order is column
*        wize and the scilab sparse format is row oriented)
         call iset(A_n+1,0,p,1)
         do kB = 1, B_nel
            j = B_icol(kB)
            p(j+1) = p(j+1) + 1
         enddo
         p(1) = 1
         do j = 2, A_n
            p(j) = p(j-1) + p(j)
         enddo
         do kB = 1, B_nel
            j = B_icol(kB)
            q(kB) = p(j)
            p(j) = p(j) + 1
         enddo
      endif

      ierr = 0
      kA = 1
      kB = 1
      kD = 1

      i = 1
      do while ( i .le. A_m )
         if (B_mnel(i) .eq. 0) then
            in = i+1
 10         continue
               if ( in .le. A_m ) then
                  if (B_mnel(in) .eq. 0) then
                     in = in + 1
                     goto 10
                  endif
               endif
            call copy_sprow(i,in-1, kA, A_it, A_mnel, A_icol, A_R, A_I,
     $                      kD, D_it, D_mnel, D_icol, D_R, D_I,
     $                      nelmax, ierr)
            if (ierr .ne. 0) return
            i = in
         else
            D_mnel(i) = 0
            kamax = ka + A_mnel(i) - 1
            j1 = 1
            do l = 1, B_mnel(i)
               j2 = B_icol(kB)
               call insert_j1j2(j1, j2-1, A_it, A_icol, A_R, A_I, kA, 
     $                     kamax, D_it, D_mnel(i), D_icol, D_R, D_I, kD, 
     $                     nelmax, ierr)
               if (ierr .ne. 0) return
               ! insertion de l'élément de C(kB) ds D(i,j2) si non nul
               if (.not. C_is_scalar) then
                  Ck_R = C_R(q(kB))
                  if (C_it .eq. 1) then
                     Ck_I = C_I(q(kB))
                  else
                     Ck_I = Ck_R   ! for the bad trick
                  endif  
               endif

               if ( Ck_R .ne. 0.d0 .or.  Ck_I .ne. 0.d0 ) then
                  if (kD .gt. nelmax) then ! but test if there is enough memory before
                     ierr = -1
                     return
                  endif
                  D_mnel(i) = D_mnel(i) + 1
                  D_icol(kD) = j2
                  D_R(kD) = Ck_R
                  if (D_it .eq. 1) then
                     if (C_it .eq. 1) then
                        D_I(kD) = Ck_I
                     else
                        D_I(kD) = 0.d0
                     endif
                  endif
                  kD = kD + 1
               endif
               kB = kB + 1
               j1 = j2 + 1
            enddo
            call insert_j1j2(j1, A_n, A_it, A_icol, A_R, A_I, kA, 
     $                 kamax, D_it, D_mnel(i), D_icol, D_R, D_I, kD, 
     $                 nelmax, ierr)
            if (ierr .ne. 0) return
            i = i + 1
         endif
      enddo
      D_nel = kD - 1

      end
