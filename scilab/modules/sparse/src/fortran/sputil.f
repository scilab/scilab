* this file contains some utility routines for some sparse
* operations (extraction / insertion)
*
* I will comment a little more in the future
* (bruno december 2004)
*

      subroutine  set_perm_id(p, n)
      implicit none
      integer p(*), n
      integer i
      do i = 1, n
         p(i) = i
      enddo
      end ! subroutine set_perm_id


      subroutine insert_row(ka, A_it, A_mnel, A_icol, A_R, A_I, 
     $                      kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                      j, p, nj, ib, B_m, B_it, B_R, B_I,
     $                      B_is_scalar, nelmax, ierr)

*      ka, kc : pointent sur les debuts de ligne de A et C

      implicit none
      integer ka, A_it, A_mnel, A_icol(*), kc, C_it, C_mnel, C_icol(*),
     $        j(*), p(*), nj, it, ib, nelmax, ierr, B_m, B_it
      logical B_is_scalar
      double precision A_R(*), A_i(*), C_R(*), C_I(*),
     $                 B_R(B_m,*), B_I(B_m,*)

*     local vars
      integer i, j1, j2, jp, k, kamax
      double precision Bij_R, Bij_I
*     external functions and subroutines
      external insert_j1j2

      if (B_is_scalar) then
         Bij_R = B_R(1,1)
         if (B_it .eq. 1) Bij_I = B_I(1,1)
      endif

*     indice max pour la ligne de A
      kamax = ka + A_mnel - 1
      j1  = 1

      k = 1
*     loop on k from 1 to nj but some values must be skiped
 100     jp = j(p(k))
         if ( k .lt. nj ) then
            if ( j(p(k+1)) .eq. jp ) then
               k = k+1
               goto 100
            endif
         endif

         j2 = jp - 1
         call insert_j1j2(j1, j2, A_it, A_icol, A_R, A_I, ka, 
     $                    kamax, C_it, C_mnel, C_icol, C_R, C_I, kc, 
     $                    nelmax, ierr)
         if (ierr .ne. 0) return
                  
       ! insertion de B ...
         if (kc .gt. nelmax) then ! test if there is enough memory
            ierr = -1
            return
         endif

         if ( .not. B_is_scalar ) then
            Bij_R = B_R(ib,p(k))
            if (B_it .eq. 1) Bij_I = B_I(ib,p(k))
         endif

         if (C_it .eq. 0) then        ! real case
            if ( Bij_R .ne. 0 ) then
               C_icol(kc) = jp
               C_mnel = C_mnel + 1
               C_R(kc) = Bij_R
               kc = kc + 1
             endif

         elseif (B_it .eq. 0) then        ! complex case but B is real
            if ( Bij_R .ne. 0.d0 ) then
               C_R(kc) = Bij_R
               C_I(kc) = 0.d0
               C_icol(kc) = jp
               C_mnel = C_mnel + 1
               kc = kc + 1
            endif

         else                             ! complex case , B complex
            if ( Bij_R.ne.0.d0 .or. Bij_I.ne.0.d0 ) then
               C_R(kc) =  Bij_R
               C_I(kc) =  Bij_I
               C_icol(kc) = jp
               C_mnel = C_mnel + 1
               kc = kc + 1
            endif
         endif

         j1 = jp + 1
         k = k + 1
         if (k .le. nj) goto 100
*      endloop on k
 
      j2 = A_icol(kamax)
      call insert_j1j2(j1, j2, A_it, A_icol, A_R, A_I, ka, 
     $                 kamax, C_it, C_mnel, C_icol, C_R, C_I, kc, 
     $                 nelmax, ierr)

      end ! subroutine insert_row
         

      subroutine copy_sprow(i1,i2, ka, A_it, A_mnel, A_icol, A_R, A_I,
     $                             kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                             nelmax, ierr)
*
*     recopie les lignes i1 à i2 de A sur C
*         
      implicit none
      integer i1, i2, ka, A_it, A_mnel(*), A_icol(*),
     $                kc, C_it, C_mnel(*), C_icol(*),
     $        it, nelmax, ierr
      double precision A_R(*), A_I(*), C_R(*), C_I(*)

