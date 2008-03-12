      subroutine hqror2(nm,n,low,igh,h,wr,wi,z,ierr,job)
c
      integer i,j,k,l,m,n,en,ll,mm,na,nm,
     x        igh,itn,its,low,mp2,enm2,ierr,job
      double precision h(nm,n),wr(n),wi(n),z(nm,n)
      double precision p,q,r,s,t,w,x,y,zz,norm,tst1,tst2,machep,dlamch
      logical notlas
c
c     this subroutine is a translation of the algol procedure hqr2,
c     num. math. 16, 181-204(1970) by peters and wilkinson.
c     handbook for auto. comp., vol.ii-linear algebra, 372-395(1971).
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c     MODIFICATIONS WRT EISPACK VERSION
c     ---------------------------------
c       1. 1x1 and 2x2 diagonal blocks are clearly isolated by 
c          forcing subdiagonal entries to zero 
c       2. Merging of hqr/hqr2 driven by a job parameter
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c     This subroutine finds the eigenvalues of a real upper 
c     hessenberg matrix by the qr method. In addition, the 
c     orthogonal transformation leading to the Schur form is
c     accumulated
c
c     on input
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement.
c
c        n is the order of the matrix.
c
c        low and igh are integers determined by the balancing
c          subroutine  balanc.  if  balanc  has not been used,
c          set low=1, igh=n.
c
c        h contains the upper hessenberg matrix.
c
c        z contains the transformation matrix produced by  eltran
c          after the reduction by  elmhes, or by  ortran  after the
c          reduction by  orthes, if performed.  if the eigenvectors
c          of the hessenberg matrix are desired, z must contain the
c          identity matrix.
c
c        job  has the decimal decomposition xy;
c            if x=0 hqror2 compute eigen-decomposition of h
c            if x=1 hqror2 computes schur decomposition of h
c            if x=2 eigenvalues are computed via schur decomposition
c            if y=0 coordinate transformation is not accumulated
c            if y=1 coordinate transformation is accumulated
c        
c
c     on output
c
c        h contains the Schur form
c
c        wr and wi contain the real and imaginary parts,
c          respectively, of the eigenvalues.  the eigenvalues
c          are unordered except that complex conjugate pairs
c          of values appear consecutively with the eigenvalue
c          having the positive imaginary part first.  if an
c          error exit is made, the eigenvalues should be correct
c          for indices ierr+1,...,n.
c
c        z contains the orthogonal transformation to the real schur
c          form. If an error exit is made, z may be incorrect.
c
c        ierr is set to
c          zero       for normal return,
c          j          if the limit of 30*n iterations is exhausted
c                     while the j-th eigenvalue is being sought.
c
c     calls cdiv for complex division.
c
c     questions and comments should be directed to burton s. garbow,
c     mathematics and computer science div, argonne national laboratory
c
c     this version dated august 1983.
c
c     ------------------------------------------------------------------
c
      jx=job/10
      jy=job-10*jx
c
      machep=dlamch('p')
c
      ierr = 0
      norm = 0.0d0
      k = 1
c     .......... store roots isolated by balanc
c                and compute matrix norm ..........
      do 50 i = 1, n
c
         do 40 j = k, n
   40    norm = norm + dabs(h(i,j))
c
         k = i
         if(jx .eq. 1) goto 50
         if (i .ge. low .and. i .le. igh) go to 50
         wr(i) = h(i,i)
         wi(i) = 0.0d0
   50 continue
c
      en = igh
      t = 0.0d0
      itn = 30*n
c     .......... search for next eigenvalues ..........
   60 if (en .lt. low) go to 340
      its = 0
      na = en - 1
      enm2 = na - 1
c     .......... look for single small sub-diagonal element
c                for l=en step -1 until low do -- ..........
   70 do 80 ll = low, en
         l = en + low - ll
         if (l .eq. low) go to 100
         s = dabs(h(l-1,l-1)) + dabs(h(l,l))
         if (s .eq. 0.0d0) s = norm
         tst1 = s
         tst2 = tst1 + dabs(h(l,l-1))
         if (tst2 .eq. tst1) go to 100
   80 continue
c     .......... form shift ..........
  100 x = h(en,en)
      if (l .eq. en) go to 270
      y = h(na,na)
      w = h(en,na) * h(na,en)
      if (l .eq. na) go to 280
      if (itn .eq. 0) go to 1000
      if (its .ne. 10 .and. its .ne. 20) go to 130
