c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine fprf2(iflag,ntot,nv,io,zero,s2,eps,al,imp,u,eta,mm1,jc,
     &                 ic,r,a,e,rr,xpr,y,w1,w2)
c
      implicit double precision (a-h,o-z)
      common /fprf2c/ u1,nc
C         the dimension is mm1*mm1 for r
      dimension al(ntot), jc(mm1), ic(mm1), a(mm1), e(mm1), r(*),
     &          rr(mm1), xpr(mm1), y(mm1), w1(mm1), w2(mm1)
      dimension i5(1), d3(1), d4(1)
C
C     *****  on entry  *****
C
C       iflag=0-1  initialize on one subgradient (mu in)
C
C       iflag=2    "     "     "     "     "     "     "
C                  and strive to enter by priority the
C                  points of the previous corral at the
C                  beginning of the iterations.
C
C       iflag=3    initialize on the previous projection
C                  (with its corresponding corral)
C
C
C      *****  on exit  *****
C
C                  iflag=0    normal end
C
C                        1    old solution is already optimal
C
C                        2    constraints non feasible
C
C                        3    trying to enter a variable
C                             that is already in the corral
C
C                        4    starting to loop
C
C
C
C
C      imp > 5    one prints final information
C
C
C      imp > 6    one prints information at each iteration
C
C
C      imp > 7    one prints also
C
C                - at each iteration the choleski matrix
C                - and the initial information such as (pi,pj) ...
C
C
C
C
C
C               ****   begin   ****
C
C              prepare various data
C
C
      iterpr = 0
      nt1 = ntot + 1
      itmax = 10 * ntot
      deps = eps
      incr = 0
      k00 = 1
      w1s = 0.d0
      w2s = 0.d0
      w12s = 0.d0
      gama = .99d0
      dzero = 10.d0 * zero
C                     initial printouts
      if (imp .gt. 7) call n1fc1o(io,21,nt1,mm1,i3,i4,i5,deps,d2,a,r)
