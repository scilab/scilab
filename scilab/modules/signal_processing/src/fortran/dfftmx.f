      subroutine dfftmx(a, b, ntot, n, nspan, isn, m, kt, wt, ck, bt,
     *    sk, np, nfac)
c
      implicit double precision(a-h,o-z)
      dimension a(*), b(*), wt(*), ck(*), bt(*), sk(*), np(*), nfac(*)
c
      inc = abs(isn)
      nt = inc*ntot
      ks = inc*nspan
      rad = atan(1.0d+0)
      s72 = rad/0.6250d+0
      c72 = cos(s72)
      s72 = sin(s72)
      s120 = sqrt(0.750d+0)
      if (isn.gt.0) go to 10
      s72 = -s72
      s120 = -s120
      rad = -rad
      go to 30
c
c scale by 1/n for isn .gt. 0
c
  10  ak = 1.0d+0/dble(n)
      do 20 j=1,nt,inc
        a(j) = a(j)*ak
        b(j) = b(j)*ak
  20  continue
c
  30  kspan = ks
      nn = nt - inc
      jc = ks/n
c
c sin, cos values are reinitialized each lim steps
c
      lim = 32
      klim = lim*jc
      i = 0
      jf = 0
      maxf = m - kt
      maxf = nfac(maxf)
      if (kt.gt.0) maxf = max(nfac(kt),maxf)
c
c compute fourier transform
c
  40  dr = 8.0d+0*dble(jc)/dble(kspan)
      cd = 2.0d+0*sin(0.50d+0*dr*rad)**2
      sd = sin(dr*rad)
      kk = 1
      i = i + 1
      if (nfac(i).ne.2) go to 110
c
c transform for factor of 2 (including rotation factor)
c
      kspan = kspan/2
      k1 = kspan + 2
  50  k2 = kk + kspan
      ak = a(k2)
      bk = b(k2)
      a(k2) = a(kk) - ak
      b(k2) = b(kk) - bk
      a(kk) = a(kk) + ak
      b(kk) = b(kk) + bk
      kk = k2 + kspan
      if (kk.le.nn) go to 50
      kk = kk - nn
      if (kk.le.jc) go to 50
      if (kk.gt.kspan) go to 350
  60  c1 = 1.0d+0- cd
      s1 = sd
      mm = min(k1/2,klim)
      go to 80
  70  ak = c1 - (cd*c1+sd*s1)
      s1 = (sd*c1-cd*s1) + s1
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, substitute
c c1=ak
c
      c1 = 0.50d+0/(ak**2+s1**2) + 0.50d+0
      s1 = c1*s1
      c1 = c1*ak
  80  k2 = kk + kspan
      ak = a(kk) - a(k2)
      bk = b(kk) - b(k2)
      a(kk) = a(kk) + a(k2)
      b(kk) = b(kk) + b(k2)
      a(k2) = c1*ak - s1*bk
      b(k2) = s1*ak + c1*bk
      kk = k2 + kspan
      if (kk.lt.nt) go to 80
      k2 = kk - nt
      c1 = -c1
      kk = k1 - k2
      if (kk.gt.k2) go to 80
      kk = kk + jc
      if (kk.le.mm) go to 70
      if (kk.lt.k2) go to 90
      k1 = k1 + inc + inc
      kk = (k1-kspan)/2 + jc
      if (kk.le.jc+jc) go to 60
      go to 40
  90  s1 = dble((kk-1)/jc)*dr*rad
      c1 = cos(s1)
      s1 = sin(s1)
      mm = min(k1/2,mm+klim)
      go to 80
c
c transform for factor of 3 (optional code)
c
 100  k1 = kk + kspan
      k2 = k1 + kspan
      ak = a(kk)
      bk = b(kk)
      aj = a(k1) + a(k2)
      bj = b(k1) + b(k2)
      a(kk) = ak + aj
      b(kk) = bk + bj
      ak = -0.50d+0*aj + ak
      bk = -0.50d+0*bj + bk
      aj = (a(k1)-a(k2))*s120
      bj = (b(k1)-b(k2))*s120
      a(k1) = ak - bj
      b(k1) = bk + aj
      a(k2) = ak + bj
      b(k2) = bk - aj
      kk = k2 + kspan
      if (kk.lt.nn) go to 100
      kk = kk - nn
      if (kk.le.kspan) go to 100
      go to 290
