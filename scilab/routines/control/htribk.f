      subroutine htribk(nm,n,ar,ai,tau,m,zr,zi)
c
      integer i,j,k,l,m,n,nm
      double precision ar(nm,n),ai(nm,n),tau(2,n),zr(nm,m),zi(nm,m)
      double precision h,s,si
c
c!purpose
c     this subroutine forms the eigenvectors of a complex hermitian
c     matrix by back transforming those of the corresponding
c     real symmetric tridiagonal matrix determined by  htridi.
c
c!calling sequence
c     subroutine htribk(nm,n,ar,ai,tau,m,zr,zi)
c
c     integer i,j,k,l,m,n,nm
c     double precision ar(nm,n),ai(nm,n),tau(2,n),zr(nm,m),zi(nm,m)
c     double precision h,s,si
c
c     on input:
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement;
c
c        n is the order of the matrix;
c
c        ar and ai contain information about the unitary trans-
c          formations used in the reduction by  htridi  in their
c          full lower triangles except for the diagonal of ar;
c
c        tau contains further information about the transformations;
c
c        m is the number of eigenvectors to be back transformed;
c
c        zr contains the eigenvectors to be back transformed
c          in its first m columns.
c
c     on output:
c
c        zr and zi contain the real and imaginary parts,
c          respectively, of the transformed eigenvectors
c          in their first m columns.
c
c     note that the last component of each returned vector
c     is real and that vector euclidean norms are preserved.
c
c!originator
c
c     this subroutine is a translation of a complex analogue of
c     the algol procedure trbak1, num. math. 11, 181-195(1968)
c     by martin, reinsch, and wilkinson.
c     handbook for auto. comp., vol.ii-linear algebra, 212-226(1971).
c
c     questions and comments should be directed to b. s. garbow,
c     applied mathematics division, argonne national laboratory
c
c!
c     ------------------------------------------------------------------
c
      if (m .eq. 0) go to 200
c     :::::::::: transform the eigenvectors of the real symmetric
c                tridiagonal matrix to those of the hermitian
c                tridiagonal matrix. ::::::::::
      do 50 k = 1, n
c
         do 50 j = 1, m
            zi(k,j) = -zr(k,j) * tau(2,k)
            zr(k,j) = zr(k,j) * tau(1,k)
   50 continue
c
      if (n .eq. 1) go to 200
c     :::::::::: recover and apply the householder matrices ::::::::::
      do 140 i = 2, n
         l = i - 1
         h = ai(i,i)
         if (h .eq. 0.0d+0) go to 140
c
         do 130 j = 1, m
            s = 0.0d+0
            si = 0.0d+0
c
            do 110 k = 1, l
               s = s + ar(i,k) * zr(k,j) - ai(i,k) * zi(k,j)
               si = si + ar(i,k) * zi(k,j) + ai(i,k) * zr(k,j)
  110       continue
c     :::::::::: double divisions avoid possible underflow ::::::::::
            s = (s / h) / h
            si = (si / h) / h
c
            do 120 k = 1, l
               zr(k,j) = zr(k,j) - s * ar(i,k) - si * ai(i,k)
               zi(k,j) = zi(k,j) - si * ar(i,k) + s * ai(i,k)
  120       continue
c
  130    continue
c
  140 continue
c
  200 return
c     :::::::::: last card of htribk ::::::::::
      end
