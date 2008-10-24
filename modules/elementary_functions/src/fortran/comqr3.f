      subroutine comqr3(nm,n,low,igh,hr,hi,wr,wi,zr,zi,ierr,job)
c
      integer i,j,l,n,en,ll,nm,igh,ip1,
     x        itn,its,low,lp1,enm1,iend,ierr
      double precision hr(nm,n),hi(nm,n),wr(n),wi(n),zr(nm,n),zi(nm,n)
      double precision si,sr,ti,tr,xi,xr,yi,yr,zzi,zzr,norm
      double precision pythag
c
c!originator
c     this subroutine is a translation of a unitary analogue of the
c     algol procedure  comlr2, num. math. 16, 181-204(1970) by peters
c     and wilkinson.
c     handbook for auto. comp., vol.ii-linear algebra, 372-395(1971).
c     the unitary analogue substitutes the qr algorithm of francis
c     (comp. jour. 4, 332-345(1962)) for the lr algorithm.
c
c     modified by  c. moler
c!purpose
c     this subroutine finds the eigenvalues of a complex upper 
c     hessenberg matrix by the qr method. The unitary transformation
c     can also be accumulated if  corth  has been used to reduce
c     this general matrix to hessenberg form.
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c     MODIFICATION OF EISPACK COMQR+COMQR2 
c        1. job parameter added 
c        2. code concerning eigenvector computation deleted
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c!calling sequence
c      subroutine comqr3(nm,n,low,igh,hr,hi,wr,wi,zr,zi,ierr
c     *                 ,job)
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
c          reduction by  corth, if performed.  
c
c        zr and zi contain the real and imaginary parts,respectively
c           of the unitary similarity used to put h on hessenberg form
c           or a unitary matrix ,if vectors are desired
c
c       job indicate the job to be performed: job=xy
c           if y=0 no accumulation of the unitary transformation
c           if y=1 transformation  accumulated in z
c
c     on output.
c     the upper hessenberg portions of hr and hi have been destroyed
c
c
c        wr and wi contain the real and imaginary parts,
c          respectively, of the eigenvalues.  if an error
c          exit is made, the eigenvalues should be correct
c          for indices ierr+1,...,n.
c
c        zr and zi contain the real and imaginary parts,
c          respectively, of the eigenvectors.  the eigenvectors
c          are unnormalized.  if an error exit is made, none of
c          the eigenvectors has been found.
c
c        ierr is set to
c          zero       for normal return,
c          j          if the j-th eigenvalue has not been
c                     determined after a total of 30*n iterations.

c
c     questions and comments should be directed to b. s. garbow,
c     applied mathematics division, argonne national laboratory
c
c!auxiliary routines
c     pythag
c!
c     ------------------------------------------------------------------
c
      ierr = 0
c*****
      jx=job/10
      jy=job-10*jx
c
c     .......... create real subdiagonal elements ..........
      iend=igh-low-1
      if(iend.lt.0) goto 180
  150 l = low + 1
c
      do 170 i = l, igh
         ll = min(i+1,igh)
         if (hi(i,i-1) .eq. 0.0d+0) go to 170
         norm = pythag(hr(i,i-1),hi(i,i-1))
         yr = hr(i,i-1)/norm
         yi = hi(i,i-1)/norm
         hr(i,i-1) = norm
         hi(i,i-1) = 0.0d+0
c
         do 155 j = i, n
            si = yr*hi(i,j) - yi*hr(i,j)
            hr(i,j) = yr*hr(i,j) + yi*hi(i,j)
            hi(i,j) = si
  155    continue
c
         do 160 j = 1, ll
            si = yr*hi(j,i) + yi*hr(j,i)
            hr(j,i) = yr*hr(j,i) - yi*hi(j,i)
            hi(j,i) = si
  160    continue
c*****
         if (jy .eq. 0) go to 170
c*****
         do 165 j = low, igh
            si = yr*zi(j,i) + yi*zr(j,i)
            zr(j,i) = yr*zr(j,i) - yi*zi(j,i)
            zi(j,i) = si
  165    continue
c
  170 continue
c     .......... store roots isolated by cbal ..........
c
  180 do 200 i = 1, n
         if (i .ge. low .and. i .le. igh) go to 200
         wr(i) = hr(i,i)
         wi(i) = hi(i,i)
  200 continue
c
  210 continue
      en = igh
      tr = 0.0d+0
      ti = 0.0d+0
      itn = 30*n
c     .......... search for next eigenvalue ..........
  220 if (en .lt. low) go to 1001
      its = 0
      enm1 = en - 1
c     .......... look for single small sub-diagonal element
c                for l=en step -1 until low do -- ..........
  240 do 260 ll = low, en
         l = en + low - ll
         if (l .eq. low) go to 300
c*****
         xr = abs(hr(l-1,l-1)) + abs(hi(l-1,l-1)
     x             + abs(hr(l,l)) +abs(hi(l,l)))
         yr = xr + abs(hr(l,l-1))
         if (xr .eq. yr) go to 300
c*****
  260 continue