c
c transform for factor of 4
c
 110  if (nfac(i).ne.4) go to 230
      kspnn = kspan
      kspan = kspan/4
 120  c1 = 1.0d+0
      s1 = 0.0d+0
      mm = min(kspan,klim)
      go to 150
 130  c2 = c1 - (cd*c1+sd*s1)
      s1 = (sd*c1-cd*s1) + s1
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, substitute
c c1=c2
c
      c1 = 0.50d+0/(c2**2+s1**2) + 0.50d+0
      s1 = c1*s1
      c1 = c1*c2
 140  c2 = c1**2 - s1**2
      s2 = c1*s1*2.0
      c3 = c2*c1 - s2*s1
      s3 = c2*s1 + s2*c1
 150  k1 = kk + kspan
      k2 = k1 + kspan
      k3 = k2 + kspan
      akp = a(kk) + a(k2)
      akm = a(kk) - a(k2)
      ajp = a(k1) + a(k3)
      ajm = a(k1) - a(k3)
      a(kk) = akp + ajp
      ajp = akp - ajp
      bkp = b(kk) + b(k2)
      bkm = b(kk) - b(k2)
      bjp = b(k1) + b(k3)
      bjm = b(k1) - b(k3)
      b(kk) = bkp + bjp
      bjp = bkp - bjp
      if (isn.lt.0) go to 180
      akp = akm - bjm
      akm = akm + bjm
      bkp = bkm + ajm
      bkm = bkm - ajm
      if (s1.eq.0.0d+0) go to 190
 160  a(k1) = akp*c1 - bkp*s1
      b(k1) = akp*s1 + bkp*c1
      a(k2) = ajp*c2 - bjp*s2
      b(k2) = ajp*s2 + bjp*c2
      a(k3) = akm*c3 - bkm*s3
      b(k3) = akm*s3 + bkm*c3
      kk = k3 + kspan
      if (kk.le.nt) go to 150
 170  kk = kk - nt + jc
      if (kk.le.mm) go to 130
c      MODIF HERE  (WAS .lt.)
      if (kk.le.kspan) go to 200
      kk = kk - kspan + inc
      if (kk.le.jc) go to 120
      if (kspan.eq.jc) go to 350
      go to 40
 180  akp = akm + bjm
      akm = akm - bjm
      bkp = bkm - ajm
      bkm = bkm + ajm
      if (s1.ne.0.0d+0) go to 160
 190  a(k1) = akp
      b(k1) = bkp
      a(k2) = ajp
      b(k2) = bjp
      a(k3) = akm
      b(k3) = bkm
      kk = k3 + kspan
      if (kk.le.nt) go to 150
      go to 170
 200  s1 = dble((kk-1)/jc)*dr*rad
      c1 = cos(s1)
      s1 = sin(s1)
      mm = min(kspan,mm+klim)
      go to 140
c
c transform for factor of 5 (optional code)
c
 210  c2 = c72**2 - s72**2
      s2 = 2.0d+0*c72*s72
 220  k1 = kk + kspan
      k2 = k1 + kspan
      k3 = k2 + kspan
      k4 = k3 + kspan
      akp = a(k1) + a(k4)
      akm = a(k1) - a(k4)
      bkp = b(k1) + b(k4)
      bkm = b(k1) - b(k4)
      ajp = a(k2) + a(k3)
      ajm = a(k2) - a(k3)
      bjp = b(k2) + b(k3)
      bjm = b(k2) - b(k3)
      aa = a(kk)
      bb = b(kk)
      a(kk) = aa + akp + ajp
      b(kk) = bb + bkp + bjp
      ak = akp*c72 + ajp*c2 + aa
      bk = bkp*c72 + bjp*c2 + bb
      aj = akm*s72 + ajm*s2
      bj = bkm*s72 + bjm*s2
      a(k1) = ak - bj
      a(k4) = ak + bj
      b(k1) = bk + aj
      b(k4) = bk - aj
      ak = akp*c2 + ajp*c72 + aa
      bk = bkp*c2 + bjp*c72 + bb
      aj = akm*s2 - ajm*s72
      bj = bkm*s2 - bjm*s72
      a(k2) = ak - bj
      a(k3) = ak + bj
      b(k2) = bk + aj
      b(k3) = bk - aj
      kk = k4 + kspan
      if (kk.lt.nn) go to 220
      kk = kk - nn
      if (kk.le.kspan) go to 220
      go to 290
