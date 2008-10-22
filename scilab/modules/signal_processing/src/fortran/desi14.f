      subroutine desi14(nmaxi,maxdeg,ndeg,vsn,gd1,gd2,adelta,nzm,
     *sm,nzero,dsk,ugc,ogc,ack,nj,nh,dk,dks,dcap02,dcap04)
c!purpose
c elliptic filter
c computation of the zeros and the locations of the extrema
c!
c
      implicit double precision (a-h,o-z)
      double precision dsk(*)
      dimension nzero(*),sm(maxdeg,*),nzm(*)
c
      external slamch
      real slamch
c
      data de /1.0d+00/
      dpi=4.0d+0*atan(1.0d+00)
      flma=2.0d+0**(int(slamch('l'))-2)
c
      dcap02 = de/vsn
      dcap01 = sqrt(dcap02)
        dcap04 = sqrt(de-dcap02*dcap02)
      dk = dellk(dcap02)
      dks = dellk(dcap04)
c
      dq = exp(-dpi*dks/dk)
c
      nh = ndeg/2
      m = ndeg + 1
      nj = m/2
      mh = nh + 1
c
      dn = dk/dble(real(ndeg))
c
      del1 = de
      if (nh.eq.0) go to 20
      do 10 i=1,nh
        inh = m - i - i
        du = dn*dble(real(inh))
        dm = dsn2(du,dk,dq)
        del1 = del1*dm*dcap01
         dsk(i) = dm
        j = mh - i
        sm(j,1) = dm
        nzero(i) = 2
        dde = de/(dcap02*dm)
        sm(i,4) = dde
  10  continue
      go to 30
  20  sm(1,1) = 0.0d+0
  30  kj = nj - 1
      mj = nj + 1
      del2 = de
      if (kj.eq.0) go to 50
      do 40 i=1,kj
        ndegi = ndeg - i - i
        du = dn*dble(real(ndegi))
        dm = dsn2(du,dk,dq)
        j = mj - i
        sm(j,2) = dm
        dde = de/(dcap02*dm)
          sm(i+1,3) = dde
        del2 = del2*dm*dcap01
  40  continue
      go to 60
  50  sm(ndeg,2) = 1.0d+0
      sm(1,3) = vsn
c
  60  ddelt = del1*del1
      adelta = ddelt
      ack = 1.0d+0/adelta
      if (nh.eq.nj) go to 80
      ack = ack*dcap01
      ddelta = del2*del2*dcap01
      adelta = ddelta
      dsk(nj) = 0.0d+0
      nzero(nj) = 1
      sm(nj,4) = flma
c
      if (nh.eq.0) go to 90
      do 70 i=1,nh
         j = mj - i
        sm(j,1) = sm(j-1,1)
        sm(i,2) = sm(i+1,2)
  70  continue
      sm(1,1) = 0.0d+0
      go to 90
c
  80  sm(mh,3) = flma
      sm(1,2) = 0.0d+0
c
  90  nzm(1) = nj
      nzm(4) = nj
      nzm(2) = mh
      nzm(3) = mh
      sm(mh,2) = 1.0d+0
      sm(1,3) = vsn
      ugc = gd2*adelta
      ogc = gd1/adelta
      sm(nmaxi-1,4) = 1.0d+0
      return
      end