c     .......... form shift ..........
  300 if (l .eq. en) go to 660
      if (itn .eq. 0) go to 1000
      if (its .eq. 10 .or. its .eq. 20) go to 320
      sr = hr(en,en)
      si = hi(en,en)
      xr = hr(enm1,en)*hr(en,enm1)
      xi = hi(enm1,en)*hr(en,enm1)
      if (xr .eq. 0.0d+0 .and. xi .eq. 0.0d+0) go to 340
      yr = (hr(enm1,enm1) - sr)/2.0d+0
      yi = (hi(enm1,enm1) - si)/2.0d+0
      call wsqrt(yr**2-yi**2+xr,2.0d+0*yr*yi+xi,zzr,zzi)
      if (yr*zzr + yi*zzi .ge. 0.0d+0) go to 310
      zzr = -zzr
      zzi = -zzi
  310 call cdiv(xr,xi,yr+zzr,yi+zzi,zzr,zzi)
      sr = sr - zzr
      si = si - zzi
      go to 340
c     .......... form exceptional shift ..........
  320 sr = abs(hr(en,enm1)) + abs(hr(enm1,en-2))
      si = 0.0d+0
c
  340 do 360 i = low, en
         hr(i,i) = hr(i,i) - sr
         hi(i,i) = hi(i,i) - si
  360 continue
c
      tr = tr + sr
      ti = ti + si
      its = its + 1
      itn = itn - 1
c     .......... reduce to triangle (rows) ..........
      lp1 = l + 1
c
      do 500 i = lp1, en
         sr = hr(i,i-1)
         hr(i,i-1) = 0.0d+0
         norm = pythag(pythag(hr(i-1,i-1),hi(i-1,i-1)),sr)
         xr = hr(i-1,i-1)/norm
         wr(i-1) = xr
         xi = hi(i-1,i-1)/norm
         wi(i-1) = xi
         hr(i-1,i-1) = norm
         hi(i-1,i-1) = 0.0d+0
         hi(i,i-1) = sr/norm
c
         do 490 j = i, n
            yr = hr(i-1,j)
            yi = hi(i-1,j)
            zzr = hr(i,j)
            zzi = hi(i,j)
            hr(i-1,j) = xr*yr + xi*yi + hi(i,i-1)*zzr
            hi(i-1,j) = xr*yi - xi*yr + hi(i,i-1)*zzi
            hr(i,j) = xr*zzr - xi*zzi - hi(i,i-1)*yr
            hi(i,j) = xr*zzi + xi*zzr - hi(i,i-1)*yi
  490    continue
c
  500 continue
c
      si = hi(en,en)
      if (si .eq. 0.0d+0) go to 540
      norm = pythag(hr(en,en),si)
      sr = hr(en,en)/norm
      si = si/norm
      hr(en,en) = norm
      hi(en,en) = 0.0d+0
      if (en .eq. n) go to 540
      ip1 = en + 1
c
      do 520 j = ip1, n
         yr = hr(en,j)
         yi = hi(en,j)
         hr(en,j) = sr*yr + si*yi
         hi(en,j) = sr*yi - si*yr
  520 continue
c     .......... inverse operation (columns) ..........
  540 do 600 j = lp1, en
         xr = wr(j-1)
         xi = wi(j-1)
c
         do 580 i = 1, j
            yr = hr(i,j-1)
            yi = 0.0d+0
            zzr = hr(i,j)
            zzi = hi(i,j)
            if (i .eq. j) go to 560
            yi = hi(i,j-1)
            hi(i,j-1) = xr*yi + xi*yr + hi(j,j-1)*zzi
  560       hr(i,j-1) = xr*yr - xi*yi + hi(j,j-1)*zzr
            hr(i,j) = xr*zzr + xi*zzi - hi(j,j-1)*yr
            hi(i,j) = xr*zzi - xi*zzr - hi(j,j-1)*yi
  580    continue
c*****
         if (jy .eq. 0) go to 600
c*****
         do 590 i = low, igh
            yr = zr(i,j-1)
            yi = zi(i,j-1)
            zzr = zr(i,j)
            zzi = zi(i,j)
            zr(i,j-1) = xr*yr - xi*yi + hi(j,j-1)*zzr
            zi(i,j-1) = xr*yi + xi*yr + hi(j,j-1)*zzi
            zr(i,j) = xr*zzr + xi*zzi - hi(j,j-1)*yr
            zi(i,j) = xr*zzi - xi*zzr - hi(j,j-1)*yi
  590    continue
c
  600 continue
c
      if (si .eq. 0.0d+0) go to 240
c
      do 630 i = 1, en
         yr = hr(i,en)
         yi = hi(i,en)
         hr(i,en) = sr*yr - si*yi
         hi(i,en) = sr*yi + si*yr
  630 continue
c*****
      if (jy .eq. 0) go to 240
c*****
      do 640 i = low, igh
         yr = zr(i,en)
         yi = zi(i,en)
         zr(i,en) = sr*yr - si*yi
         zi(i,en) = sr*yi + si*yr
  640 continue
c
      go to 240
c     .......... a root found ..........
  660 hr(en,en) = hr(en,en) + tr
      wr(en) = hr(en,en)
      hi(en,en) = hi(en,en) + ti
      wi(en) = hi(en,en)
      en = enm1
      go to 220
c     .......... all roots found.  ..........

c      go to 1001
c
c     .......... set error -- no convergence to an
c                eigenvalue after 30 iterations ..........
 1000 ierr = en
 1001 return
      end