c
c transform for odd factors
c
 230  k = nfac(i)
      kspnn = kspan
      kspan = kspan/k
      if (k.eq.3) go to 100
      if (k.eq.5) go to 210
      if (k.eq.jf) go to 250
      jf = k
      s1 = rad/(dble(k)/8.0d+0)
      c1 = cos(s1)
      s1 = sin(s1)
      ck(jf) = 1.0d+0
      sk(jf) = 0.0d+0
      j = 1
 240  ck(j) = ck(k)*c1 + sk(k)*s1
      sk(j) = ck(k)*s1 - sk(k)*c1
      k = k - 1
      ck(k) = ck(j)
      sk(k) = -sk(j)
      j = j + 1
      if (j.lt.k) go to 240
 250  k1 = kk
      k2 = kk + kspnn
      aa = a(kk)
      bb = b(kk)
      ak = aa
      bk = bb
      j = 1
      k1 = k1 + kspan
 260  k2 = k2 - kspan
      j = j + 1
      wt(j) = a(k1) + a(k2)
      ak = wt(j) + ak
      bt(j) = b(k1) + b(k2)
      bk = bt(j) + bk
      j = j + 1
      wt(j) = a(k1) - a(k2)
      bt(j) = b(k1) - b(k2)
      k1 = k1 + kspan
      if (k1.lt.k2) go to 260
      a(kk) = ak
      b(kk) = bk
      k1 = kk
      k2 = kk + kspnn
      j = 1
 270  k1 = k1 + kspan
      k2 = k2 - kspan
      jj = j
      ak = aa
      bk = bb
      aj = 0.0d+0
      bj = 0.0d+0
      k = 1
 280  k = k + 1
      ak = wt(k)*ck(jj) + ak
      bk = bt(k)*ck(jj) + bk
      k = k + 1
      aj = wt(k)*sk(jj) + aj
      bj = bt(k)*sk(jj) + bj
      jj = jj + j
      if (jj.gt.jf) jj = jj - jf
      if (k.lt.jf) go to 280
      k = jf - j
      a(k1) = ak - bj
      b(k1) = bk + aj
      a(k2) = ak + bj
      b(k2) = bk - aj
      j = j + 1
      if (j.lt.k) go to 270
      kk = kk + kspnn
      if (kk.le.nn) go to 250
      kk = kk - nn
      if (kk.le.kspan) go to 250
c
c multiply by rotation factor (except for factors of 2 and 4)
c
 290  if (i.eq.m) go to 350
      kk = jc + 1
 300  c2 = 1.0d+0- cd
      s1 = sd
      mm = min(kspan,klim)
      go to 320
 310  continue
      c2 = c1 - (cd*c1+sd*s1)
      s1 = s1 + (sd*c1-cd*s1)
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, they may
c be deleted.
c
      c1 = 0.50d+0/(c2**2+s1**2) + 0.50d+0
      s1 = c1*s1
      c2 = c1*c2
 320  c1 = c2
      s2 = s1
      kk = kk + kspan
 330  ak = a(kk)
      a(kk) = c2*ak - s2*b(kk)
      b(kk) = s2*ak + c2*b(kk)
      kk = kk + kspnn
      if (kk.le.nt) go to 330
      ak = s1*s2
      s2 = s1*c2 + c1*s2
      c2 = c1*c2 - ak
      kk = kk - nt + kspan
      if (kk.le.kspnn) go to 330
      kk = kk - kspnn + jc
      if (kk.le.mm) go to 310
c             MODIFICATION OF ORIGINAL CODE:
      if (kk.le.kspan) go to 340
c         SINGLETON's CODE was:
c     if (kk.lt.kspan) go to 340
      kk = kk - kspan + jc + inc
      if (kk.le.jc+jc) go to 300
      go to 40
 340  s1 = dble((kk-1)/jc)*dr*rad
      c2 = cos(s1)
      s1 = sin(s1)
      mm = min(kspan,mm+klim)
      go to 320
c
c permute the results to normal order---done in two stages
c permutation for square factors of n
c
 350  np(1) = ks
      if (kt.eq.0) go to 440
      k = kt + kt + 1
      if (m.lt.k) k = k - 1
      j = 1
      np(k+1) = jc
 360  np(j+1) = np(j)/nfac(j)
      np(k) = np(k+1)*nfac(j)
      j = j + 1
      k = k - 1
      if (j.lt.k) go to 360
      k3 = np(k+1)
      kspan = np(2)
      kk = jc + 1
      k2 = kspan + 1
      j = 1
      if (n.ne.ntot) go to 400
