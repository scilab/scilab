      subroutine tranze(nmaxi,maxdeg,ityp,ndeg,nzm,a,vd,sm,nzero)
c!purpose
c reactance transformation of the zeros and the locations of the
c extrema
c!
c
      implicit double precision (a-h,o-z)
c
      external slamch
      real slamch
      dimension nzm(*),nzero(*)
      double precision sm(maxdeg,*)
c
      dimension  msm(4)
c
      flma=2.0d+0**(int(slamch('l'))-2)
      flmi=2.0d+0*dlamch('p')
      fa = 1.0d+0
      if (ityp.eq.1) go to 190
       if (ityp.eq.3) go to 60
c
      me = nzm(4)
      do 10 i=1,me
        q = sm(i,4)
        if (q.lt.flma) fa = fa*q
  10  continue
c
      fa = fa*fa
c
c lowpass - highpass
c
      do 50 j=1,4
        me = nzm(j)
        do 40 i=1,me
          qi = sm(i,j)
          if (abs(qi).lt.flmi) go to 20
          qi = 1.0d+0/qi
          go to 30
  20      qi = flma
   30      sm(i,j) = qi
  40    continue
  50  continue
      go to 90
  60  do 80 j=1,2
        me = nzm(j)
        ma = me + 1
        me = me/2
        do 70 i=1,me
          qi = sm(i,j)
          ii = ma - i
          sm(i,j) = sm(ii,j)
          sm(ii,j) = qi
  70    continue
  80  continue
c
  90  if (ityp.eq.2) go to 190
c
c lowpass - bandpass transformation
c
       qa = 2.0d+0*a
      nn = ndeg + 1
      if (ityp.eq.4) go to 110
c
      msm(1) = 1
      if (nzm(1).ne.1) msm(1) = ndeg
      msm(2) = 2
      if (nzm(2).ne.1) msm(2) = nn
      do 100 j=3,4
        msm(j) = 2*nzm(j)
 100  continue
      go to 130
c
 110  do 120 j=1,2
        msm(j) = 2*nzm(j)
 120  continue
      msm(3) = 2
      if (nzm(3).ne.1) msm(3) = nn
      msm(4) = 1
      if (nzm(4).ne.1) msm(4) = ndeg
c
 130  s = 1.0d+0
      do 180 j=1,4
        me = nzm(j)
        ma = msm(j)
        nzm(j) = ma
        if (j.eq.3) s = -1.0d+0
        do 170 i=1,me
          qr = sm(i,j)
          nu = nzero(i)
          if (abs(qr).lt.flma) go to 150
          if (j.ne.4) go to 140
          fa = fa*(vd/a)**nu
 140      qi = qr
          go to 160
c
 150      qr = qr/qa
          dr = qr
          dqi = sqrt(dr*dr+1.0d+0)
          qi = dqi
  160      sm(i,j) = qi - s*qr
          ii = ma - i + 1
          if (abs(qr).lt.flmi) nu = 2*nu
          if (j.eq.4) nzero(ii) = nu
          sm(ii,j) = qi + s*qr
 170    continue
 180  continue
c
 190  do 220 j=1,4
        me = nzm(j)
        do 210 i=1,me
          q = sm(i,j)
          if (q.lt.flma) go to 200
          if (j.ne.4 .or. ityp.ge.3) go to 210
          nu = nzero(i)
          fa = fa*vd**nu
          go to 210
 200      sm(i,j) = q*vd
 210    continue
 220  continue
       sm(nmaxi-1,4) = sm(nmaxi-1,4)*fa
      return
      end