*     local vars
      integer nbe, i, n
*     external functions and subroutines
      external icopy, unsfdcopy

      if (i1 .gt. i2) return  ! a mettre au niveau de l'appelant ?

      nbe = 0
      do i = i1, i2
         nbe = nbe + A_mnel(i)
      enddo
      if (kc + nbe .gt. nelmax) then
         ierr = -1
         return
      endif

      n = i2 - i1 + 1
      call icopy(n,   A_mnel(i1), 1, C_mnel(i1), 1)
      call icopy(nbe, A_icol(ka), 1, C_icol(kc), 1)
      if (C_it .ge. 0) then
         call unsfdcopy(nbe, A_R(ka), 1, C_R(kc), 1)
         if (C_it .eq. 1) then     ! C is complex
            if (A_it .eq. 1) then   ! A is complex
               call unsfdcopy(nbe, A_I(ka), 1, C_I(kc), 1)
            else                   ! A is real
               call dset(nbe, 0.d0, C_I(kc), 1)
            endif
         endif
      endif

      ! mise à jour de ka et kc
      ka = ka + nbe
      kc = kc + nbe

      end ! subroutine copy_sprow


      subroutine insert_j1j2(j1, j2, A_it, A_icol, A_R, A_I, ka, kamax, 
     $                       C_it, C_mnel, C_icol, C_R, C_I, kc, 
     $                       nelmax, ierr)
*
*     insere les coefs de la ligne A dans C dont les indices de
*     colonnes sont compris entre j1 et j2
      
      implicit none
      integer j1, j2, A_it, A_icol(*), ka, kamax, C_it, C_mnel, 
     $        C_icol(*), kc, nelmax, ierr
      double precision A_R(*), A_I(*), C_R(*), C_I(*)

*     local var
      integer j

      if ( ka .gt. kamax ) return

*     goto the first element with column index >= j1
      do while ( A_icol(ka) .lt. j1 ) 
         ka = ka + 1
         if (ka .gt. kamax) return
      enddo

      j = A_icol(ka)
      do while (j .le. j2)   ! add a new element to C 
         if (kc .gt. nelmax) then  ! but test if there is enough memory before
            ierr = -1
            return
         endif
         C_icol(kc) = j
         C_R(kc) = A_R(ka)
         if ( C_it .eq. 1 ) then ! C is complex
            if ( A_it .eq. 0 ) then ! A is real
               C_I(kc) = 0.d0
            else                ! A is complex
               C_I(kc) = A_I(ka)
            endif
         endif
         kc = kc + 1          ! number of the next element of C
         C_mnel = C_mnel + 1  ! increment the number of nnz of the row for C
         ka = ka + 1
         if ( ka .gt. kamax ) return
         j = A_icol(ka)
      enddo
            
      end ! subroutine insert_j1j2


      subroutine copy_fullrow2sprow(i, kc, C_it, C_mnel, C_icol, C_R,
     $                              C_I, B_m, B_n, B_it, B_R, B_I,
     $                              B_is_scalar, nelmax, ierr)
      implicit none
      integer i, kc, C_it, C_mnel, C_icol(*), B_m, B_n, B_it, nelmax, 
     $        ierr
      logical B_is_scalar
      double precision C_R(*), C_I(*), B_R(B_m,*), B_I(B_m,*)
      integer j
      double precision Bij_R, Bij_I

      if (B_is_scalar) then
         Bij_R = B_R(1,1)
         if (B_it .eq. 1) Bij_I = B_I(1,1)
      endif

      do j = 1, B_n

         if (kc .gt. nelmax) then ! test if enough memory  
            ierr = -1
            return
         endif

         if ( .not. B_is_scalar ) then
            Bij_R = B_R(i,j)
            if (B_it .eq. 1) Bij_I = B_I(i,j)
         endif

         if (C_it .eq. 0) then      ! real case (ie A and B are real)
            if ( Bij_R .ne. 0.d0 ) then
               C_icol(kc) = j
               C_R(kc) = Bij_R
               C_mnel = C_mnel + 1
               kc = kc + 1
            endif

         else ! C_it = 1 this is the complex case (but B may be real or complex)
            if (B_it .eq. 0) then   ! B is real
               if ( Bij_R .ne. 0.d0 ) then
                  C_icol(kc) = j
                  C_R(kc) = Bij_R
                  C_I(kc) = 0.d0
                  C_mnel = C_mnel + 1
                  kc = kc + 1
               endif
            else                    ! B is complex
               if ( Bij_R.ne.0.d0 .or. Bij_I.ne.0.d0 ) then
                  C_icol(kc) = j
                  C_R(kc) = Bij_R
                  C_I(kc) = Bij_I
                  C_mnel = C_mnel + 1
                  kc = kc + 1
               endif
            endif
         endif

      enddo

      end  ! subroutine copy_fullrow2sprow

      subroutine isorti(ix, p, n)

      implicit none
      integer n, ix(n), p(n)
      
      integer i, j, pivot, temp
      call set_perm_id(p, n)

      do i = 2, n
         pivot = ix(p(i))
         temp = p(i)
         j = i-1
 10      continue
            if ( ix(p(j)) .gt. pivot ) then
               p(j+1) = p(j)
               j = j-1
               if ( j .ge. 1 ) goto 10
            endif
         p(j+1) = temp
      enddo

      end ! subroutine isorti
      
                              
      integer function dicho_search(jj, val, n)
