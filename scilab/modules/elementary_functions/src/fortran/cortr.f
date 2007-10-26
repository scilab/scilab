      subroutine cortr(nm,n,low,igh,hr,hi,ortr,orti,zr,zi)
c!purpose
c     cortr accumulate the  unitary similarities performed by corth
c!calling sequence
c
c      subroutine cortr(nm,n,low,igh,hr,hi,ortr,orti,zr,zi)
c
c     on input.
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement.
c
c        n is the order of the matrix.
c
c        low and igh are integers determined by the balancing
c          subroutine  cbal.  if  cbal  has not been used,
c          set low=1, igh=n.
c
c        hr and hi contain the real and imaginary parts,
c          respectively, of the complex upper hessenberg matrix.
c          their lower triangles below the subdiagonal contain further
c          information about the transformations which were used in the
c          reduction by  corth, if performed.  if the eigenvectors of
c          the hessenberg matrix are desired, these elements may be
c          arbitrary.
c
c
c     on output.
c
c        zr and zi contain the real and imaginary parts,
c          respectivelyof the tranformations performed
c
c!
      double precision hr(nm,n),hi(nm,n),zr(nm,n),zi(nm,n),ortr(igh)
      double precision orti(igh),sr,si,norm
c     .......... initialize eigenvector matrix ..........
      do 100 i = 1, n
c
         do 100 j = 1, n
            zr(i,j) = 0.0d+0
            zi(i,j) = 0.0d+0
            if (i .eq. j) zr(i,j) = 1.0d+0
  100 continue
c     .......... form the matrix of accumulated transformations
c                from the information left by corth ..........
      iend = igh - low - 1
      if (iend .le. 0) then
         goto 150
      else
         goto 105
      endif
c     .......... for i=igh-1 step -1 until low+1 do -- ..........
  105 do 140 ii = 1, iend
         i = igh - ii
cx         if (ortr(i) .eq. 0.0d+0 .and. orti(i) .eq. 0.0d+0) go to 140
cx         if (hr(i,i-1).eq.0.0d+0 .and. hi(i,i-1).eq.0.0d+0) go to 140
c     .......... norm below is negative of h formed in corth ..........
         norm = hr(i,i-1)*ortr(i) + hi(i,i-1)*orti(i)
         if (norm.eq.0.0d+00) goto 140
         ip1 = i + 1
c
         do 110 k = ip1, igh
            ortr(k) = hr(k,i-1)
            orti(k) = hi(k,i-1)
  110    continue
c
         do 130 j = i, igh
            sr = 0.0d+0
            si = 0.0d+0
c
            do 115 k = i, igh
               sr = sr + ortr(k)*zr(k,j) + orti(k)*zi(k,j)
               si = si + ortr(k)*zi(k,j) - orti(k)*zr(k,j)
  115       continue
c
            sr = sr/norm
            si = si/norm
c
            do 120 k = i, igh
               zr(k,j) = zr(k,j) + sr*ortr(k) - si*orti(k)
               zi(k,j) = zi(k,j) + sr*orti(k) + si*ortr(k)
  120       continue
c
  130    continue
c
  140 continue
c*****
  150 return
      end
