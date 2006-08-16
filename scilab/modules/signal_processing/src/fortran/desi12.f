      subroutine desi12(nmaxi,maxdeg,iapro,ndeg,vsn,gd1,gd2,adelta,
     *nzm,sm,nzero,pren,pimn,ugc,ack,nj,nh)
c!purpose
c chebyshev filter (passband or stopband)
c computation of the zeros and the locations of the extrema
c!calling sequence
c     subroutine desi12 (nmaxi,maxdeg,iapro,ndeg,vsn,gd1,gd2,adelta,
c    *nzm,sm,nzero,pren,pimn,ugc,ack,nj,nh)
c!
c
      implicit double precision (a-h,o-z)
      dimension nzm(*),sm(maxdeg,*),nzero(*),pren(*),pimn(*)
c
      external slamch
      real slamch
c
      pi=4.0d+0*atan(1.0d+0)
      flma=2.0d+0**(int(slamch('l'))-2)
      adelta = cosh(real(ndeg)*arcosh(vsn))
c
        fa = 1.0d+0
        nh = ndeg/2
        nj = (ndeg+1)/2
        fn = pi/(2.0d+0*dble(real(ndeg)))
c
      do 10 i=1,nj
        nzero(i) = 0
        inj = i + i - 1
        q = fn*dble(real(inj))
        pren(i) = sin(q)
        pimn(i) = cos(q)
  10  continue
c
      fn = 2.0d+0*fn
c
      if (iapro.eq.3) go to 40
c
      m = nj + 1
      do 20 i=1,nj
        j = m - i
      sm(i,1) = pimn(j)
20    continue
      nzm(1) = nj
      m = nh + 1
      do 30 i=1,m
      mi = m - i
        q = real(mi)*fn
        sm(i,2) = cos(q)
  30  continue
      nzm(2) = m
      sm(1,3) = vsn
      nzm(3) = 1
      sm(1,4) = flma
      nzm(4) = 1
      nzero(1) = ndeg
c
      ugc = gd2/adelta
cccccccccccccccccccccccccccccccccccccc
c The two lines below were missing
      ogc = gd1
      go to 80
cccccccccccccccccccccccccccccccccccccc
  40  sm(1,1) = 0.0d+0
      nzm(1) = 1
      sm(1,2) = 1.0d+0
      nzm(2) = 1
      do 50 i=1,nj
        inj = nj - i
        q = real(inj)*fn
        sm(inj+1,3) = vsn/cos(q)
  50  continue
      nzm(3) = nj
      do 60 i=1,nh
        nzero(i) = 2
        q = pimn(i)
        fa = fa*q*q
        sm(i,4) = vsn/q
  60  continue
      if (nh.eq.nj) go to 70
      nzero(nj) = 1
       sm(nj,4) = flma
  70  nzm(4) = nj
c
      ugc = gd2
      ogc = gd1*adelta
  80  ack = fa
      sm(nmaxi-1,4) = 1.0d+0
      return
      end