*
*     PURPOSE
*        val(1..n) being a increasing array, this
*        routines by the mean of a dichotomic search computes
*        the smallest indice of jj in val, ie the smallest 
*        integer k such that val(k) = jj or return 0 if jj 
*        is not in val.
*
*     EXAMPLE
*        if val = [1 4 5 5 8 8 12] we must
*        get  k = 3 if jj = 5
*             k = 5 if jj = 8
*             k = 0 if jj = 3
*             etc... 
*
*     code brings by Bruno to speed up sparse extraction

      implicit none
      integer jj, n, val(n)

      integer k, k1, k2

      if ( n .lt. 1 ) then ! void search array 
         dicho_search = 0
         return
      endif

      if ( val(1) .le. jj  .and.  jj .le. val(n) ) then
*        find k such that jj = val(k) by a dicho search
         k1 = 1
         k2 = n
         do while ( k2 - k1 .gt. 1 )
            k = (k1 + k2)/2
            if ( jj .le. val(k) ) then
               k2 = k
            else
               k1 = k
            endif
         enddo
*        here we know that val(k1) <= jj <= val(k2)  with k2 = k1 + 1 
         if (jj .eq. val(k1)) then
            dicho_search = k1
         else if (jj .eq. val(k2)) then
            dicho_search = k2
         else
            dicho_search = 0
         endif
      else
         dicho_search = 0
      endif
      
      end 

      integer function dicho_search_bis(jj, val, p, n)
*
*     PURPOSE
*        same operation than the previus function but here
*        the array val is in increasing order through the
*        the permutation p, ie we have :
*            val(p(1)) <= val(p(2)) <= ... <= val(p(n))
*
*     code brings by Bruno to speed up sparse extraction

      implicit none
      integer jj, n, val(n), p(n)

      integer k, k1, k2

      if ( n .lt. 1 ) then ! void search array 
         dicho_search_bis = 0
         return
      endif

      if ( val(p(1)) .le. jj  .and.  jj .le. val(p(n)) ) then
*        find the smallest k such that jj = val(p(k)) by a dicho search
         k1 = 1
         k2 = n
         do while ( k2 - k1 .gt. 1 )
            k = (k1 + k2)/2
            if ( jj .le. val(p(k)) ) then
               k2 = k
            else
               k1 = k
            endif
         enddo
