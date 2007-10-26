      subroutine corth(nm,n,low,igh,ar,ai,ortr,orti)
c
      integer i,j,m,n,ii,jj,la,mp,nm,igh,kp1,low
      double precision ar(nm,n),ai(nm,n),ortr(igh),orti(igh)
      double precision f,g,h,fi,fr,scale
c
c!purpose
c
c     given a complex general matrix, this subroutine
c     reduces a submatrix situated in rows and columns
c     low through igh to upper hessenberg form by
c     unitary similarity transformations.
c
c!calling sequence
c     subroutine corth(nm,n,low,igh,ar,ai,ortr,orti)
c
c     integer i,j,m,n,ii,jj,la,mp,nm,igh,kp1,low
c     double precision ar(nm,n),ai(nm,n),ortr(igh),orti(igh)
c     double precision f,g,h,fi,fr,scale
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
c          subroutine  cbal.  if  cbal  has not been used,
c          set low=1, igh=n;
c
c        ar and ai contain the real and imaginary parts,
c          respectively, of the complex input matrix.
c
c     on output:
c
c        ar and ai contain the real and imaginary parts,
c          respectively, of the hessenberg matrix.  information
c          about the unitary transformations used in the reduction
c          is stored in the remaining triangles under the
c          hessenberg matrix;
c
c        ortr and orti contain further information about the
c          transformations.  only elements low through igh are used.
c
c!originator
c
c     this subroutine is a translation of a complex analogue of
c     the algol procedure orthes, num. math. 12, 349-368(1968)
c     by martin and wilkinson.
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
         ortr(m) = 0.0d+0
         orti(m) = 0.0d+0
         scale = 0.0d+0
c     :::::::::: scale column (algol tol then not needed) ::::::::::
         do 90 i = m, igh
   90    scale = scale + abs(ar(i,m-1)) + abs(ai(i,m-1))
c
         if (scale .eq. 0.0d+0) go to 180
         mp = m + igh
c     :::::::::: for i=igh step -1 until m do -- ::::::::::
         do 100 ii = m, igh
            i = mp - ii
            ortr(i) = ar(i,m-1) / scale
            orti(i) = ai(i,m-1) / scale
            h = h + ortr(i) * ortr(i) + orti(i) * orti(i)
  100    continue
c
         g = sqrt(h)
        f = sqrt(ortr(m)*ortr(m)+orti(m)*orti(m))
         if (f .eq. 0.0d+0) go to 103
         h = h + f * g
         g = g / f
         ortr(m) = (1.0d+0 + g) * ortr(m)
         orti(m) = (1.0d+0 + g) * orti(m)
         go to 105
c
  103    ortr(m) = g
         ar(m,m-1) = scale
c     :::::::::: form (i-(u*ut)/h) * a ::::::::::
  105    do 130 j = m, n
            fr = 0.0d+0
            fi = 0.0d+0
c     :::::::::: for i=igh step -1 until m do -- ::::::::::
            do 110 ii = m, igh
               i = mp - ii
               fr = fr + ortr(i) * ar(i,j) + orti(i) * ai(i,j)
               fi = fi + ortr(i) * ai(i,j) - orti(i) * ar(i,j)
  110       continue
c
            fr = fr / h
            fi = fi / h
c
            do 120 i = m, igh
               ar(i,j) = ar(i,j) - fr * ortr(i) + fi * orti(i)
               ai(i,j) = ai(i,j) - fr * orti(i) - fi * ortr(i)
  120       continue
c
  130    continue
c     :::::::::: form (i-(u*ut)/h)*a*(i-(u*ut)/h) ::::::::::
         do 160 i = 1, igh
            fr = 0.0d+0
            fi = 0.0d+0
c     :::::::::: for j=igh step -1 until m do -- ::::::::::
            do 140 jj = m, igh
               j = mp - jj
               fr = fr + ortr(j) * ar(i,j) - orti(j) * ai(i,j)
               fi = fi + ortr(j) * ai(i,j) + orti(j) * ar(i,j)
  140       continue
c
            fr = fr / h
            fi = fi / h
c
            do 150 j = m, igh
               ar(i,j) = ar(i,j) - fr * ortr(j) - fi * orti(j)
               ai(i,j) = ai(i,j) + fr * orti(j) - fi * ortr(j)
  150       continue
c
  160    continue
c
         ortr(m) = scale * ortr(m)
         orti(m) = scale * orti(m)
         ar(m,m-1) = -g * ar(m,m-1)
         ai(m,m-1) = -g * ai(m,m-1)
  180 continue
c
  200 return
c     :::::::::: last card of corth ::::::::::
      end