c     .......... form exceptional shift ..........
      t = t + x
c
      do 120 i = low, en
  120 h(i,i) = h(i,i) - x
c
      s = dabs(h(en,na)) + dabs(h(na,enm2))
      x = 0.75d0 * s
      y = x
      w = -0.4375d0 * s * s
  130 its = its + 1
      itn = itn - 1
c     .......... look for two consecutive small
c                sub-diagonal elements.
c                for m=en-2 step -1 until l do -- ..........
      do 140 mm = l, enm2
         m = enm2 + l - mm
         zz = h(m,m)
         r = x - zz
         s = y - zz
         p = (r * s - w) / h(m+1,m) + h(m,m+1)
         q = h(m+1,m+1) - zz - r - s
         r = h(m+2,m+1)
         s = dabs(p) + dabs(q) + dabs(r)
         p = p / s
         q = q / s
         r = r / s
         if (m .eq. l) go to 150
         tst1 = dabs(p)*(dabs(h(m-1,m-1)) + dabs(zz) + dabs(h(m+1,m+1)))
         tst2 = tst1 + dabs(h(m,m-1))*(dabs(q) + dabs(r))
         if (tst2 .eq. tst1) go to 150
  140 continue
c
  150 mp2 = m + 2
c
      do 160 i = mp2, en
         h(i,i-2) = 0.0d0
         if (i .eq. mp2) go to 160
         h(i,i-3) = 0.0d0
  160 continue
c     .......... double qr step involving rows l to en and
c                columns m to en ..........
      do 260 k = m, na
         notlas = k .ne. na
         if (k .eq. m) go to 170
         p = h(k,k-1)
         q = h(k+1,k-1)
         r = 0.0d0
         if (notlas) r = h(k+2,k-1)
         x = dabs(p) + dabs(q) + dabs(r)
         if (x .eq. 0.0d0) go to 260
         p = p / x
         q = q / x
         r = r / x
  170    s = dsign(dsqrt(p*p+q*q+r*r),p)
         if (k .eq. m) go to 180
         h(k,k-1) = -s * x
         go to 190
  180    if (l .ne. m) h(k,k-1) = -h(k,k-1)
  190    p = p + s
         x = p / s
         y = q / s
         zz = r / s
         q = q / p
         r = r / p
         if (notlas) go to 225
c     .......... row modification ..........
         do 200 j = k, n
            p = h(k,j) + q * h(k+1,j)
            h(k,j) = h(k,j) - p * x
            h(k+1,j) = h(k+1,j) - p * y
  200    continue
c
         j = min0(en,k+3)
c     .......... column modification ..........
         do 210 i = 1, j
            p = x * h(i,k) + y * h(i,k+1)
            h(i,k) = h(i,k) - p
            h(i,k+1) = h(i,k+1) - p * q
  210    continue
         if(jy .eq. 1) then
c     .......... accumulate transformations ..........
         do 220 i = low, igh
            p = x * z(i,k) + y * z(i,k+1)
            z(i,k) = z(i,k) - p
            z(i,k+1) = z(i,k+1) - p * q
  220    continue
         endif
         go to 255
  225    continue
c     .......... row modification ..........
         do 230 j = k, n
            p = h(k,j) + q * h(k+1,j) + r * h(k+2,j)
            h(k,j) = h(k,j) - p * x
            h(k+1,j) = h(k+1,j) - p * y
            h(k+2,j) = h(k+2,j) - p * zz
  230    continue
c
         j = min0(en,k+3)
c     .......... column modification ..........
         do 240 i = 1, j
            p = x * h(i,k) + y * h(i,k+1) + zz * h(i,k+2)
            h(i,k) = h(i,k) - p
            h(i,k+1) = h(i,k+1) - p * q
            h(i,k+2) = h(i,k+2) - p * r
  240    continue
         if(jy .eq. 1) then
c     .......... accumulate transformations ..........
         do 250 i = low, igh
            p = x * z(i,k) + y * z(i,k+1) + zz * z(i,k+2)
            z(i,k) = z(i,k) - p
            z(i,k+1) = z(i,k+1) - p * q
            z(i,k+2) = z(i,k+2) - p * r
  250    continue
         endif
  255    continue
c
  260 continue
c
      go to 70
c     .......... one root found ..........
  270 h(en,en) = x + t

