C/MEMBR ADD NAME=ORTHES,SSI=0
      subroutine orthes(nm,n,low,igh,a,ort)
c
      integer i,j,m,n,ii,jj,la,mp,nm,igh,kp1,low
      double precision a(nm,n),ort(igh)
      double precision f,g,h,scale
c! purpose
c
c     given a real general matrix, this subroutine
c     reduces a submatrix situated in rows and columns
c     low through igh to upper hessenberg form by
c     orthogonal similarity transformations.
c
c! calling sequence
c
c      subroutine orthes(nm,n,low,igh,a,ort)
c
c     on input:
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement;
c
c        n is the order of the matrix;
c
c        low and igh are integers determined by the balancing
c          subroutine  balanc.  if  balanc  has not been used,
c          set low=1, igh=n;
c
c        a contains the input matrix.
c
c     on output:
c
c        a contains the hessenberg matrix.  information about
c          the orthogonal transformations used in the reduction
c          is stored in the remaining triangle under the
c          hessenberg matrix;
c
c        ort contains further information about the transformations.
c          only elements low through igh are used.
c
c!originator
c
c     this subroutine is a translation of the algol procedure orthes,
c     num. math. 12, 349-368(1968) by martin and wilkinson.
c     handbook for auto. comp., vol.ii-linear algebra, 339-358(1971).
c     questions and comments should be directed to b. s. garbow,
c     applied mathematics division, argonne national laboratory
c
c!
c     ------------------------------------------------------------------
c
      la = igh - 1
      kp1 = low + 1
      if (la .lt. kp1) go to 200
c
      do 180 m = kp1, la
         h = 0.0d+0
         ort(m) = 0.0d+0
         scale = 0.0d+0
c     :::::::::: scale column (algol tol then not needed) ::::::::::
         do 90 i = m, igh
   90    scale = scale + abs(a(i,m-1))
c
         if (scale .eq. 0.0d+0) go to 180
         mp = m + igh
c     :::::::::: for i=igh step -1 until m do -- ::::::::::
         do 100 ii = m, igh
            i = mp - ii
            ort(i) = a(i,m-1) / scale
            h = h + ort(i) * ort(i)
  100    continue
c
         g = -sign(sqrt(h),ort(m))
         h = h - ort(m) * g
         ort(m) = ort(m) - g
c     :::::::::: form (i-(u*ut)/h) * a ::::::::::
         do 130 j = m, n
            f = 0.0d+0
c     :::::::::: for i=igh step -1 until m do -- ::::::::::
            do 110 ii = m, igh
               i = mp - ii
               f = f + ort(i) * a(i,j)
  110       continue
c
            f = f / h
c
            do 120 i = m, igh
  120       a(i,j) = a(i,j) - f * ort(i)
c
  130    continue
c     :::::::::: form (i-(u*ut)/h)*a*(i-(u*ut)/h) ::::::::::
         do 160 i = 1, igh
            f = 0.0d+0
c     :::::::::: for j=igh step -1 until m do -- ::::::::::
            do 140 jj = m, igh
               j = mp - jj
               f = f + ort(j) * a(i,j)
  140       continue
c
            f = f / h
c
            do 150 j = m, igh
  150       a(i,j) = a(i,j) - f * ort(j)
c
  160    continue
c
         ort(m) = scale * ort(m)
         a(m,m-1) = scale * g
  180 continue
c
  200 return
c     :::::::::: last card of orthes ::::::::::
      end
