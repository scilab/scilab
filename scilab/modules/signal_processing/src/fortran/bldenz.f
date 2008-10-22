      subroutine bldenz(ndeg,nj,zfa,zpr,zpi,nb,fact,c1,c0)
c!purpose
c  build denominator blocks of second order  --  z-domain
c!calling sequence
c     subroutine bldenz (ndeg,nj,zfa,zpr,zpi,nb,fact,c1,c0)
c     implicit double precision (a-h,o-z)
c     double precision zpr(*),zpi(*)
c     double precision c1(*),c0(*)
c!
      implicit double precision (a-h,o-z)
      double precision zpr(*),zpi(*)
      double precision c1(*),c0(*)
      flmi=2.0d+0*dlamch('p')
      nb = (ndeg+1)/2
      n = 0
      fact = zfa
      do 40 i=1,nb
        n = n + 1
        qr = zpr(n)
        qi = zpi(n)
         if (abs(qi).lt.flmi) go to 10
        c1(i) = -2.0d+0*qr
        c0(i) = qr*qr + qi*qi
        go to 40
  10    if (n.ge.nj) go to 20
        if (abs(zpi(n+1)).lt.flmi) go to 30
  20    c1(i) = -qr
        c0(i) = 0.0d+0
        go to 40
  30    n = n + 1
        qi = zpr(n)
        c1(i) = -qr - qi
        c0(i) = qr*qi
  40  continue
      return
      end
