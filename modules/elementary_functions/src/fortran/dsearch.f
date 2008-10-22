      subroutine dsearchc(X, m, val, n, indX, occ, info) 
*
*
*     PURPOSE
*        val(0..n) being an array (with strict increasing order and n >=1)
*        representing intervals, this routine, by the mean of a 
*        dichotomic search, computes :
*           
*           a/ for each X(i) its interval number indX(i) :
*                     indX(i) = j if  X(i) in (val(j-1), val(j)]
*                             = 1 if  X(i) = val(0)
*                             = 0 if  X(i) is not in [val(0),val(n)]
*
*           b/ the number of points falling in the interval j :
*
*              occ(j) = # { X(i) such that X(i) in (val(j-1), val(j)] } for j>1
*         and  occ(1) = # { X(i) such that X(i) in [val(0), val(1)] }
*             
*     PARAMETERS
*        inputs :
*           m         integer
*           X(1..m)   double float array
*           n         integer
*           val(0..n) double float array (val(0) < val(1) < ....)
*        outputs
*           indX(1..m) integer array
*           occ(1..n)  integer array
*           info       integer (number of X(i) not in [val(0), val(n)])
*
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer m, n, info
      double precision X(m), val(0:n)
      integer occ(n), indX(m)

      integer i, j1, j2, j

      do j = 1, n
         occ(j) = 0
      enddo

      info = 0

      do i = 1, m
         if ( val(0) .le. X(i) .and. X(i) .le. val(n) ) then  
*           X(i) is in [val(0),val(n)] :
*           find j such that val(j-1) <= X(i) <= val(j) by a dicho search
            j1 = 0
            j2 = n
            do while ( j2 - j1 .gt. 1 )
               j = (j1 + j2)/2
               if ( X(i) .le. val(j) ) then
                  j2 = j
               else
                  j1 = j
               endif
            enddo
*           we have val(j1) < X(i) <= val(j2)  if j2 > 1  (j1=j2-1)
*                or val(j1) <= X(i) <= val(j2) if j2 = 1  (j1=j2-1)
*           so that j2 is the good interval number in all cases
            occ(j2) = occ(j2) + 1
            indX(i) = j2
         else     ! X(i) is not in [val(0), val(n)]
            info = info + 1
            indX(i) = 0
         endif
      enddo

      end
*
**************************************************************************
*
      subroutine dsearchd(X, m, val, n, indX, occ, info)
*
*     PURPOSE
*        val(1..n) being a strictly increasing array, this
*        routines by the mean of a dichotomic search computes :
*
*        a/ the number of occurences (occ(j)) of each value val(j) 
*           in the array X :
*
*              occ(j) = #{ X(i) such that X(i) = val(j) }
*
*        b/ the array indX :  if X(i) = val(j) then indX(i) = j
*           (if X(i) is not in val then indX(i) = 0)
*
*     PARAMETERS
*        inputs :
*           m         integer
*           X(1..m)   double float array
*           n         integer
*           val(1..n) double float array (must be in a strict increasing order)
*        outputs :
*           occ(1..n)  integer array
*           indX(1..m) integer array
*           info       integer  (number of X(i) which are not in val(1..n))
*
*     AUTHOR
*        Bruno Pincon
*
      implicit none
      integer m, n, info
      double precision X(m), val(n)
      integer occ(n), indX(m)

      integer i, j1, j2, j

      do j = 1, n
         occ(j) = 0
      enddo

      info = 0

      do i = 1, m
         if ( val(1) .le. X(i) .and. X(i) .le. val(n) ) then
*           find j such that X(i) = val(j) by a dicho search
            j1 = 1
            j2 = n
            do while ( j2 - j1 .gt. 1 )
               j = (j1 + j2)/2
               if ( X(i) .lt. val(j) ) then
                  j2 = j
               else
                  j1 = j
               endif
            enddo
*           here we know that val(j1) <= X(i) <= val(j2)  with j2 = j1 + 1
*           (in fact we have exactly  val(j1) <= X(i) < val(j2) if j2 < n)
            if (X(i) .eq. val(j1)) then
               occ(j1) = occ(j1) + 1
               indX(i) = j1
            else if (X(i) .eq. val(j2)) then  ! (note: this case may happen only for j2=n)
               occ(j2) = occ(j2) + 1
               indX(i) = j2
            else  ! X(i) is not in {val(1), val(2),..., val(n)}
               info = info + 1
               indX(i) = 0
            endif
         else     ! X(i) is not in {val(1), val(2),..., val(n)}
            info = info + 1
            indX(i) = 0
         endif
      enddo

      end