*        here we know that val(p(k1)) <= jj <= val(p(k2))  with k2 = k1 + 1 
         if (jj .eq. val(p(k1))) then
            dicho_search_bis = k1
         else if (jj .eq. val(p(k2))) then
            dicho_search_bis = k2
         else
            dicho_search_bis = 0
         endif
      else
         dicho_search_bis = 0
      endif
      
      end 

      logical function is_in_order(val, n)
*
*     PURPOSE
*        test if the array val(1..n) is in increasing order
*
*     code brings by Bruno to speed up sparse extraction
*
      implicit none
      integer n, val(n), i
      
      is_in_order = .true.

      do i = 2, n
         if ( val(i) .lt. val(i-1) ) then
            is_in_order = .false.
            return
         endif
      enddo

      end


      subroutine insert_in_order(icol, kleft, kright, ind, it, B_R, B_I, 
     $                           val_re, val_im)
*
*    expliquer ....
*
      implicit none
      integer icol(*), kleft, kright, ind, it
      double precision B_R(*), B_I(*), val_re, val_im

      integer k

      k = kright
      if (k .gt. kleft) then  ! a caution due to the fortran evaluation of .and.
         do while (k .gt. kleft  .and.  icol(k-1) .gt. ind)
            icol(k) = icol(k-1)
            if (it .ge. 0) B_R(k) = B_R(k-1)
            if (it .eq. 1) B_I(k) = B_I(k-1)
            k = k-1
         enddo
      endif
      icol(k) = ind
      if (it .ge. 0) B_R(k) = val_re
      if (it .eq. 1) B_I(k) = val_im

      end


