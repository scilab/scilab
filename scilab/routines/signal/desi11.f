      subroutine desi11(nmaxi,maxdeg,vsn,ndeg,gd1,gd2,adelta,nzm,sm
     *,nzero,pren,pimn,ugc,ogc,nj,nh)
c!purpose
c butterworth filter
c computation of the zeros and locations of the extrema
c!
c
      implicit double precision (a-h,o-z)
      external slamch
      real slamch
      dimension  nzm(*),sm(maxdeg,4),nzero(*),pren(*),pimn(*)
c
      pi=4.0d+0*atan(1.0d+0)
      flma=2.0d+0**(int(slamch('l'))-2)
c
      adelta = vsn**ndeg
c
      nh = ndeg/2
      nj = (ndeg+1)/2
      fdeg = real(ndeg)
      fn = pi/2.0d+0/fdeg
c
      do 10 i=1,nj
        nzero(i) = 0
        iii = i + i - 1
        q = fn*real(iii)
        pren(i) = sin(q)
        pimn(i) = cos(q)
  10  continue
c
      fn = 2.0d+0*fn
      nzero(1) = ndeg
      nzm(1) = 1
      sm(1,1) = 0.0d+0
      nzm(2) = 1
      sm(1,2) = 1.0d+0
      nzm(3) = 1
      sm(1,3) = vsn
      nzm(4) = 1
      sm(1,4) = flma
c
      ugc = gd2/adelta
      ogc = gd1
      sm(nmaxi-1,4) = 1.0d+0
      return
      end