C
C                     initial point
C
 100  if (iflag .ne. 3) goto 110
      if (imp .gt. 6) call n1fc1o(io,22,nv,i2,i3,i4,jc,d1,d2,d3,d4)
      j0 = nt1
      ps = u1 * (a(nt1)-deps)
      ment = (nt1-1) * mm1
      do 103 k = 1,nv
        jk = ment + jc(k)
 103  ps = ps + xpr(k)*r(jk)
      if (ps .lt. s2) goto 107
      if (imp .gt. 0) call n1fc1o(io,23,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      iflag = 1
      return
 107  nv = nv + 1
      nc = nc + 1
      jc(nv) = j0
      iterpr = 1
      goto 300
 110  if (iflag .le. 1) goto 140
C        save the corral of previous call
      do 120 i = 1,nt1
 120  ic(i) = 0
      do 130 k = 1,nv
        jk = jc(k)
 130  ic(jk) = 1
      ic(nt1) = 1
C           initialize with one feasible gradient
 140  jc(1) = 1
      nv = 2
      nc = 1
      jc(2) = 0
      do 150 j = 2,nt1
        if (a(j) .gt. deps) goto 150
        jc(2) = j
 150  continue
      if (jc(2) .gt. 0) goto 160
      if (imp .gt. 0) call n1fc1o(io,24,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      iflag = 2
      return
 160  j = jc(2)
      rr(1) = 1.d0
      jj = (j-1)*mm1 + j
      ps = 1.d0 + r(jj)
      if (ps .gt. 0.d0) goto 170
      iflag = 3
      return
 170  rr(2) = dsqrt(ps)
      r(2) = a(j)
      do 180 i = 1,nt1
 180  xpr(i) = 0.d0
      xpr(1) = deps - a(j)
      xpr(2) = 1.d0
      u1 = 0.d0
      u2 = -r(jj)
      if (imp .gt. 6) call n1fc1o(io,25,j,i2,i3,i4,i5,d1,d2,d3,d4)
C
C                 stopping criterion
C
 200  iterpr = iterpr + 1
      if (imp .gt. 6) call n1fc1o(io,26,nv,i2,i3,i4,i5,d1,d2,d3,xpr)
      if (iterpr .le. itmax) goto 205
      if (imp .gt. 0) call n1fc1o(io,27,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      iflag = 4
      return
 205  s2 = (-deps)*u1 - u2
      if (s2 .le. eta) goto 900
      sp = gama * s2
C                    first compute all the tests,
C            and test with the corral of previous call
      j0 = 0
      do 220 j = 2,nt1
        ps = u1 * (a(j)-deps)
        do 210 k = 1,nv
          jj = jc(k)
          if (jj .eq. 1) goto 210
          j1 = max0(j,jj)
          j2 = min0(j,jj)
          jj = (j1-1)*mm1 + j2
          ps = ps + xpr(k)*r(jj)
 210    continue
        y(j) = ps
        if (iflag .ne. 2) goto 220
        if (ic(j) .ne. 1) goto 220
        if (ps .ge. sp) goto 220
        j0 = j
        sp = ps
 220  continue
      if (j0 .eq. 0) goto 240
      if (sp .ge. gama*s2) goto 240
      ps1 = dabs(u1*(deps-a(j0)))
      do 230 k = 1,nv
        j = jc(k)
        if (j .eq. j0) goto 240
        if (j .eq. 1) goto 230
        j1 = max0(j0,j)
        j2 = min0(j0,j)
        jj = (j1-1)*mm1 + j2
        ps1 = ps1 + xpr(k)*dabs(u1*(2.d0*deps-a(j))+2.d0*y(j)-r(jj))
 230  continue
      ps1 = ps1 * 1000.d0 * dzero
      if (sp .gt. s2-ps1) goto 240
      ic(j0) = 0
      goto 280
C                     now the remaining ones
 240  j0 = 0
      sp = gama * s2
      do 260 j = 2,nt1
        if (iflag.eq.2 .and. ic(j).eq.1) goto 260
        if (y(j) .ge. sp) goto 260
        sp = y(j)
        j0 = j
 260  continue
      if (j0 .eq. 0) goto 290
      ps1 = dabs(u1*(deps-a(j0)))
      do 270 k = 1,nv
        j = jc(k)
        if (j .eq. 1) goto 270
        j1 = max0(j0,j)
        j2 = min0(j0,j)
        jj = (j1-1)*mm1 + j2
        ps1 = ps1 + xpr(k)*dabs(u1*(2.d0*deps-a(j))+2.d0*y(j)-r(jj))
 270  continue
      ps1 = ps1 * 1000.d0 * dzero
      if (sp .gt. s2-ps1) goto 290
 280  nc = nc + 1
      nv = nv + 1
      jc(nv) = j0
      if (imp .gt. 6) call n1fc1o(io,28,j0,i2,i3,i4,i5,s2,sp,d3,d4)
      goto 300
C         first set of optimality conditions satisfied
 290  if (u1 .ge. (-dble(float(nv)))*dzero) goto 900
      j0 = 1
      nv = nv + 1
      jc(nv) = 1
      if (imp .gt. 6) call n1fc1o(io,29,i1,i2,i3,i4,i5,s2,u1,d3,d4)
C
C               augmenting r
C
 300  nv1 = nv - 1
      do 305 k = 1,nv1
        if (jc(k) .ne. j0) goto 305
        if (imp .gt. 0) call n1fc1o(io,30,j0,i2,i3,i4,i5,d1,d2,d3,d4)
        iflag = 3
        return
 305  continue
      j = jc(1)
      j1 = max0(j,j0)
      j2 = min0(j,j0)
      jj = (j1-1)*mm1 + j2
      r(nv) = (a(j)*a(j0)+e(j)*e(j0)+r(jj)) / rr(1)
      ps0 = r(nv) * r(nv)
      if (nv1 .eq. 1) goto 330
      do 320 k = 2,nv1
        j = jc(k)
        j1 = max0(j,j0)
        j2 = min0(j,j0)
        jj = (j1-1)*mm1 + j2
        ps = a(j)*a(j0) + e(j)*e(j0) + r(jj)
        k1 = k - 1
        do 310 kk = 1,k1
          j1 = (kk-1)*mm1 + k
          j2 = (kk-1)*mm1 + nv
 310    ps = ps - r(j1)*r(j2)
        mek = k1*mm1 + nv
        r(mek) = ps / rr(k)
 320  ps0 = ps0 + r(mek)*r(mek)
      jj = (j0-1)*mm1 + j0
      ps0 = a(j0)*a(j0) + e(j0)*e(j0) + r(jj) - ps0
      if (ps0 .gt. 0.d0) goto 330
      iflag = 3
      return
 330  rr(nv) = dsqrt(ps0)
      if (iterpr .le. 1) goto 400
      incr = 1
      k00 = nv
C
C          solving the corral-system
C
 400  k = k00
      if (k .gt. nv) goto 430
      if (imp .gt. 7) call n1fc1o(io,31,nv,mm1,i3,i4,i5,d1,d2,rr,r)
 410  j = jc(k)
      ps1 = a(j)
      ps2 = e(j)
      if (k .eq. 1) goto 420
      k1 = k - 1
      do 415 kk = 1,k1
        jj = (kk-1)*mm1 + k
        ps0 = r(jj)
        ps1 = ps1 - ps0*w1(kk)
 415  ps2 = ps2 - ps0*w2(kk)
 420  ps0 = rr(k)
      w1(k) = ps1 / ps0
      w2(k) = ps2 / ps0
      k = k + 1
      if (k .le. nv) goto 410
C                two-two system
 430  k = 1
      if (incr .eq. 1) k = nv
 440  w1s = w1s + w1(k)*w1(k)
      w2s = w2s + w2(k)*w2(k)
      w12s = w12s + w1(k)*w2(k)
      k = k + 1
      if (k .le. nv) goto 440
      det = w1s*w2s - w12s*w12s
      ps2 = w2s*deps - w12s
      ps1 = w1s - w12s*deps
 450  v1 = ps2 / det
      v2 = ps1 / det
 460  u1 = deps - v1
      u2 = 1.d0 - v2
      if (nv .eq. nc+1) u1 = 0.d0
C                  backward
      y(nv) = (v1*w1(nv)+v2*w2(nv)) / rr(nv)
      if (nv .eq. 1) goto 500
      do 480 l = 2,nv
        k = nv - l + 1
        k1 = k + 1
        ps = v1*w1(k) + v2*w2(k)
        mek = (k-1) * mm1
        do 470 kk = k1,nv
          mej = mek + kk
 470    ps = ps - r(mej)*y(kk)
 480  y(k) = ps / rr(k)
C
C                test for positivity
C
 500  continue
      do 530 k = 1,nv
        if (y(k) .le. 0.d0) goto 550
 530  continue
      do 540 k = 1,nv
 540  xpr(k) = y(k)
      goto 200
C           interpolating between x and y
 550  teta = 0.d0
      k0 = k
      do 560 k = 1,nv
        if (y(k) .ge. 0.d0) goto 560
        ps = y(k) / (y(k)-xpr(k))
        if (teta .ge. ps) goto 560
        teta = ps
        k0 = k
 560  continue
      do 570 k = 1,nv
        ps = teta*xpr(k) + (1.d0-teta)*y(k)
        if (ps .le. dzero) ps = 0.d0
 570  xpr(k) = ps
      if (imp .le. 6) goto 600
      ps1 = 0.d0
      ps2 = 0.d0
      do 580 k = 1,nv
        do 580 kk = 1,nv
          j1 = max0(jc(k),jc(kk))
          j2 = min0(jc(k),jc(kk))
          jj = (j1-1)*mm1 + j2
          ps1 = ps1 + xpr(k)*xpr(kk)*r(jj)
          ps2 = ps2 + y(k)*y(kk)*r(jj)
 580  continue
C
C                  compressing the corral
C
 600  nv = nv - 1
      incr = 0
      k00 = k0
      w1s = 0.d0
      w2s = 0.d0
      w12s = 0.d0
      l = jc(k0)
      if (l .ne. 1) nc = nc - 1
      if (imp .gt. 6) call n1fc1o(io,32,k0,l,i3,i4,i5,y(k0),ps1,ps2,d4)
      if (k0 .gt. nv) goto 400
      k1 = k0 - 1
      do 620 k = k0,nv
        xpr(k) = xpr(k+1)
        if (k0 .eq. 1) goto 620
        do 610 kk = 1,k1
          mek = (kk-1)*mm1 + k
 610    r(mek) = r(mek+1)
 620  jc(k) = jc(k+1)
      xpr(nv+1) = 0.d0
 630  mek = (k0-1)*mm1 + k0 + 1
      ps = r(mek)
      ps12 = rr(k0+1)
      ps0 = dsqrt(ps*ps+ps12*ps12)
      ps = ps / ps0
      ps12 = ps12 / ps0
      rr(k0) = ps0
      if (k0 .eq. nv) goto 400
      k1 = k0 + 1
      mek01 = (k0-1) * mm1
      mek = k0 * mm1
      mekk = mek - mm1
      do 640 k = k1,nv
        j1 = mekk + k
        j2 = mek + k
        r(j1) = ps*r(j1+1) + ps12*r(j2+1)
        if (k .gt. k1) r(j2) = ps2
 640  ps2 = (-ps12)*r(j1+1) + ps*r(j2+1)
      r(j2+1) = ps2
      k0 = k0 + 1
      goto 630
C
C                      *** finished ***
C
 900  iflag = 0
      do 930 j = 1,ntot
 930  al(j) = 0.
      do 940 k = 1,nv
        j = jc(k) - 1
        if (j .ne. 0) al(j) = xpr(k)
 940  continue
      u = u1
      if (imp .le. 5) return
      call n1fc1o(io,34,nc,nv,i3,i4,jc,s2,sp,u1,d4)
      return
      end