ccccc ADDED TO MARK BLOCK SEPARATION BY HARD ZEROS
      if(en+1.le.n) h(en+1,en)=0.0d0
cccccccccccccccccccccccccccccccccccccccccccccccccc
      if (jx.ne.1) then
      wr(en) = h(en,en)
      wi(en) = 0.0d0
      endif
      en = na
      go to 60
c     .......... two roots found ..........
  280 p = (y - x) / 2.0d0
      q = p * p + w
      zz = dsqrt(dabs(q))
      h(en,en) = x + t
      x = h(en,en)
      h(na,na) = y + t
      if (q .lt. 0.0d0) go to 320
c     .......... real pair ..........
      zz = p + dsign(zz,p)
      if (jx.ne.1) then 
      wr(na) = x + zz
      wr(en) = wr(na)
      if (zz .ne. 0.0d0) wr(en) = x - w / zz
      wi(na) = 0.0d0
      wi(en) = 0.0d0
      endif
      x = h(en,na)
      s = dabs(x) + dabs(zz)
      p = x / s
      q = zz / s
      r = dsqrt(p*p+q*q)
      p = p / r
      q = q / r
c     .......... row modification ..........
      do 290 j = na, n
         zz = h(na,j)
         h(na,j) = q * zz + p * h(en,j)
         h(en,j) = q * h(en,j) - p * zz
  290 continue
c     .......... column modification ..........
      do 300 i = 1, en
         zz = h(i,na)
         h(i,na) = q * zz + p * h(i,en)
         h(i,en) = q * h(i,en) - p * zz
  300 continue
      if(jy .eq. 1) then
c     .......... accumulate transformations ..........
      do 310 i = low, igh
         zz = z(i,na)
         z(i,na) = q * zz + p * z(i,en)
         z(i,en) = q * z(i,en) - p * zz
  310 continue
      endif

ccccc ADDED TO MARK BLOCK SEPARATION BY HARD ZEROS
      h(en,na)=0.0d0
      if(en+1.le.n) h(en+1,en)=0.0d0
cccccccccccccccccccccccccccccccccccccccccccccccccc

c
      go to 330
c     .......... complex pair ..........
  320 if (jx.ne.1) then 
      wr(na) = x + p
      wr(en) = x + p
      wi(na) = zz
      wi(en) = -zz
      endif

ccccc ADDED TO MARK BLOCK SEPARATION BY HARD ZEROS
      if(en+1.le.n) h(en+1,en)=0.0d0
cccccccccccccccccccccccccccccccccccccccccccccccccc

  330 en = enm2
      go to 60

  340 if(jx.ne.0) goto 1001
      if (norm .eq. 0.0d+0) go to 1001
c     :::::::::: for en=n step -1 until 1 do -- ::::::::::
      do 800 nn = 1, n
         en = n + 1 - nn
         p = wr(en)
         q = wi(en)
         na = en - 1
         q=q+1.0d+0
         CRES=(q-1.0d+0)
         if (CRES .lt. 0) then
            goto 710
         elseif (CRES .eq. 0) then
            goto 600
         else
            goto 800
         endif
c     :::::::::: real vector ::::::::::
  600    m = en
         h(en,en) = 1.0d+0
         if (na .eq. 0) go to 800
c     :::::::::: for i=en-1 step -1 until 1 do -- ::::::::::
         do 700 ii = 1, na
            i = en - ii
            w = h(i,i) - p
            r = h(i,en)
            if (m .gt. na) go to 620
c
            do 610 j = m, na
  610       r = r + h(i,j) * h(j,en)
c
  620       if (wi(i) .ge. 0.0d+0) go to 630
            zz = w
            s = r
            go to 700
  630       m = i
            if (wi(i) .ne. 0.0d+0) go to 640
            t = w
            if (w .eq. 0.0d+0) t = machep * norm
            h(i,en) = -r / t
            go to 700
c     :::::::::: solve real equations ::::::::::
  640       x = h(i,i+1)
            y = h(i+1,i)
            q = (wr(i) - p) * (wr(i) - p) + wi(i) * wi(i)
            t = (x * s - zz * r) / q
            h(i,en) = t
            if (abs(x) .le. abs(zz)) go to 650
            h(i+1,en) = (-r - w * t) / x
            go to 700
  650       h(i+1,en) = (-s - y * t) / zz
  700    continue