c
c permutation for single-variate transform (optional code)
c
 370  ak = a(kk)
      a(kk) = a(k2)
      a(k2) = ak
      bk = b(kk)
      b(kk) = b(k2)
      b(k2) = bk
      kk = kk + inc
      k2 = kspan + k2
      if (k2.lt.ks) go to 370
 380  k2 = k2 - np(j)
      j = j + 1
      k2 = np(j+1) + k2
      if (k2.gt.np(j)) go to 380
      j = 1
 390  if (kk.lt.k2) go to 370
      kk = kk + inc
      k2 = kspan + k2
      if (k2.lt.ks) go to 390
      if (kk.lt.ks) go to 380
      jc = k3
      go to 440
c
c permutation for multivariate transform
c
 400  k = kk + jc
 410  ak = a(kk)
      a(kk) = a(k2)
      a(k2) = ak
      bk = b(kk)
      b(kk) = b(k2)
      b(k2) = bk
      kk = kk + inc
      k2 = k2 + inc
      if (kk.lt.k) go to 410
      kk = kk + ks - jc
      k2 = k2 + ks - jc
      if (kk.lt.nt) go to 400
      k2 = k2 - nt + kspan
      kk = kk - nt + jc
      if (k2.lt.ks) go to 400
 420  k2 = k2 - np(j)
      j = j + 1
      k2 = np(j+1) + k2
      if (k2.gt.np(j)) go to 420
      j = 1
 430  if (kk.lt.k2) go to 400
      kk = kk + jc
      k2 = kspan + k2
      if (k2.lt.ks) go to 430
      if (kk.lt.ks) go to 420
      jc = k3
 440  if (2*kt+1.ge.m) return
      kspnn = np(kt+1)
c
c permutation for square-free factors of n
c
      j = m - kt
      nfac(j+1) = 1
 450  nfac(j) = nfac(j)*nfac(j+1)
      j = j - 1
      if (j.ne.kt) go to 450
      kt = kt + 1
      nn = nfac(kt) - 1
      jj = 0
      j = 0
      go to 480
 460  jj = jj - k2
      k2 = kk
      k = k + 1
      kk = nfac(k)
 470  jj = kk + jj
      if (jj.ge.k2) go to 460
      np(j) = jj
 480  k2 = nfac(kt)
      k = kt + 1
      kk = nfac(k)
      j = j + 1
      if (j.le.nn) go to 470
c
c determine the permutation cycles of length greater than 1
c
      j = 0
      go to 500
 490  k = kk
      kk = np(k)
      np(k) = -kk
      if (kk.ne.j) go to 490
      k3 = kk
 500  j = j + 1
      kk = np(j)
      if (kk.lt.0) go to 500
      if (kk.ne.j) go to 490
      np(j) = -j
      if (j.ne.nn) go to 500
      maxf = inc*maxf
c
c reorder a and b, following the permutation cycles
c
      go to 570
 510  j = j - 1
      if (np(j).lt.0) go to 510
      jj = jc
 520  kspan = jj
      if (jj.gt.maxf) kspan = maxf
      jj = jj - kspan
      k = np(j)
      kk = jc*k + i + jj
      k1 = kk + kspan
      k2 = 0
 530  k2 = k2 + 1
      wt(k2) = a(k1)
      bt(k2) = b(k1)
      k1 = k1 - inc
      if (k1.ne.kk) go to 530
 540  k1 = kk + kspan
      k2 = k1 - jc*(k+np(k))
      k = -np(k)
 550  a(k1) = a(k2)
      b(k1) = b(k2)
      k1 = k1 - inc
      k2 = k2 - inc
      if (k1.ne.kk) go to 550
      kk = k2
      if (k.ne.j) go to 540
      k1 = kk + kspan
      k2 = 0
 560  k2 = k2 + 1
      a(k1) = wt(k2)
      b(k1) = bt(k2)
      k1 = k1 - inc
      if (k1.ne.kk) go to 560
      if (jj.ne.0) go to 520
      if (j.ne.1) go to 510
 570  j = k3 + 1
      nt = nt - kspnn
      i = nt - inc + 1
      if (nt.ge.0) go to 510
      return
      end
