      subroutine htridi(nm,n,ar,ai,d,e,e2,tau)
c
      integer i,j,k,l,n,ii,nm,jp1
      double precision ar(nm,n),ai(nm,n),d(n),e(n),e2(n),tau(2,n)
      double precision f,g,h,fi,gi,hh,si,scale
c
c!purpose
c
c     this subroutine reduces a complex hermitian matrix
c     to a real symmetric tridiagonal matrix using
c     unitary similarity transformations.
c
c!calling sequence
c     subroutine htridi(nm,n,ar,ai,d,e,e2,tau)
c
c     integer n,nm
c     double precision ar(nm,n),ai(nm,n),d(n),e(n),e2(n),tau(2,n)
c
c     on input:
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement;
c
c        n is the order of the matrix;
c
c        ar and ai contain the real and imaginary parts,
c          respectively, of the complex hermitian input matrix.
c          only the lower triangle of the matrix need be supplied.
c
c     on output:
c
c        ar and ai contain information about the unitary trans-
c          formations used in the reduction in their full lower
c          triangles.  their strict upper triangles and the
c          diagonal of ar are unaltered;
c
c        d contains the diagonal elements of the the tridiagonal matrix;
c
c        e contains the subdiagonal elements of the tridiagonal
c          matrix in its last n-1 positions.  e(1) is set to zero;
c
c        e2 contains the squares of the corresponding elements of e.
c          e2 may coincide with e if the squares are not needed;
c
c        tau contains further information about the transformations.
c
c     arithmetic is real except for the use of the subroutines
c
c!originator
c
c     this subroutine is a translation of a complex analogue of
c     the algol procedure tred1, num. math. 11, 181-195(1968)
c     by martin, reinsch, and wilkinson.
c     handbook for auto. comp., vol.ii-linear algebra, 212-226(1971).
c     questions and comments should be directed to b. s. garbow,
c     applied mathematics division, argonne national laboratory
c
c!
c     ------------------------------------------------------------------
c
      tau(1,n) = 1.0d+0
      tau(2,n) = 0.0d+0
c
      do 100 i = 1, n
  100 d(i) = ar(i,i)
c     :::::::::: for i=n step -1 until 1 do -- ::::::::::
      do 300 ii = 1, n
         i = n + 1 - ii
         l = i - 1
         h = 0.0d+0
         scale = 0.0d+0
         if (l .lt. 1) go to 130
c     :::::::::: scale row (algol tol then not needed) ::::::::::
         do 120 k = 1, l
  120    scale = scale + abs(ar(i,k)) + abs(ai(i,k))
c
         if (scale .ne. 0.0d+0) go to 140
         tau(1,l) = 1.0d+0
         tau(2,l) = 0.0d+0
  130    e(i) = 0.0d+0
         e2(i) = 0.0d+0
         go to 290
c
  140    do 150 k = 1, l
            ar(i,k) = ar(i,k) / scale
            ai(i,k) = ai(i,k) / scale
            h = h + ar(i,k) * ar(i,k) + ai(i,k) * ai(i,k)
  150    continue
c
         e2(i) = scale * scale * h
         g = sqrt(h)
         e(i) = scale * g
         f = sqrt(ar(i,l)*ar(i,l)+ai(i,l)*ai(i,l))
c     :::::::::: form next diagonal element of matrix t ::::::::::
         if (f .eq. 0.0d+0) go to 160
         tau(1,l) = (ai(i,l) * tau(2,i) - ar(i,l) * tau(1,i)) / f
         si = (ar(i,l) * tau(2,i) + ai(i,l) * tau(1,i)) / f
         h = h + f * g
         g = 1.0d+0 + g / f
         ar(i,l) = g * ar(i,l)
         ai(i,l) = g * ai(i,l)
         if (l .eq. 1) go to 270
         go to 170
  160    tau(1,l) = -tau(1,i)
         si = tau(2,i)
         ar(i,l) = g
  170    f = 0.0d+0
c
         do 240 j = 1, l
            g = 0.0d+0
            gi = 0.0d+0
c     :::::::::: form element of a*u ::::::::::
            do 180 k = 1, j
               g = g + ar(j,k) * ar(i,k) + ai(j,k) * ai(i,k)
               gi = gi - ar(j,k) * ai(i,k) + ai(j,k) * ar(i,k)
  180       continue
c
            jp1 = j + 1
            if (l .lt. jp1) go to 220
c
            do 200 k = jp1, l
               g = g + ar(k,j) * ar(i,k) - ai(k,j) * ai(i,k)
               gi = gi - ar(k,j) * ai(i,k) - ai(k,j) * ar(i,k)
  200       continue
c     :::::::::: form element of p ::::::::::
  220       e(j) = g / h
            tau(2,j) = gi / h
            f = f + e(j) * ar(i,j) - tau(2,j) * ai(i,j)
  240    continue
c
         hh = f / (h + h)
c     :::::::::: form reduced a ::::::::::
         do 260 j = 1, l
            f = ar(i,j)
            g = e(j) - hh * f
            e(j) = g
            fi = -ai(i,j)
            gi = tau(2,j) - hh * fi
            tau(2,j) = -gi
c
            do 260 k = 1, j
               ar(j,k) = ar(j,k) - f * e(k) - g * ar(i,k)
     x                           + fi * tau(2,k) + gi * ai(i,k)
               ai(j,k) = ai(j,k) - f * tau(2,k) - g * ai(i,k)
     x                           - fi * e(k) - gi * ar(i,k)
  260    continue
c
  270    do 280 k = 1, l
            ar(i,k) = scale * ar(i,k)
            ai(i,k) = scale * ai(i,k)
  280    continue
c
         tau(2,l) = -si
  290    hh = d(i)
         d(i) = ar(i,i)
         ar(i,i) = hh
         ai(i,i) = scale * sqrt(h)
  300 continue
c
      return
c     :::::::::: last card of htridi ::::::::::
      end