c     :::::::::: end real vector ::::::::::
         go to 800
c     :::::::::: complex vector ::::::::::
  710    m = na
c     :::::::::: last vector component chosen imaginary so that
c                eigenvector matrix is triangular ::::::::::
         if (abs(h(en,na)) .le. abs(h(na,en))) go to 720
         h(na,na) = q / h(en,na)
         h(na,en) = -(h(en,en) - p) / h(en,na)
         go to 730
  720 z3r=h(na,na)-p
      z3=z3r*z3r+q*q
      h(na,na)=-h(na,en)*q/z3
      h(na,en)=-h(na,en)*z3r/z3
  730    h(en,na) = 0.0d+0
         h(en,en) = 1.0d+0
         enm2 = na - 1
         if (enm2 .eq. 0) go to 800
c     :::::::::: for i=en-2 step -1 until 1 do -- ::::::::::
         do 790 ii = 1, enm2
            i = na - ii
            w = h(i,i) - p
            ra = 0.0d+0
            sa = h(i,en)
c
            do 760 j = m, na
               ra = ra + h(i,j) * h(j,na)
               sa = sa + h(i,j) * h(j,en)
  760       continue
c
            if (wi(i) .ge. 0.0d+0) go to 770
            zz = w
            r = ra
            s = sa
            go to 790
  770       m = i
            if (wi(i) .ne. 0.0d+0) go to 780
        z3=w*w+q*q
        z3r=-ra*w-sa*q
        z3i=ra*q-sa*w
        h(i,na)=z3r/z3
        h(i,en)=z3i/z3
            go to 790
c     :::::::::: solve complex equations ::::::::::
  780       x = h(i,i+1)
            y = h(i+1,i)
            vr = (wr(i) - p) * (wr(i) - p) + wi(i) * wi(i) - q * q
            vi = (wr(i) - p) * 2.0d+0 * q
            if (vr .eq. 0.0d+0 .and. vi .eq. 0.0d+0) vr = machep * norm
     x       * (abs(w) + abs(q) + abs(x) + abs(y) + abs(zz))
            z3r=x*r-zz*ra+q*sa
            z3i=x*s-zz*sa-q*ra
            z3=vr*vr+vi*vi
            h(i,na)=(z3r*vr+z3i*vi)/z3
            h(i,en)=(-z3r*vi+z3i*vr)/z3
            if (abs(x) .le. abs(zz) + abs(q)) go to 785
            h(i+1,na) = (-ra - w * h(i,na) + q * h(i,en)) / x
            h(i+1,en) = (-sa - w * h(i,en) - q * h(i,na)) / x
            go to 790
  785 z3r=-r-y*h(i,na)
      z3i=-s-y*h(i,en)
      z3=zz*zz+q*q
      h(i+1,na)=(z3r*zz+z3i*q)/z3
      h(i+1,en)=(-z3r*q+z3i*zz)/z3
  790    continue
c     :::::::::: end complex vector ::::::::::
  800 continue
c     :::::::::: end back substitution.
      if(jy.eq.0) goto 1001
c                vectors of isolated roots ::::::::::
      do 840 i = 1, n
         if (i .ge. low .and. i .le. igh) go to 840
c
         do 820 j = i, n
  820    z(i,j) = h(i,j)
c
  840 continue
c     :::::::::: multiply by transformation matrix to give
c                vectors of original full matrix.
c                for j=n step -1 until low do -- ::::::::::
      do 880 jj = low, n
         j = n + low - jj
         m = min(j,igh)
c
         do 880 i = low, igh
            zz = 0.0d+0
c
            do 860 k = low, m
  860       zz = zz + z(i,k) * h(k,j)
c
            z(i,j) = zz
  880 continue
c
      go to 1001
c     .......... set error -- all eigenvalues have not
c                converged after 30*n iterations ..........
 1000 ierr = en
 1001 return
      end



      subroutine cdiv(ar,ai,br,bi,cr,ci)
      double precision ar,ai,br,bi,cr,ci
c
c     complex division, (cr,ci) = (ar,ai)/(br,bi)
c
      double precision s,ars,ais,brs,bis
      s = dabs(br) + dabs(bi)
      ars = ar/s
      ais = ai/s
      brs = br/s
      bis = bi/s
      s = brs**2 + bis**2
      cr = (ars*brs + ais*bis)/s
      ci = (ais*brs - ars*bis)/s
      return
      end