**********************************************************************
*
*    subroutine added by Bruno (Feb 20 2006 to speed up the
*     sparse(i,j) = sparse insertion
*
      subroutine insert_sprow(ka, A_it, A_mnel, A_icol, A_R, A_I,
     $                        kb, B_it, B_mnel, B_icol, B_R, B_I, 
     $                        kc, C_it, C_mnel, C_icol, C_R, C_I,
     $                        j, p, nj, nelmax, ierr)

*      ka, kb, kc : pointent sur les debuts de ligne de A, B et C
*      kb  is not modified
*      ka is equal to kamax+1 on output (indice pointer on the next row of A)
*      kc is modified (on output it is the index of the future next element of C)
*
      implicit none
      integer ka, A_it, A_mnel, A_icol(*), 
     $        kb, B_it, B_mnel, B_icol(*),
     $        kc, C_it, C_mnel, C_icol(*),
     $        j(*), p(*), nj, it, ib, nelmax, ierr
      double precision A_R(*), A_i(*), B_R(*), B_I(*), C_R(*), C_I(*)

*     local vars
      integer i, j1, j2, jp, k, kamax, num
*     external functions and subroutines
      external dicho_search
      integer dicho_search
      external insert_j1j2

*     indice max pour la ligne de A
      kamax = ka + A_mnel - 1
      j1  = 1

      k = 1
*     loop on k from 1 to nj but some values must be skiped
 100     jp = j(p(k))
         if ( k .lt. nj ) then
            if ( j(p(k+1)) .eq. jp ) then
               k = k+1
               goto 100
            endif
         endif

         j2 = jp - 1
         if ( j1 .le. j2 ) then
            call insert_j1j2(j1, j2, A_it, A_icol, A_R, A_I, ka, 
     $                       kamax, C_it, C_mnel, C_icol, C_R, C_I, kc, 
     $                       nelmax, ierr)
            if (ierr .ne. 0) return
         endif

       ! insertion de B (ib, p(k))
         num = dicho_search(p(k), B_icol(kb), B_mnel)
         if ( num .ne. 0 ) then  ! B(ib,p(k)) is not zero insert it in C
            num = kb + num - 1
            if (kc .gt. nelmax) then ! test if there is enough memory
               ierr = -1
               return
            endif

            C_icol(kc) = jp
            C_mnel = C_mnel + 1
            C_R(kc) = B_R(num)

            if ( C_it .eq. 1 ) then ! C is complex
               if (B_it .eq. 0) then !  but B is real
                  C_I(kc) = 0.d0
               else                  ! complex case , B complex
                  C_I(kc) = B_I(num)
               endif
            endif
            kc = kc + 1
         endif

         j1 = jp + 1
         k = k + 1
         if (k .le. nj) goto 100
*      endloop on k
 
      j2 = A_icol(kamax)
      if ( j1 .le. j2 ) then
         call insert_j1j2(j1, j2, A_it, A_icol, A_R, A_I, ka, 
     $                    kamax, C_it, C_mnel, C_icol, C_R, C_I, kc, 
     $                    nelmax, ierr)
      endif

      ka = kamax + 1

      end ! subroutine insert_sprow
 


      SUBROUTINE QSORTI (X, IND, N)
      INTEGER N, X(N), IND(N)
C
C***********************************************************
C
C                                               ROBERT RENKA
C                                       OAK RIDGE NATL. LAB.
C
C   THIS SUBROUTINE USES AN ORDER N*LOG(N) QUICK SORT TO
C SORT AN INTEGER ARRAY X INTO INCREASING ORDER.  THE ALGOR-
C ITHM IS AS FOLLOWS.  IND IS INITIALIZED TO THE ORDERED
C SEQUENCE OF INDICES 1,...,N, AND ALL INTERCHANGES ARE
C APPLIED TO IND.  X IS DIVIDED INTO TWO PORTIONS BY PICKING
C A CENTRAL ELEMENT T.  THE FIRST AND LAST ELEMENTS ARE COM-
C PARED WITH T, AND INTERCHANGES ARE APPLIED AS NECESSARY SO
C THAT THE THREE VALUES ARE IN ASCENDING ORDER.  INTER-
C CHANGES ARE THEN APPLIED SO THAT ALL ELEMENTS GREATER THAN
C T ARE IN THE UPPER PORTION OF THE ARRAY AND ALL ELEMENTS
C LESS THAN T ARE IN THE LOWER PORTION.  THE UPPER AND LOWER
C INDICES OF ONE OF THE PORTIONS ARE SAVED IN LOCAL ARRAYS,
C AND THE PROCESS IS REPEATED ITERATIVELY ON THE OTHER
C PORTION.  WHEN A PORTION IS COMPLETELY SORTED, THE PROCESS
C BEGINS AGAIN BY RETRIEVING THE INDICES BOUNDING ANOTHER
C UNSORTED PORTION.
C
C INPUT PARAMETERS -   N - LENGTH OF THE ARRAY X.
C
C                      X - VECTOR OF LENGTH N TO BE SORTED.
C
C                    IND - VECTOR OF LENGTH .GE. N.
C
C N AND X ARE NOT ALTERED BY THIS ROUTINE.
C
C OUTPUT PARAMETER - IND - SEQUENCE OF INDICES 1,...,N
C                          PERMUTED IN THE SAME FASHION AS X
C                          WOULD BE.  THUS, THE ORDERING ON
C                          X IS DEFINED BY Y(I) = X(IND(I)).
C
C INTRINSIC FUNCTIONS CALLED BY QSORTI - IFIX, FLOAT
C
C***********************************************************
C
C NOTE -- IU AND IL MUST BE DIMENSIONED .GE. LOG(N) WHERE
C         LOG HAS BASE 2.
C
C***********************************************************
C
      INTEGER IU(21), IL(21)
      INTEGER M, I, J, K, L, IJ, IT, ITT, INDX, T
      REAL    R
C
C LOCAL PARAMETERS -
C
C IU,IL =  TEMPORARY STORAGE FOR THE UPPER AND LOWER
C            INDICES OF PORTIONS OF THE ARRAY X
C M =      INDEX FOR IU AND IL
C I,J =    LOWER AND UPPER INDICES OF A PORTION OF X
C K,L =    INDICES IN THE RANGE I,...,J
C IJ =     RANDOMLY CHOSEN INDEX BETWEEN I AND J
C IT,ITT = TEMPORARY STORAGE FOR INTERCHANGES IN IND
C INDX =   TEMPORARY INDEX FOR X
C R =      PSEUDO RANDOM NUMBER FOR GENERATING IJ
C T =      CENTRAL ELEMENT OF X
C
      IF (N .LE. 0) RETURN
C
C INITIALIZE IND, M, I, J, AND R
C
      DO 1 I = 1,N
    1   IND(I) = I
      M = 1
      I = 1
      J = N
      R = .375
C
C TOP OF LOOP
C
    2 IF (I .GE. J) GO TO 10
      IF (R .GT. .5898437) GO TO 3
      R = R + .0390625
      GO TO 4
    3 R = R - .21875
C
C INITIALIZE K
C
    4 K = I
C
C SELECT A CENTRAL ELEMENT OF X AND SAVE IT IN T
C
      IJ = I + IFIX(R*FLOAT(J-I))
      IT = IND(IJ)
      T = X(IT)
C
C IF THE FIRST ELEMENT OF THE ARRAY IS GREATER THAN T,
C   INTERCHANGE IT WITH T
C
      INDX = IND(I)
      IF (X(INDX) .LE. T) GO TO 5
      IND(IJ) = INDX
      IND(I) = IT
      IT = INDX
      T = X(IT)
C
C INITIALIZE L
C
    5 L = J
C
C IF THE LAST ELEMENT OF THE ARRAY IS LESS THAN T,
C   INTERCHANGE IT WITH T
C
      INDX = IND(J)
      IF (X(INDX) .GE. T) GO TO 7
      IND(IJ) = INDX
      IND(J) = IT
      IT = INDX
      T = X(IT)
C
C IF THE FIRST ELEMENT OF THE ARRAY IS GREATER THAN T,
C   INTERCHANGE IT WITH T
C
      INDX = IND(I)
      IF (X(INDX) .LE. T) GO TO 7
      IND(IJ) = INDX
      IND(I) = IT
      IT = INDX
      T = X(IT)
      GO TO 7
C
C INTERCHANGE ELEMENTS K AND L
C
    6 ITT = IND(L)
      IND(L) = IND(K)
      IND(K) = ITT
C
C FIND AN ELEMENT IN THE UPPER PART OF THE ARRAY WHICH IS
C   NOT LARGER THAN T
C
    7 L = L - 1
      INDX = IND(L)
      IF (X(INDX) .GT. T) GO TO 7
C
C FIND AN ELEMENT IN THE LOWER PART OF THE ARRAY WHCIH IS
C   NOT SMALLER THAN T
C
    8 K = K + 1
      INDX = IND(K)
      IF (X(INDX) .LT. T) GO TO 8
C
C IF K .LE. L, INTERCHANGE ELEMENTS K AND L
C
      IF (K .LE. L) GO TO 6
C
C SAVE THE UPPER AND LOWER SUBSCRIPTS OF THE PORTION OF THE
C   ARRAY YET TO BE SORTED
C
      IF (L-I .LE. J-K) GO TO 9
      IL(M) = I
      IU(M) = L
      I = K
      M = M + 1
      GO TO 11
C
    9 IL(M) = K
      IU(M) = J
      J = L
      M = M + 1
      GO TO 11
C
C BEGIN AGAIN ON ANOTHER UNSORTED PORTION OF THE ARRAY
C
   10 M = M - 1
      IF (M .EQ. 0) RETURN
      I = IL(M)
      J = IU(M)
C
   11 IF (J-I .GE. 11) GO TO 4
      IF (I .EQ. 1) GO TO 2
      I = I - 1
C
C SORT ELEMENTS I+1,...,J.  NOTE THAT 1 .LE. I .LT. J AND
C   J-I .LT. 11.
C
   12 I = I + 1
      IF (I .EQ. J) GO TO 10
      INDX = IND(I+1)
      T = X(INDX)
      IT = INDX
      INDX = IND(I)
      IF (X(INDX) .LE. T) GO TO 12
      K = I
C
   13 IND(K+1) = IND(K)
      K = K - 1
      INDX = IND(K)
      IF (T .LT. X(INDX)) GO TO 13
      IND(K+1) = IT
      GO TO 12
      END
 
    
